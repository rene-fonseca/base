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
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/Integer.h>
#include <base/TypeInfo.h>

using namespace base;

void server(String a, String servicename) {

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
  if (a == "") { // should we find an address
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
    address = InetAddress(a);
  }

  unsigned short port; // the port to bind the server socket to
  try {
    Integer integer(servicename);
    if ((integer < 0) || (integer > 0xffff)) {
      throw OutOfRange("Port is out of range");
    }
    port = integer;
  } catch(InvalidFormat& e) {
    try {
      InetService service(servicename);
      port = service.getPort();
      fout << "Service: name=" << service.getName()
           << "  port=" << service.getPort()
           << "  protocol=" << service.getProtocol() << ENDL;
    } catch(ServiceNotFound& e) {
      fout << "Warning: " << e.getMessage() << ENDL;
      fout << "Service: port=" << port << ENDL;
    }
  }

  fout << "Initializing server socket..." << ENDL;
  ServerSocket serverSocket(address, port, 1);

  fout << "Server address..." << ENDL;
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << ENDL;

  fout << "Waiting for client to connect..." << ENDL;

  unsigned int connection = 1; // the number of connections so far
  bool terminate = false; // specifies whether the server should be terminated

  while (!terminate) {
    fout << "Listening for connections..." << ENDL;
    StreamSocket socket(serverSocket.accept());

    fout << "Connection number " << connection++ << " established from" << ENDL;
    fout << "  remote: address=" << socket.getAddress() << " port=" << socket.getPort() << ENDL;

    fout << "Talking with client..." << ENDL;
    {
      FormatOutputStream outstream(socket); // must be destroyed before socket is closed
      FormatInputStream instream(socket);

      fout << "Waiting for request" << FLUSH;
      while (!instream.wait(1000000)) {
        fout << "." << FLUSH;
      }
      fout << ENDL;

      fout << "Processing request" << ENDL;
      fout << ">: ";
      while (instream.available()) {
        char ch;
        instream >> ch;
        fout << ch;
      }

      fout << "Sending acknowledge" << ENDL;
      outstream << "Hi, I'm the server and this is my response" << ENDL;

      fout << "Waiting for termination request" << FLUSH;
      while (!instream.wait(1000000)) {
        fout << "." << FLUSH;
      }
      fout << ENDL;

      fout << "Processing terminating request" << ENDL;
      fout << ">: ";
      while (instream.available()) {
        char ch;
        instream >> ch;
        fout << ch;
      }
    }

    fout << "Closing connection..." << ENDL;
    socket.close();

    if (connection == 5) {
      terminate = true;
    }
  }

  fout << "Closing server socket..." << ENDL;
  serverSocket.close();
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
    fout << "server [address] [service]" << ENDL;
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
