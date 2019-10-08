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
  
  MulticastServerApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(
      "multicastServer",
      numberOfArguments,
      arguments,
      environment) {
  }

  void server(const String& servicename) throw() {
    fout << "Hostname: " << InetAddress::getLocalHost() << ENDL;

    unsigned short port; // the port to bind the server socket to
    try {
      Integer integer(servicename);
      if ((integer < 0) || (integer > 0xffff)) {
        throw OutOfRange("Port is out of range");
      }
      port = integer;
    } catch (InvalidFormat&) {
      try {
        InetService service(servicename);
        port = service.getPort();
        fout << "Service: name=" << service.getName()
             << indent(2) << "port=" << service.getPort()
             << indent(2) << "protocol=" << service.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << "Warning: " << e.getMessage() << ENDL;
        fout << "Service: port=" << port << ENDL;
      }
    }

    if (!group.isMulticast()) {
      ferr << "Error: "
           << "Address is not a multicast address." << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    fout << "Initializing server socket..." << ENDL;
    Socket socket;

    fout << "Creating datagram socket..." << ENDL;
    Socket::Domain domain = group.isIPv4() ? Socket::IPV4 : Socket::IPV6;
    socket.create(Socket::DATAGRAM, domain);

    fout << "Enabling reuse..." << ENDL;
    socket.setReuseAddress(true);
    
    InetAddress address;
    
    fout << "Binding to address..." << ENDL;
    socket.bind(address, port);
    
    fout << "Default multicast interface for outgoing packets: "
         << socket.getMulticastInterface() << ENDL;
    
    fout << "Joining multicast group " << group
         << " on interface " << interface << ENDL;
    socket.joinGroup(interface, group);

    fout << "Disabling multicast loopback..." << ENDL;
    socket.setMulticastLoopback(false);

    fout << "Setting multicast time to live (TTL)..." << ENDL;
    socket.setMulticastHops(1);

    fout << "Multicast settings: " << EOL
         << indent(2) << "Default interface: " << socket.getMulticastInterface() << EOL
         << indent(2) << "Hops: " << socket.getMulticastHops() << EOL
         << indent(2) << "Loopback: " << socket.getMulticastLoopback() << EOL
         << ENDL;

    fout << "Server address..." << ENDL;
    fout << indent(2)
         << InetEndPoint(socket.getLocalAddress(), socket.getLocalPort())
         << ENDL;
    
    fout << "Sending here am I packet..." << ENDL;
    static const Literal message = "HERE AM I";
    socket.sendTo(
      Cast::pointer<const uint8*>(message.getValue()),
      message.getLength(),
      group,
      port
    );
    
    Allocator<uint8> buffer(4096);
    while (!isTerminated()) {
      InetAddress remoteAddress;
      unsigned short remotePort;
      
      fout << "Waiting for packet..." << ENDL;
      unsigned int bytesReceived = socket.receiveFrom(
        buffer.getElements(),
        buffer.getSize(),
        remoteAddress,
        remotePort
      );
      
      fout << "Packet of " << bytesReceived
           << " bytes received from " << remoteAddress
           << " on port " << remotePort << ENDL;
      
      fout << ">: " << String(
        Cast::pointer<const char*>(buffer.getElements()),
        bytesReceived
      ) << ENDL;
      
      fout << "Sending packet back to client..." << ENDL;
      static const Literal response = "MULTICAST PACKET FROM SERVER";
      unsigned int bytesSent = socket.sendTo(
        Cast::pointer<const uint8*>(response.getValue()),
        response.getLength(),
        remoteAddress,
        remotePort
      );
      fout << "bytesSent: " << bytesSent << ENDL;
    }

    fout << "Leaving multicast group " << group
         << " on interface " << interface << ENDL;
    socket.leaveGroup(interface, group);    
  }

  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
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
      fout << getFormalName() << " group [service]" << ENDL;
      return; // stop
    }
    server(service);
  }
};

STUB(MulticastServerApplication);
