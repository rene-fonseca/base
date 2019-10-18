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
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Integer.h>
#include <base/net/StreamSocket.h>
#include <base/net/InetEndPoint.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/concurrency/Thread.h>

using namespace com::azure::dev::base;

class DatagramClientApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DatagramClientApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(
        "datagramClient",
        numberOfArguments,
        arguments,
        environment
    ) {
  }

  void client(String host, String service) {
    fout << "Server: " << host << ENDL;

    InetAddress address; // the address of the remote host
    {
      fout << "Server addresses:" << ENDL;
      List<InetAddress> addresses = InetAddress::getAddressesByName(host);
      List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
      unsigned int index = 0;
      while (enu.hasNext()) {
        const InetAddress* temp = enu.next();
        if (index == 0) { // use the first address
          address = *temp;
          fout << indent(2) << "address " << index++ << ": " << *temp << " (USING THIS)" << ENDL;
        } else {
          fout << indent(2) << "address " << index++ << ": " << *temp << ENDL;
        }
      }
    }

    unsigned short port; // the port to bind the server socket to
    try {
      Integer integer(service);
      if ((integer < 0) || (integer > 0xffff)) {
        throw OutOfRange("Port is out of range");
      }
      port = integer;
    } catch (InvalidFormat&) {
      try {
        InetService s(service);
        port = s.getPort();
        fout << "Service: name=" << s.getName()
             << indent(2) << "port=" << s.getPort()
             << indent(2) << "protocol=" << s.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << "Warning: " << e.getMessage() << ENDL;
        fout << "Service: port=" << port << ENDL;
      }
    }

    fout << "Initializing socket..." << ENDL;
    Socket socket;

    fout << "Creating datagram socket..." << ENDL;
    socket.create(Socket::DATAGRAM);

    fout << "Requesting permission to send broadcast messages..." << ENDL;
    socket.setBroadcast(true);

    fout << "Sending datagram..." << ENDL;
    static const Literal sendBuffer = "DATAGRAM FROM CLIENT";
    unsigned int bytesSent = socket.sendTo(
      Cast::pointer<const uint8*>(sendBuffer.getValue()),
      sendBuffer.getLength(),
      address,
      port
    );

    Allocator<uint8> receiveBuffer(4096);
    InetAddress remoteAddress;
    unsigned short remotePort;

    fout << "Waiting for datagram..." << ENDL;
    unsigned int bytesReceived = socket.receiveFrom(
      receiveBuffer.getElements(),
      receiveBuffer.getSize(),
      remoteAddress,
      remotePort
    );
    fout << "Datagram of " << bytesReceived << " bytes received from "
         << InetEndPoint(remoteAddress, remotePort)
         << ENDL;
    fout << ">: "
         << String(
              Cast::pointer<const char*>(receiveBuffer.getElements()),
              bytesReceived
            ) << ENDL;
    
    fout << "Closing socket..." << ENDL;
    socket.close();
  }

  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    String host = InetAddress::getLocalHost(); // default host
    String service = "1234"; // default service

    const Array<String> arguments = getArguments();
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      host = arguments[1]; // the address
      break;
    case 2:
      host = arguments[1]; // the address
      service = arguments[2]; // the service
      break;
    default:
      fout << "datagramClient [host] [service]" << ENDL;
      return;
    }

    client(host, service);
  }
};

STUB(DatagramClientApplication);
