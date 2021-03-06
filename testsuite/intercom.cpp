/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

using namespace com::azure::dev::base;

class IntercomServlet : public virtual Object {
public:

  class Recorder : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet = nullptr;
  public:

    Recorder(IntercomServlet* object) noexcept
      : intercomServlet(object) {
    }

    void run() noexcept {
      intercomServlet->record();
    }
  };

  class Player : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet = nullptr;
  public:

    Player(IntercomServlet* object) noexcept
      : intercomServlet(object) {
    }
  
    void run() noexcept {
      intercomServlet->play();
    }
  };

  class Reader : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet = nullptr;
  public:

    Reader(IntercomServlet* object) noexcept
      : intercomServlet(object) {
    }

    void run() noexcept {
      intercomServlet->read();
    }
  };

  class Writer : public virtual Runnable {
  private:

    IntercomServlet* intercomServlet = nullptr;
  public:

    Writer(IntercomServlet* object) noexcept
      : intercomServlet(object) {
    }

    void run() noexcept {
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
  unsigned int channels = 0;
  unsigned int sampleRate = 0;
  bool isServer = false;
  bool loopback = false;
  InetEndPoint endPoint;
  StreamSocket streamSocket;
public:

  IntercomServlet(unsigned int channels,
                  unsigned int sampleRate,
                  bool loopback,
                  bool isServer,
                  const InetEndPoint& endPoint) noexcept
    : recorder(this),
      player(this),
      reader(this),
      writer(this)
  {
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->isServer = isServer;
    this->endPoint = endPoint;
    this->loopback = loopback;
  }
  
  void record() noexcept
  {
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
        /*unsigned int bytesRead =*/ soundInputStream.read(
          buffer->getElements(),
          buffer->getByteSize()
        );
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
    fout << "Recording thread terminating" << ENDL;
  }

  void play() noexcept {
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
        /*unsigned int bytesWritten =*/ soundOutputStream.write(
          buffer->getElements(),
          buffer->getByteSize()
        );
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
    fout << "Playing thread terminating" << ENDL;
  }

  void write() noexcept {
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
          streamSocket.write(
            Cast::pointer<const uint8*>(buffer->getElements()),
            buffer->getByteSize()
          );
        } catch (IOException& e) {
          fout << "IOException: " << e.getMessage() << ENDL;
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
    fout << "Writing thread terminating" << ENDL;
  }

  void read() noexcept {
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
          /*unsigned int bytesRead =*/ streamSocket.read(
            Cast::pointer<uint8*>(buffer->getElements()),
            buffer->getByteSize()
          );
        } catch (EndOfFile&) {
          fout << "Connection terminated by remote host" << ENDL;
          Application::getApplication()->terminate();
          break;
        } catch (IOException& e) {
          fout << "IO error: " << e.getMessage() << ENDL;
          Application::getApplication()->terminate();
          break;
        }
        guard.exclusiveLock();
        playingQueue.push(buffer);
        playingSemaphore.post();
        guard.releaseLock();
      }
    }
    fout << "Reading thread terminating" << ENDL;
  }

  bool hostAllowed(const InetAddress& host) noexcept
  {
    // check address 172.30.* (mask then check if equal)
    return true;
  }

  void server()
  {
    fout << "Initializing server socket: " << endPoint << ENDL;
    ServerSocket serverSocket(endPoint.getAddress(), endPoint.getPort(), 1);

    fout << "Waiting for client..." << ENDL;
    streamSocket = serverSocket.accept();
    fout << "Connection from: "
         << InetEndPoint(streamSocket.getAddress(), streamSocket.getPort())
         << ENDL;
    if (!hostAllowed(streamSocket.getAddress())) {
      throw OutOfDomain();
    }
  }

  void client() noexcept
  {
    fout << "Connecting to server: " << endPoint << ENDL;
    streamSocket.connect(endPoint.getAddress(), endPoint.getPort());
    fout << "Connected to: "
         << InetEndPoint(streamSocket.getAddress(), streamSocket.getPort())
         << ENDL;
  }
  
  void run() noexcept {
    fout << "Allocating buffers..." << ENDL;
    for (unsigned int i = 0; i < 16; ++i) {
      recordingQueue.push(new Allocator<short>(16*256)); //(sampleRate/16+4095)/4096)*4096)
      recordingSemaphore.post();
    }
    for (unsigned int i = 0; i < 16; ++i) {
      readingQueue.push(new Allocator<short>(16*256)); //(sampleRate/16+4095)/4096)*4096)
      readingSemaphore.post();
    }

    fout << "Creating threads..." << ENDL;
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

    fout << "Starting threads..." << ENDL;
    recorderThread.start();
    playerThread.start();
    readerThread.start();
    writerThread.start();

    fout << "Waiting..." << ENDL;
    Timer timer;

    for (unsigned int i = 0; i < 30*(1000/500); ++i) {
      if (Application::getApplication()->isTerminated()) {
        break;
      }
      Thread::millisleep(maximum<int>((i+1)*500 - timer.getLiveMicroseconds()/1000, 0));
      if (!loopback) {
        fout << "Time: " << setPrecision(3)
             << timer.getLiveMicroseconds()/1000000. << " - "
             << "Recording queue: " << recordingQueue.getSize() << " - "
             << "Playing queue: " << playingQueue.getSize() << " - "
             << "Reading queue: " << readingQueue.getSize() << " - "
             << "Writing queue: " << writingQueue.getSize() << CR
             << FLUSH;
      } else {
        fout << "Time: " << setPrecision(3)
             << timer.getLiveMicroseconds()/1000000. << " - "
             << "Recording queue: " << recordingQueue.getSize() << " - "
             << "Playing queue: " << playingQueue.getSize() << CR
             << FLUSH;
      }
    }
    fout << ENDL;

    if (!Application::getApplication()->isTerminated()) {
      fout << "Voluntary termination" << ENDL;
      Application::getApplication()->terminate();
    }

    fout << "Waiting for threads to terminate..." << ENDL;
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

    fout << "Releasing buffers..." << ENDL;
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

    fout << "Completed" << ENDL;
  }
  
  ~IntercomServlet() noexcept {
  }
};

class IntercomApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  bool loopback = false;
  bool isServer = false;
  String host;
  unsigned short port = 0;
  unsigned int channels = 0;
  unsigned int samplingRate = 0;
  IntercomServlet* intercomServlet = nullptr;
public:
  
  /** The default port. */
  static const unsigned short DEFAULT_PORT = 1234;
  static const unsigned int DEFAULT_CHANNELS = 1;
  static const unsigned int DEFAULT_SAMPLE_RATE = 44100;
  
  IntercomApplication()
    : Application("intercom")
  {
    loopback = false;
    isServer = true;
    port = DEFAULT_PORT;
    channels = DEFAULT_CHANNELS;
    samplingRate = DEFAULT_SAMPLE_RATE;
  }

  void handleArguments() {
    bool loopbackSpecified = false;
    bool portSpecified = false;
    bool hostSpecified = false;
    bool stereoSpecified = false;
    bool rateSpecified = false;
    
    Array<String>::ReadEnumerator enu = getArguments().getReadEnumerator();

    // connect to host:port or server using port
    
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--help") {
      } else if (argument == "--loopback") {
        bassert(!loopbackSpecified && !hostSpecified && !portSpecified, OutOfDomain());
        loopbackSpecified = true;
        loopback = true;
      } else if (argument == "--stereo") {
        bassert(!stereoSpecified, OutOfDomain());
        stereoSpecified = true;
        channels = 2;
      } else if (argument == "--host") {
        bassert(!hostSpecified && !loopbackSpecified, OutOfDomain("Already specified."));
        bassert(enu.hasNext(), OutOfDomain("Host value missing."));
        host = enu.next();
        hostSpecified = true;
        isServer = false;
      } else if (argument == "--port") {
        bassert(!portSpecified && !loopbackSpecified, OutOfDomain());
        bassert(enu.hasNext(), OutOfDomain("Port value missing."));
        const String& rateString = enu.next();
        unsigned int temp = UnsignedInteger(rateString).getValue();
        bassert((temp > 0) && (temp <= 0xffff), OutOfDomain("Port is invalid."));
        portSpecified = true;
        port = temp;
      } else if (argument == "--rate") {
        bassert(!rateSpecified, OutOfDomain());
        bassert(enu.hasNext(), OutOfDomain("Rate value missing."));
        const String& rateString = enu.next();
        unsigned int temp = UnsignedInteger(rateString).getValue();
        bassert((temp >= 1000) && (temp <= 44100), OutOfDomain("Sampling rate is invalid."));
        rateSpecified = true;
        samplingRate = temp;
      } else {
        _throw OutOfDomain("Invalid argument.");
      }
    }
  }

  void onTermination() noexcept
  {
    // override default application termination
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    handleArguments();

    //ASSERT(SoundInputDevice::getNumberOfDevices() > 0);
    //ASSERT(SoundOutputDevice::getNumberOfDevices() > 0);
    
    //fout << "Number of input devices: " << SoundInputDevice::getNumberOfDevices() << ENDL;
    //fout << "Number of output devices: " << SoundOutputDevice::getNumberOfDevices() << ENDL;
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
      List<InetAddress>::ReadEnumerator enu =
        InetAddress::getAddressesByName(host).getReadEnumerator();
      endPoint.setAddress(enu.next());
      endPoint.setPort(port);
    }
    IntercomServlet(channels, samplingRate, loopback, isServer, endPoint).run();
  }

  ~IntercomApplication() noexcept {
  }
};

APPLICATION_STUB(IntercomApplication);
