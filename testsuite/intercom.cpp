/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/String.h>
#include <base/net/InetAddress.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/Semaphore.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/collection/Queue.h>
#include <base/Timer.h>
#include <base/net/ServerSocket.h>
#include <base/UnsignedInteger.h>
#include <base/sound/SoundInputStream.h>
#include <base/sound/SoundOutputStream.h>
#include <base/io/EndOfFile.h>

using namespace base;

class IntercomServlet : public virtual Object {
public:

  class Recorder : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet;
  public:

    Recorder(IntercomServlet* object) throw() : intercomServlet(object) {}

    void run() throw() {
      intercomServlet->record();
    }
  };

  class Player : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet;
  public:

    Player(IntercomServlet* object) throw() : intercomServlet(object) {}
  
    void run() throw() {
      intercomServlet->play();
    }
  };

  class Reader : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet;
  public:

    Reader(IntercomServlet* object) throw() : intercomServlet(object) {}

    void run() throw() {
      intercomServlet->read();
    }
  };

  class Writer : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet;
  public:

    Writer(IntercomServlet* object) throw() : intercomServlet(object) {}

    void run() throw() {
      intercomServlet->write();
    }
  };
private:

  Recorder recorder;
  Player player;
  Reader reader;
  Writer writer;
  Queue<Allocator<short>*> recordingQueue;
  Queue<Allocator<short>*> playingQueue;
  Queue<Allocator<short>*> readingQueue;
  Queue<Allocator<short>*> writingQueue;
  Semaphore recordingSemaphore;
  Semaphore playingSemaphore;
  Semaphore readingSemaphore;
  Semaphore writingSemaphore;
  MutualExclusion guard;
  unsigned int channels;
  unsigned int sampleRate;
  bool isServer;
  bool loopback;
  InetEndPoint endPoint;
  StreamSocket streamSocket;
