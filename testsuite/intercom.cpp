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
private:

  Recorder recorder;
  Player player;
  Queue<Allocator<short>* > recordingQueue;
  Queue<Allocator<short>* > playingQueue;
  Semaphore recordingSemaphore;
  Semaphore playingSemaphore;
  MutualExclusion guard;
  unsigned int channels;
  unsigned int sampleRate;
  unsigned int recorderCount;
  unsigned int playerCount;
  bool isServer;
  InetEndPoint endPoint;
  StreamSocket clientSocket;
public:

  IntercomServlet(unsigned int channels, unsigned int sampleRate, bool isServer, const InetEndPoint& endPoint) throw() :
    recorder(this),
    player(this) {
    this->channels = channels;
    this->sampleRate = sampleRate;
    recorderCount = 0;
    playerCount = 0;
    this->isServer = isServer;
    this->endPoint = endPoint;
  }
  
  void record() throw() {
    if (isServer) {
      ++recorderCount;
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
          unsigned int bytesRecorded = soundInputStream.read(buffer->getElements(), buffer->getByteSize());
          guard.exclusiveLock();
          playingQueue.push(buffer);
          playingSemaphore.post();
          guard.releaseLock();
        }
      }
      soundInputStream.pause();
      ++recorderCount;
    } else {
      ++recorderCount;
      while (!Thread::getThread()->isTerminated()) {
        recordingSemaphore.wait();
        if (Thread::getThread()->isTerminated()) {
          break;
        }
        if (!recordingQueue.isEmpty()) {
          guard.exclusiveLock();
          Allocator<short>* buffer = recordingQueue.pop();
          guard.releaseLock();
          unsigned int bytesRecorded = clientSocket.read(pointer_cast<char*>(buffer->getElements()), buffer->getByteSize());
          guard.exclusiveLock();
          playingQueue.push(buffer);
          playingSemaphore.post();
          guard.releaseLock();
        }
      }
      ++recorderCount;
    }
  }

  void play() throw() {
    if (isServer) {
      ++playerCount;
      while (!Thread::getThread()->isTerminated()) {
        playingSemaphore.wait();
        if (Thread::getThread()->isTerminated()) {
          break;
        }
        if (!playingQueue.isEmpty()) {
          guard.exclusiveLock();
          Allocator<short>* buffer = playingQueue.pop();
          guard.releaseLock();
          clientSocket.write(pointer_cast<const char*>(buffer->getElements()), buffer->getByteSize());
          guard.exclusiveLock();
          recordingQueue.push(buffer);
          recordingSemaphore.post();
          guard.releaseLock();
        }
      }
      ++playerCount;
    } else {
      ++playerCount;
      SoundOutputStream soundOutputStream(sampleRate, channels);
      while (!Thread::getThread()->isTerminated()) {
        playingSemaphore.wait();
        if (Thread::getThread()->isTerminated()) {
          break;
        }
        if (!playingQueue.isEmpty()) {
          guard.exclusiveLock();
          Allocator<short>* buffer = playingQueue.pop();
          guard.releaseLock();
          soundOutputStream.write(buffer->getElements(), buffer->getByteSize());
          guard.exclusiveLock();
          recordingQueue.push(buffer);
          recordingSemaphore.post();
          guard.releaseLock();
        }
      }
      ++playerCount;
    }
  }

  bool hostAllowed(const InetAddress& host) throw() {
    // check address 172.30.* (mask then check if equal)
    return true;
  }

  void server() throw() {
    fout << MESSAGE("Initializing server socket: ") << endPoint << ENDL;
    ServerSocket serverSocket(endPoint.getAddress(), endPoint.getPort(), 1);

    fout << MESSAGE("Waiting for client...") << ENDL;
    clientSocket = serverSocket.accept();
    fout << MESSAGE("Connection from: ") << InetEndPoint(clientSocket.getAddress(), clientSocket.getPort()) << ENDL;
    assert(hostAllowed(clientSocket.getAddress()), OutOfDomain());
  }

  void client() throw() {
    fout << MESSAGE("Connecting to server: ") << endPoint << ENDL;
    clientSocket.connect(endPoint.getAddress(), endPoint.getPort());
    fout << MESSAGE("Connected to: ") << InetEndPoint(clientSocket.getAddress(), clientSocket.getPort())<< ENDL;
  }
  
  void run() throw() {
    fout << MESSAGE("Allocating buffers...") << ENDL;
    for (unsigned int i = 0; i < 16; ++i) {
      recordingQueue.push(new Allocator<short>(((sampleRate/16+4095)/4096)*4096));
      recordingSemaphore.post();
    }

    fout << MESSAGE("Creating threads...") << ENDL;
    Thread recorderThread(&recorder);
    Thread playerThread(&player);
    
    if (isServer) {
      server();
    } else {
      client();
    }

    fout << MESSAGE("Starting threads...") << ENDL;
    recorderThread.start();
    playerThread.start();
    
    fout << MESSAGE("Waiting...") << ENDL;
    Timer timer;

    for (unsigned int i = 0; i < 30*(1000/500); ++i) {
      if (Application::getApplication()->isTerminated()) {
        break;
      }
      Thread::millisleep(maximum<int>((i+1)*500 - timer.getLiveMicroseconds()/1000, 0));
    }

    if (!Application::getApplication()->isTerminated()) {
      fout << MESSAGE("Voluntary termination") << ENDL;
      Application::getApplication()->terminate();
    }
    
    /*
      fout << MESSAGE("Time: ") << setPrecision(3) << timer.getLiveMicroseconds()/1000000. << EOL
           << FLUSH;
    */


    fout << MESSAGE("Releasing buffers...") << ENDL;
    // clean-up buffers
    while (!recordingQueue.isEmpty()) {
      delete recordingQueue.pop();
    }
    while (!playingQueue.isEmpty()) {
      delete playingQueue.pop();
    }

    fout << MESSAGE("Completed") << ENDL;
  }
  
  ~IntercomServlet() throw() {}
};

class IntercomApplication : public Application {
private:

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
    isServer = true;
    port = DEFAULT_PORT;
    channels = DEFAULT_CHANNELS;
    samplingRate = DEFAULT_SAMPLE_RATE;
  }

  void handleArguments() throw(OutOfDomain) {
    bool portSpecified = false;
    bool hostSpecified = false;
    bool stereoSpecified = false;
    bool rateSpecified = false;
    
    Array<String>::ReadEnumerator enu = Application::getApplication()->getArguments().getReadEnumerator();

    // connect to host:port or server using port
    
    while (enu.hasNext()) {
      const String* argument = enu.next();
      if (*argument == "--help") {
      } else if (*argument == "--stereo") {
        assert(!stereoSpecified, OutOfDomain());
        stereoSpecified = true;
        channels = 2;
      } else if (*argument == "--host") {
        assert(!hostSpecified && !portSpecified, OutOfDomain("Already specified"));
        assert(enu.hasNext(), OutOfDomain("Host value missing"));
        host = *enu.next();
        hostSpecified = true;
        isServer = false;
      } else if (*argument == "--port") {
        assert(!portSpecified && !hostSpecified, OutOfDomain());
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
  
  int main() throw(OutOfDomain) {
    fout << Application::getFormalName() << MESSAGE(" Version 1.0") << EOL
         << MESSAGE("Copyright (c) 2002 by by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << EOL << FLUSH;

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
    IntercomServlet(channels, samplingRate, isServer, endPoint).run();
    
    return 0;
  }
  
};

int main(int argc, const char* argv[], const char* envp[]) {
  IntercomApplication application(argc, argv, envp);
  try {
    return application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::EXIT_CODE_NORMAL;
}
