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
#include <base/Integer.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/net/ServerSocket.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>

using namespace dk::sdu::mip::base;

class MulticastServerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  InetAddress interface;
  InetAddress group;
public:
  
  MulticastServerApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("multicastServer"), numberOfArguments, arguments, environment) {
  }

  void server(const String& servicename) throw() {
    fout << MESSAGE("Hostname: ") << InetAddress::getLocalHost() << ENDL;

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
        fout << MESSAGE("Service: name=") << service.getName()
             << indent(2) << MESSAGE("port=") << service.getPort()
             << indent(2) << MESSAGE("protocol=") << service.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << MESSAGE("Warning: ") << e.getMessage() << ENDL;
        fout << MESSAGE("Service: port=") << port << ENDL;
      }
    }

    if (!group.isMulticast()) {
      ferr << MESSAGE("Error: ")
           << MESSAGE("Address is not a multicast address.") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    fout << MESSAGE("Initializing server socket...") << ENDL;
    Socket socket;

    fout << MESSAGE("Creating datagram socket...") << ENDL;
    Socket::Domain domain = group.isIPv4() ? Socket::IPV4 : Socket::IPV6;
    socket.create(Socket::DATAGRAM, domain);

    fout << MESSAGE("Enabling reuse...") << ENDL;
    socket.setReuseAddress(true);
    
    InetAddress address;
    
    fout << MESSAGE("Binding to address...") << ENDL;
    socket.bind(address, port);
    
    fout << MESSAGE("Default multicast interface for outgoing packets: ")
         << socket.getMulticastInterface() << ENDL;
    
    fout << MESSAGE("Joining multicast group ") << group
         << MESSAGE(" on interface ") << interface << ENDL;
    socket.joinGroup(interface, group);

    fout << MESSAGE("Disabling multicast loopback...") << ENDL;
    socket.setMulticastLoopback(false);

    fout << MESSAGE("Setting multicast time to live (TTL)...") << ENDL;
    socket.setMulticastHops(1);

    fout << MESSAGE("Multicast settings: ") << EOL
         << indent(2) << MESSAGE("Default interface: ") << socket.getMulticastInterface() << EOL
         << indent(2) << MESSAGE("Hops: ") << socket.getMulticastHops() << EOL
         << indent(2) << MESSAGE("Loopback: ") << socket.getMulticastLoopback() << EOL
         << ENDL;

    fout << MESSAGE("Server address...") << ENDL;
    fout << indent(2)
         << InetEndPoint(socket.getLocalAddress(), socket.getLocalPort())
         << ENDL;
    
    fout << MESSAGE("Sending here am I packet...") << ENDL;
    char message[] = "HERE AM I";
    socket.sendTo(message, sizeof(message), group, port);
    
    while (!isTerminated()) {
      char buffer[4096];
      InetAddress remoteAddress;
      unsigned short remotePort;
      
      fout << MESSAGE("Waiting for packet...") << ENDL;
      unsigned int bytesReceived = socket.receiveFrom(
        buffer,
        sizeof(buffer),
        remoteAddress,
        remotePort
      );
      
      fout << MESSAGE("Packet of ") << bytesReceived
           << MESSAGE(" bytes received from ") << remoteAddress
           << MESSAGE(" on port ") << remotePort << ENDL;
      
      fout << MESSAGE(">: ") << buffer << ENDL;
      
      fout << MESSAGE("Sending packet back to client...") << ENDL;
      char response[] = "MULTICAST PACKET FROM SERVER";
      unsigned int bytesSent = socket.sendTo(
        response,
        sizeof(response),
        remoteAddress,
        remotePort
      );
      fout << MESSAGE("bytesSent: ") << bytesSent << ENDL;
    }

    fout << MESSAGE("Leaving multicast group ") << group
         << MESSAGE(" on interface ") << interface << ENDL;
    socket.leaveGroup(interface, group);    
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    String service = "1234"; // default service

    const Array<String> arguments = getArguments();  
    switch (arguments.getSize()) {
    case 1:
      group = InetAddress(arguments[0]);
      break;
    case 2:
      group = InetAddress(arguments[0]);
      service = arguments[1];
      break;
    default:
      fout << getFormalName() << MESSAGE(" group [service]") << ENDL;
      return; // stop
    }
    server(service);
  }
};

STUB(MulticastServerApplication);
