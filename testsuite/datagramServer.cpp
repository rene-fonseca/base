/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Integer.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>

using namespace base;

class DatagramServerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DatagramServerApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("datagramServer"), numberOfArguments, arguments, environment) {
  }

  void server(const String& servicename) throw() {
    fout << "Hostname: " << InetAddress::getLocalHost() << ENDL;

    InetAddress address("0.0.0.0");

    unsigned short port; // the port to bind the server socket to
    try {
      Integer integer(servicename);
      if ((integer < 0) || (integer > 0xffff)) {
        throw OutOfRange("Port is out of range");
      }
      port = integer;
    } catch (InvalidFormat& e) {
      try {
        InetService service(servicename);
        port = service.getPort();
        fout << "Service: name=" << service.getName()
             << "  port=" << service.getPort()
             << "  protocol=" << service.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << "Warning: " << e.getMessage() << ENDL;
        fout << "Service: port=" << port << ENDL;
      }
    }

    fout << "Initializing server socket..." << ENDL;
    Socket serverSocket;

    fout << "Creating datagram socket..." << ENDL;
    serverSocket.create(Socket::DATAGRAM);

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

    fout << MESSAGE("Closing server socket...") << ENDL;
    serverSocket.close();
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    String service = "1234"; // default service

    const Array<String> arguments = getArguments();  
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      service = arguments[1]; // the service
      break;
    default:
      fout << "datagramServer [service]" << ENDL;
      return; // stop
    }
    server(service);
  }
};

STUB(DatagramServerApplication);