public:

  IntercomServlet(unsigned int channels, unsigned int sampleRate, bool loopback, bool isServer, const InetEndPoint& endPoint) throw() :
    recorder(this),
    player(this),
    reader(this),
    writer(this) {
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->isServer = isServer;
    this->endPoint = endPoint;
    this->loopback = loopback;
  }
  
  void record() throw() {
    SoundInputStream soundInputStream(sampleRate, channels);
    soundInputStream.resume();
    while (!Thread::getThread()->isTerminated()) {
      recordingSemaphore.wait();
      if (Thread::getThread()->isTerminated()) {
        break;
      }
      if (!recordingQueue.isEmpty()) {
        guard.exclusiveLock();
        Allocator<short>* buffer = recordingQueue.pop();
        guard.releaseLock();
        unsigned int bytesRead = soundInputStream.read(buffer->getElements(), buffer->getByteSize());
        guard.exclusiveLock();
        if (loopback) {
          playingQueue.push(buffer);
          playingSemaphore.post();
        } else {
          writingQueue.push(buffer);
          writingSemaphore.post();
        }
        guard.releaseLock();
      }
    }
    soundInputStream.pause();
    fout << MESSAGE("Recording thread terminating") << ENDL;
  }

  void play() throw() {
    SoundOutputStream soundOutputStream(sampleRate, channels);
    soundOutputStream.resume();
    while (!Thread::getThread()->isTerminated()) {
      playingSemaphore.wait();
      if (Thread::getThread()->isTerminated()) {
        break;
      }
      if (!playingQueue.isEmpty()) {
        guard.exclusiveLock();
        Allocator<short>* buffer = playingQueue.pop();
        guard.releaseLock();
        unsigned int bytesWritten = soundOutputStream.write(buffer->getElements(), buffer->getByteSize());
        guard.exclusiveLock();
        if (loopback) {
          recordingQueue.push(buffer);
          recordingSemaphore.post();
        } else {
          readingQueue.push(buffer);
          readingSemaphore.post();
        }
        guard.releaseLock();
      }
    }
    soundOutputStream.reset();
    fout << MESSAGE("Playing thread terminating") << ENDL;
  }

  void write() throw() {
    if (loopback) {
      return;
    }
    while (!Thread::getThread()->isTerminated()) {
      writingSemaphore.wait();
      if (Thread::getThread()->isTerminated()) {
        break;
      }
      if (!writingQueue.isEmpty()) {
        guard.exclusiveLock();
        Allocator<short>* buffer = writingQueue.pop();
        guard.releaseLock();
        try {
          streamSocket.write(pointer_cast<const char*>(buffer->getElements()), buffer->getByteSize());
        } catch(IOException& e) {
          fout << MESSAGE("IOException: ") << e.getMessage() << ENDL;
          Application::getApplication()->terminate();
          break;
        }
        guard.exclusiveLock();
        recordingQueue.push(buffer);
        recordingSemaphore.post();
        guard.releaseLock();
      }
    }
    streamSocket.shutdownOutputStream();
    fout << MESSAGE("Writing thread terminating") << ENDL;
  }

  void read() throw() {
    if (loopback) {
      return;
    }
    while (!Thread::getThread()->isTerminated()) {
      readingSemaphore.wait();
      if (Thread::getThread()->isTerminated()) {
        break;
      }
      if (!readingQueue.isEmpty()) {
        guard.exclusiveLock();
        Allocator<short>* buffer = readingQueue.pop();
        guard.releaseLock();
        try {
          unsigned int bytesRead = streamSocket.read(pointer_cast<char*>(buffer->getElements()), buffer->getByteSize());
        } catch(EndOfFile& e) {
          fout << MESSAGE("Connection terminated by remote host") << ENDL;
          Application::getApplication()->terminate();
          break;
        } catch(IOException& e) {
          fout << MESSAGE("IO error: ") << e.getMessage() << ENDL;
          Application::getApplication()->terminate();
          break;
        }
        guard.exclusiveLock();
        playingQueue.push(buffer);
        playingSemaphore.post();
        guard.releaseLock();
      }
    }
    fout << MESSAGE("Reading thread terminating") << ENDL;
  }

  bool hostAllowed(const InetAddress& host) throw() {
    // check address 172.30.* (mask then check if equal)
    return true;
  }

  void server() throw() {
    fout << MESSAGE("Initializing server socket: ") << endPoint << ENDL;
    ServerSocket serverSocket(endPoint.getAddress(), endPoint.getPort(), 1);

    fout << MESSAGE("Waiting for client...") << ENDL;
    streamSocket = serverSocket.accept();
    fout << MESSAGE("Connection from: ") << InetEndPoint(streamSocket.getAddress(), streamSocket.getPort()) << ENDL;
    assert(hostAllowed(streamSocket.getAddress()), OutOfDomain());
  }

  void client() throw() {
    fout << MESSAGE("Connecting to server: ") << endPoint << ENDL;
    streamSocket.connect(endPoint.getAddress(), endPoint.getPort());
    fout << MESSAGE("Connected to: ") << InetEndPoint(streamSocket.getAddress(), streamSocket.getPort())<< ENDL;
  }
  
  void run() throw() {
    fout << MESSAGE("Allocating buffers...") << ENDL;
    for (unsigned int i = 0; i < 16; ++i) {
      recordingQueue.push(new Allocator<short>(16*256)); //(sampleRate/16+4095)/4096)*4096)
      recordingSemaphore.post();
    }
    for (unsigned int i = 0; i < 16; ++i) {
      readingQueue.push(new Allocator<short>(16*256)); //(sampleRate/16+4095)/4096)*4096)
      readingSemaphore.post();
    }

    fout << MESSAGE("Creating threads...") << ENDL;
    Thread recorderThread(&recorder);
    Thread playerThread(&player);
    Thread readerThread(&reader);
    Thread writerThread(&writer);

    if (!loopback) {
      if (isServer) {
        server();
      } else {
        client();
      }
    }

    fout << MESSAGE("Starting threads...") << ENDL;
    recorderThread.start();
    playerThread.start();
    readerThread.start();
    writerThread.start();

    fout << MESSAGE("Waiting...") << ENDL;
    Timer timer;

    for (unsigned int i = 0; i < 30*(1000/500); ++i) {
      if (Application::getApplication()->isTerminated()) {
        break;
      }
      Thread::millisleep(maximum<int>((i+1)*500 - timer.getLiveMicroseconds()/1000, 0));
      if (!loopback) {
        fout << MESSAGE("Time: ") << setPrecision(3) << timer.getLiveMicroseconds()/1000000. << MESSAGE(" - ")
             << MESSAGE("Recording queue: ") << recordingQueue.getSize() << MESSAGE(" - ")
             << MESSAGE("Playing queue: ") << playingQueue.getSize() << MESSAGE(" - ")
             << MESSAGE("Reading queue: ") << readingQueue.getSize() << MESSAGE(" - ")
             << MESSAGE("Writing queue: ") << writingQueue.getSize() << CR
             << FLUSH;
      } else {
        fout << MESSAGE("Time: ") << setPrecision(3) << timer.getLiveMicroseconds()/1000000. << MESSAGE(" - ")
             << MESSAGE("Recording queue: ") << recordingQueue.getSize() << MESSAGE(" - ")
             << MESSAGE("Playing queue: ") << playingQueue.getSize() << CR
             << FLUSH;
      }
    }
    fout << ENDL;

    if (!Application::getApplication()->isTerminated()) {
      fout << MESSAGE("Voluntary termination") << ENDL;
      Application::getApplication()->terminate();
    }

    fout << MESSAGE("Waiting for threads to terminate...") << ENDL;
    recorderThread.terminate();
    recordingSemaphore.post();
    recorderThread.join();
    playerThread.terminate();
    playingSemaphore.post();
    playerThread.join();

    readerThread.terminate();
    readingSemaphore.post();
    readerThread.join();
    writerThread.terminate();
    writingSemaphore.post();
    writerThread.join();

    fout << MESSAGE("Releasing buffers...") << ENDL;
    while (!recordingQueue.isEmpty()) {
      delete recordingQueue.pop();
    }
    while (!playingQueue.isEmpty()) {
      delete playingQueue.pop();
    }
    while (!readingQueue.isEmpty()) {
      delete readingQueue.pop();
    }
    while (!writingQueue.isEmpty()) {
      delete writingQueue.pop();
    }

    fout << MESSAGE("Completed") << ENDL;
  }
  
  ~IntercomServlet() throw() {}
};

