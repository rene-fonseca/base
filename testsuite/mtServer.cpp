/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Integer.h>
#include <base/collection/List.h>
#include <base/collection/Queue.h>
#include <base/concurrency/Semaphore.h>
#include <base/concurrency/Thread.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>

using namespace base;

Queue<StreamSocket*> jobs;
Semaphore semaphore;



class CommunicationThread : public Runnable {
private:

  unsigned int id;
public:

  CommunicationThread(unsigned int _id) throw() : id(_id) {
  }

  void run() throw(IOException) {
    fout << id << MESSAGE(": ") << MESSAGE("Thread is up and running") << ENDL;

    while (true) {
      semaphore.wait(); // wait for job

      StreamSocket* job = jobs.pop();
      if (!job) { // should we terminate
        break;
      }
      StreamSocket socket = *job; // dereference for convenience

      fout << id << ": " << MESSAGE("Communicating with...") << ENDL;
      fout << id << ": "<< MESSAGE("  address=") << socket.getAddress() << MESSAGE(" port=") << socket.getPort() << ENDL;

      {
        FormatOutputStream outstream(socket); // must be destroyed before socket is closed
        FormatInputStream instream(socket);

        fout << id << ": " << MESSAGE("Waiting for request") << FLUSH;
        while (!instream.wait(1000000)) {
          fout << '.' << FLUSH;
        }
        fout << ENDL;

        fout << id << ": " << MESSAGE("Processing request") << ENDL;
        fout << id << ": " << ">: ";
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }

        fout << id << ": " << MESSAGE("Sending acknowledge") << ENDL;
        outstream << MESSAGE("Hi, I'm a multithreaded server and this is my response. ")
                  << MESSAGE("This connection has been assigned to context ") << id << '.' << ENDL;

        fout << id << ": " << MESSAGE("Waiting for termination request") << FLUSH;
        while (!instream.wait(1000000)) {
          fout << '.' << FLUSH;
        }
        fout << ENDL;

        fout << id << ": " << MESSAGE("Processing terminating request") << ENDL;
        fout << id << ": " << ">: ";
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }
      }

      fout << id << ": " << MESSAGE("Closing connection...") << ENDL;
      socket.close();
    }

    fout << id << ": " << MESSAGE("Thread has been terminated") << ENDL;
  }
};



class ContextBinder : public Object {
private:

  CommunicationThread runnable; // must be initialized before context
  Thread context;
public:

  ContextBinder(unsigned int id) throw()
    : runnable(id), context(&runnable) {
  }

  void start() {
    context.start();
  }

  ~ContextBinder() throw() {
    context.terminate(); // ask context to terminate
    context.join(); // wait for context to complete
  }
};



class MTServerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  MTServerApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application(MESSAGE("mtServer"), argc, argv, env) {
  }
  
  void server(String desiredAddress, String desiredService) {
    fout << MESSAGE("Hostname: ") << InetAddress::getLocalHost() << ENDL;

    {
      List<InetInterface> interfaces = InetInterface::getInetInterfaces();
      List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
      fout << MESSAGE("Available interfaces:") << ENDL;
      while (enu.hasNext()) {
        const InetInterface* i = enu.next();
        fout << MESSAGE("  interface: index=") << i->getIndex() << MESSAGE(" name=") << i->getName() << ENDL;
      }
    }

    InetAddress address; // the address to bind the server socket to
    if (desiredAddress == "") { // should we find an address
      fout << MESSAGE("Local addresses:") << ENDL;
      List<InetAddress> addresses = InetAddress::getAddressesByName(InetAddress::getLocalHost());
      List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
      unsigned int index = 0;
      while (enu.hasNext()) {
        const InetAddress* temp = enu.next();
        if (index == 0) { // use the first address
          address = *temp;
          fout << MESSAGE("  address ") << index++ << ": " << *temp << MESSAGE(" (USING THIS)") << ENDL;
        } else {
          fout << MESSAGE("  address ") << index++ << ": " << *temp << ENDL;
        }
      }
    } else {
      address = InetAddress(desiredAddress);
    }

    unsigned short port; // the port to bind the server socket to
    try {
      Integer integer(desiredService);
      if ((integer < 0) || (integer > 0xffff)) {
        throw OutOfRange("Port is out of range");
      }
      port = integer;
    } catch (InvalidFormat& e) {
      try {
        InetService service(desiredService);
        port = service.getPort();
        fout << MESSAGE("Service: name=") << service.getName()
             << MESSAGE("  port=") << service.getPort()
             << MESSAGE("  protocol=") << service.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << MESSAGE("Warning: ") << e.getMessage() << ENDL;
        fout << MESSAGE("Service: port=") << port << ENDL;
      }
    }

    fout << MESSAGE("Initializing thread pool") << ENDL;
    List<ContextBinder*> threadPool;
    for (unsigned int i = 0; i < 4; ++i) {
      ContextBinder* temp = new ContextBinder(i);
      threadPool.add(temp);
      temp->start();
    }

    fout << MESSAGE("Initializing server socket...") << ENDL;
    ServerSocket serverSocket(address, port, 1);

    fout << MESSAGE("Server address...") << ENDL;
    fout << MESSAGE("  address=") << serverSocket.getLocalAddress() << MESSAGE(" port=") << serverSocket.getLocalPort() << ENDL;

    unsigned int count = 10; // total number of connections to accept
    while (count--) {
      fout << MESSAGE("Waiting for connection...") << ENDL;
      StreamSocket socket(serverSocket.accept());
      fout << MESSAGE("Connection established with: address=") << socket.getAddress() << MESSAGE(" port=") << socket.getPort() << ENDL;

      jobs.push(new StreamSocket(socket)); // add job to queue
      semaphore.post(); // notify one thread
    }

    fout << MESSAGE("Closing server socket...") << ENDL;
    serverSocket.close();

    fout << MESSAGE("Releasing thread pool") << ENDL;
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
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    String address; // default address
    String service = "1234"; // default service

    const Array<String> arguments = getArguments();
  
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      address = arguments[1]; // the address
      break;
    case 2:
      address = arguments[1]; // the address
      service = arguments[2]; // the service
      break;
    default:
      fout << MESSAGE("mtServer [address] [service]") << ENDL;
      return;
    }

    server(address, service);
  }
};

STUB(MTServerApplication);
