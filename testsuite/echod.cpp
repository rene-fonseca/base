/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/UnsignedInteger.h>
#include <base/collection/List.h>
#include <base/collection/Queue.h>
#include <base/concurrency/Semaphore.h>
#include <base/concurrency/Thread.h>
#include <base/net/InetEndPoint.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/EndOfFile.h>

using namespace dk::sdu::mip::base;

Queue<StreamSocket*> jobs;
Semaphore semaphore;

class EchoThread : public Runnable {
private:

  static const unsigned int BUFFER_SIZE = 4096;
  Allocator<uint8> buffer;
public:
  
  EchoThread() throw()
    : buffer(BUFFER_SIZE) {
  }
  
  void run() throw() {
    Thread* thread = Thread::getThread();
    while (!thread->isTerminated()) {
      semaphore.wait(); // wait for job
      
      StreamSocket* job = jobs.pop();
      if (!job) { // should we terminate
        break;
      }
      StreamSocket socket = *job; // dereference for convenience
      
      try {
        bool ended = false;
        while (!thread->isTerminated() && !ended) {
          bool event = socket.wait(250000);
          if (!event) {
            continue;
          }
          
          while (true) {
            unsigned int bytesToRead = buffer.getSize();
            unsigned int bytesRead = socket.read(buffer.getElements(), bytesToRead, true);
            if (bytesRead == 0) {
              ended = true;
              break;
            }
            socket.write(buffer.getElements(), bytesRead);
            if (bytesRead < buffer.getSize()) {
              break;
            }
          }
        }
        socket.shutdownOutputStream();
        socket.close();
      } catch (IOException&) {
      }
    }
  }

  virtual ~EchoThread() throw() {
  }
};

class ContextBinder : public Object {
private:
  
  EchoThread runnable; // must be initialized before context
  Thread context;
public:
  
  ContextBinder() throw()
    : runnable(),
      context(&runnable) {
  }
  
  void start() {
    context.start();
  }
  
  ~ContextBinder() throw() {
    context.terminate(); // ask context to terminate
    context.join(); // wait for context to complete
  }
};

class EchoServiceApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  /** The echo service port. */
  static const unsigned short ECHO_SERVICE_PORT = 7;
  static const unsigned int CONCURRENCY = 16;
  /** The server port. */
  unsigned short port;
  
  enum Command {
    HELP,
    ECHO_SERVICE
  };
public:
  
  EchoServiceApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application("echod", numberOfArguments, arguments, environment) {
  }
  
  void help() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    fout << getFormalName() << " [--help] [--port PORT]" << ENDL;
  }
  
  bool accept(const InetEndPoint& endPoint) throw() {
    // fout << "Incoming connection: " << endPoint << ENDL;
    return true;
  }
  
  void echod() throw() {
    List<ContextBinder*> threadPool;
    for (unsigned int i = 0; i < CONCURRENCY; ++i) {
      ContextBinder* temp = new ContextBinder();
      threadPool.add(temp);
      temp->start();
    }
    
    try {
      InetAddress address;
      ServerSocket serverSocket(address, port, 1);
      
      Thread* thread = Thread::getThread();
      while (!thread->isTerminated()) {
        if (serverSocket.wait(250)) {
          StreamSocket clientSocket = serverSocket.accept(100);
          if (clientSocket.isValid()) {
            // TAG: add thread to pool if required?
            InetEndPoint endPoint(
              clientSocket.getAddress(),
              clientSocket.getPort()
            );
            if (accept(endPoint)) {
              jobs.push(new StreamSocket(clientSocket));
              semaphore.post(); // notify one thread
            } else {
              clientSocket.shutdownOutputStream();
              clientSocket.close();
            }
          }
        }
      }
    } catch (IOException& e) {
      ferr << "Error: " << e.getMessage() << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
    
    for (unsigned int i = 0; i < threadPool.getSize(); ++i) {
      jobs.push(0); // queue dummy job to terminate context
      semaphore.post(); // notify
    }
    List<ContextBinder*>::Enumerator enu = threadPool.getEnumerator();
    while (enu.hasNext()) {
      delete *enu.next();
    }
  }
  
  void main() throw() {
    String service;
    Command command = ECHO_SERVICE;
    
    const Array<String> arguments = getArguments();
    if (arguments.getSize() > 0) {
      Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
      while (enu.hasNext()) {
        String argument = *enu.next();
        if (argument == "--help") {
          command = HELP;
        } else if (argument == "--port") {
          String temp = *enu.next();
          try {
            UnsignedInteger value(temp);
            if (value > 0xffff) {
              ferr << "Error: " << "Invalid port" << ENDL;
              setExitCode(EXIT_CODE_ERROR);
              return;
            }
            port = value;
          } catch (InvalidFormat&) {
            try {
              InetService service(temp);
              port = service.getPort();
            } catch (ServiceNotFound& e) {
              ferr << "Error: " << e.getMessage() << ENDL;
              setExitCode(EXIT_CODE_ERROR);
              return;
            }
          }
        } else {
          ferr << "Error: " << "Invalid argument" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
      }
    }
    
    switch (command) {
    case HELP:
      help();
      break;
    case ECHO_SERVICE:
      echod();
      break;
    }
  }

  ~EchoServiceApplication() throw() {
  }
};

STUB(EchoServiceApplication);