class IntercomApplication : public Application {
private:

  bool loopback;
  bool isServer;
  String host;
  unsigned short port;
  unsigned int channels;
  unsigned int samplingRate;
  IntercomServlet* intercomServlet;
public:
  
  /** The default port. */
  static const unsigned short DEFAULT_PORT = 1234;
  static const unsigned int DEFAULT_CHANNELS = 1;
  static const unsigned int DEFAULT_SAMPLE_RATE = 44100;
  
  IntercomApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("intercom"), numberOfArguments, arguments, environment) {
    loopback = false;
    isServer = true;
    port = DEFAULT_PORT;
    channels = DEFAULT_CHANNELS;
    samplingRate = DEFAULT_SAMPLE_RATE;
  }

  void handleArguments() throw(OutOfDomain) {
    bool loopbackSpecified = false;
    bool portSpecified = false;
    bool hostSpecified = false;
    bool stereoSpecified = false;
    bool rateSpecified = false;
    
    Array<String>::ReadEnumerator enu = Application::getApplication()->getArguments().getReadEnumerator();

    // connect to host:port or server using port
    
    while (enu.hasNext()) {
      const String* argument = enu.next();
      if (*argument == "--help") {
      } else if (*argument == "--loopback") {
        assert(!loopbackSpecified && !hostSpecified && !portSpecified, OutOfDomain());
        loopbackSpecified = true;
        loopback = true;
      } else if (*argument == "--stereo") {
        assert(!stereoSpecified, OutOfDomain());
        stereoSpecified = true;
        channels = 2;
      } else if (*argument == "--host") {
        assert(!hostSpecified && !loopbackSpecified, OutOfDomain("Already specified"));
        assert(enu.hasNext(), OutOfDomain("Host value missing"));
        host = *enu.next();
        hostSpecified = true;
        isServer = false;
      } else if (*argument == "--port") {
        assert(!portSpecified && !loopbackSpecified, OutOfDomain());
        assert(enu.hasNext(), OutOfDomain("Port value missing"));
        const String* rateString = enu.next();
        unsigned int temp = UnsignedInteger(*rateString).getValue();
        assert((temp > 0) && (temp <= 0xffff), OutOfDomain("Port is invalid"));
        portSpecified = true;
        port = temp;
      } else if (*argument == "--rate") {
        assert(!rateSpecified, OutOfDomain());
        assert(enu.hasNext(), OutOfDomain("Rate value missing"));
        const String* rateString = enu.next();
        unsigned int temp = UnsignedInteger(*rateString).getValue();
        assert((temp >= 1000) && (temp <= 44100), OutOfDomain("Sampling rate is invalid"));
        rateSpecified = true;
        samplingRate = temp;
      } else {
        throw OutOfDomain("Invalid argument");
      }
    }
  }

  void onTermination() throw() {
    // override default application termination
  }

  void main() throw(OutOfDomain) {
    fout << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    handleArguments();

    //ASSERT(SoundInputDevice::getNumberOfDevices() > 0);
    //ASSERT(SoundOutputDevice::getNumberOfDevices() > 0);
    
    //fout << MESSAGE("Number of input devices: ") << SoundInputDevice::getNumberOfDevices() << ENDL;
    //fout << MESSAGE("Number of output devices: ") << SoundOutputDevice::getNumberOfDevices() << ENDL;
    /*
    if (!SoundInputDevice::isFormatSupported(channels, samplingRate)) {
      ferr << "Input device does not support sound format" << ENDL;
      return 1;
    }

    if (!SoundOutputDevice::isFormatSupported(channels, samplingRate)) {
      ferr << "Output device does not support sound format" << ENDL;
      return 1;
    }
    */
    InetEndPoint endPoint;
    if (isServer) {
      endPoint.setAddress(InetAddress());
      endPoint.setPort(port);
    } else {
      List<InetAddress>::ReadEnumerator enu = InetAddress::getAddressesByName(host).getReadEnumerator();
      endPoint.setAddress(*enu.next());
      endPoint.setPort(port);
    }
    IntercomServlet(channels, samplingRate, loopback, isServer, endPoint).run();
  }
  
};

int main(int argc, const char* argv[], const char* env[]) {
  IntercomApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
