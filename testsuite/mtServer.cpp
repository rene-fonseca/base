/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/Integer.h>
#include <base/collection/List.h>
#include <base/collection/Queue.h>
#include <base/concurrency/Semaphore.h>
#include <base/TypeInfo.h>

using namespace base;

Queue<StreamSocket*> jobs;
Semaphore semaphore;



class CommunicationThread : public Runnable {
private:

  unsigned int id;
public:

  CommunicationThread(unsigned int i) throw() : id(i) {}

  void run() throw(IOException) {
    fout << id << ": " << "Thread is up and running" << ENDL;

    while (true) {
      semaphore.wait(); // wait for job

      StreamSocket* job = jobs.pop();
      if (!job) { // should we terminate
        break;
      }
      StreamSocket socket = *job; // dereference for convenience

      fout << id << ": " << "Communicating with..." << ENDL;
      fout << id << ": "<< "  address=" << socket.getAddress() << " port=" << socket.getPort() << ENDL;

      {
        FormatOutputStream outstream(socket); // must be destroyed before socket is closed
        FormatInputStream instream(socket);

        fout << id << ": " << "Waiting for request" << FLUSH;
        while (!instream.wait(1000000)) {
          fout << "." << FLUSH;
        }
        fout << ENDL;

        fout << id << ": " << "Processing request" << ENDL;
        fout << id << ": " << ">: ";
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }

        fout << id << ": " << "Sending acknowledge" << ENDL;
        outstream << "Hi, I'm a multithreaded server and this is my response. "
                  << "This connection has been assigned to context " << id << "." << ENDL;

        fout << id << ": " << "Waiting for termination request" << FLUSH;
        while (!instream.wait(1000000)) {
          fout << "." << FLUSH;
        }
        fout << ENDL;

        fout << id << ": " << "Processing terminating request" << ENDL;
        fout << id << ": " << ">: ";
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }
      }

      fout << id << ": " << "Closing connection..." << ENDL;
      socket.close();
    }

    fout << id << ": " << "Thread has been terminated" << ENDL;
  }
};



class ContextBinder : public Object {
private:

  CommunicationThread runnable; // must be initialized before context
  Thread context;
public:

  ContextBinder(unsigned int id) throw() : runnable(id), context(&runnable) {
  }

  void start() {
    context.start();
  }

  ~ContextBinder() throw() {
    context.terminate(); // ask context to terminate
    context.join(); // wait for context to complete
  }
};



void server(String desiredAddress, String desiredService) {

  fout << "Hostname: " << InetAddress::getLocalHost() << ENDL;

  {
    List<InetInterface> interfaces = InetInterface::getInetInterfaces();
    List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
    fout << "Available interfaces:" << ENDL;
    while (enu.hasNext()) {
      const InetInterface* i = enu.next();
      fout << "  interface: index=" << i->getIndex() << " name=" << i->getName() << ENDL;
    }
  }

  InetAddress address; // the address to bind the server socket to
  if (desiredAddress == "") { // should we find an address
    fout << "Local addresses:" << ENDL;
    List<InetAddress> addresses = InetAddress::getAddressesByName(InetAddress::getLocalHost());
    List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
    unsigned int index = 0;
    while (enu.hasNext()) {
      const InetAddress* temp = enu.next();
      if (index == 0) { // use the first address
        address = *temp;
        fout << "  address " << index++ << ": " << *temp << " (USING THIS)" << ENDL;
      } else {
        fout << "  address " << index++ << ": " << *temp << ENDL;
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
  } catch(InvalidFormat& e) {
    try {
      InetService service(desiredService);
      port = service.getPort();
      fout << "Service: name=" << service.getName()
           << "  port=" << service.getPort()
           << "  protocol=" << service.getProtocol() << ENDL;
    } catch(ServiceNotFound& e) {
      fout << "Warning: " << e.getMessage() << ENDL;
      fout << "Service: port=" << port << ENDL;
    }
  }

  fout << "Initializing thread pool" << ENDL;
  List<ContextBinder*> threadPool;
  for (unsigned int i = 0; i < 4; ++i) {
    ContextBinder* temp = new ContextBinder(i);
    threadPool.add(temp);
    temp->start();
  }

  fout << "Initializing server socket..." << ENDL;
  ServerSocket serverSocket(address, port, 1);

  fout << "Server address..." << ENDL;
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << ENDL;

  unsigned int count = 10; // total number of connections to accept
  while (count--) {
    fout << "Waiting for connection..." << ENDL;
    StreamSocket socket(serverSocket.accept());
    fout << "Connection established with: address=" << socket.getAddress() << " port=" << socket.getPort() << ENDL;

    jobs.push(new StreamSocket(socket)); // add job to queue
    semaphore.post(); // notify one thread
  }

  fout << "Closing server socket..." << ENDL;
  serverSocket.close();

  fout << "Releasing thread pool" << ENDL;
  for (unsigned int i = 0; i < threadPool.getSize(); ++i) {
    jobs.push(0); // queue dummy job to terminate context
    semaphore.post(); // notify
  }
  List<ContextBinder*>::Enumerator enu = threadPool.getEnumerator();
  while (enu.hasNext()) {
    delete *enu.next();
  }
}

int main(int argc, char* argv[]) {
  fout << "Testing ServerSocket..." << ENDL;

  String address; // default address
  String service = "1234"; // default service

  switch (argc) {
  case 1:
    // use defaults
    break;
  case 2:
    address = argv[1]; // the address
    break;
  case 3:
    address = argv[1]; // the address
    service = argv[2]; // the service
    break;
  default:
    fout << "mtServer [address] [service]" << ENDL;
    return 0; // stop
  }

  try {
    server(address, service);
  } catch(Exception& e) {
    ferr << getTypename(e) << ": "<< e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  fout << "Completed" << ENDL;
  return 0;
}
