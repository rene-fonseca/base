/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

void server(String servicename) {
  fout << "Hostname: " << InetAddress::getLocalHost() << ENDL;

  InetAddress address("0.0.0.0");

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
  Socket serverSocket;

  fout << "Creating datagram socket..." << ENDL;
  serverSocket.create(false);

  fout << "Binding to address..." << ENDL;
  serverSocket.bind(address, port);

  fout << "Server address..." << ENDL;
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << ENDL;

  fout << "Requesting permission to send/receive broadcast messages..." << ENDL;
  serverSocket.setBroadcast(true);

  unsigned int datagrams = 10; // the number of connections to accept

  while (datagrams--) {
    char buffer[4096];
    InetAddress remoteAddress;
    unsigned short remotePort;

    fout << "Waiting for datagram..." << ENDL;
    unsigned int bytesReceived = serverSocket.receiveFrom(buffer, sizeof(buffer), remoteAddress, remotePort);

    fout << "Datagram of " << bytesReceived << " bytes received from " << remoteAddress
         << " on port " << remotePort << ENDL;
    fout << ">: " << buffer << ENDL;

    fout << "Sending datagram back to client..." << ENDL;
    char response[] = "DATAGRAM FROM SERVER";
    unsigned int bytesSent = serverSocket.sendTo(response, sizeof(response), remoteAddress, remotePort);
    fout << "bytesSent: " << bytesSent << ENDL;
  }

  fout << "Closing server socket..." << ENDL;
  serverSocket.close();
}

int main(int argc, char* argv[]) {
  fout << "Testing datagram socket server..." << ENDL;

  String service = "1234"; // default service

  switch (argc) {
  case 1:
    // use defaults
    break;
  case 2:
    service = argv[1]; // the service
    break;
  default:
    fout << "datagramServer [service]" << ENDL;
    return 0; // stop
  }

  try {
    server(service);
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
