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
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/Integer.h>
#include <base/TypeInfo.h>

using namespace base;

class ServerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ServerApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("server"), numberOfArguments, arguments, environment) {
  }

  void server(String a, String servicename) {
    fout << MESSAGE("Hostname: ") << InetAddress::getLocalHost() << ENDL;

    {
      List<InetInterface> interfaces = InetInterface::getInterfaces();
      List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
      fout << MESSAGE("Available interfaces:") << ENDL;
      while (enu.hasNext()) {
        const InetInterface* i = enu.next();
        fout << MESSAGE("  interface: index=") << i->getIndex() << MESSAGE(" name=") << i->getName() << ENDL;
      }
    }
    
    InetAddress address; // the address to bind the server socket to
    if (a == "") { // should we find an address
      fout << MESSAGE("Local addresses:") << ENDL;
      List<InetAddress> addresses = InetAddress::getAddressesByName(InetAddress::getLocalHost());
      List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
      unsigned int index = 0;
      while (enu.hasNext()) {
        const InetAddress* temp = enu.next();
        if (index == 0) { // use the first address
          address = *temp;
          fout << MESSAGE("  address ") << index++ << MESSAGE(": ") << *temp << MESSAGE(" (USING THIS)") << ENDL;
        } else {
          fout << MESSAGE("  address ") << index++ << MESSAGE(": ") << *temp << ENDL;
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
    } catch (InvalidFormat& e) {
      try {
        InetService service(servicename);
        port = service.getPort();
        fout << MESSAGE("Service: name=") << service.getName()
             << MESSAGE("  port=") << service.getPort()
             << MESSAGE("  protocol=") << service.getProtocol() << ENDL;
      } catch (ServiceNotFound& e) {
        fout << MESSAGE("Error: ") << e.getMessage() << ENDL;
        return;
      }
    }

    fout << MESSAGE("Initializing server socket...") << ENDL;
    ServerSocket serverSocket(address, port, 1);

    fout << MESSAGE("Server address...") << ENDL;
    fout << MESSAGE("  address=") << serverSocket.getLocalAddress() << MESSAGE(" port=") << serverSocket.getLocalPort() << ENDL;

    fout << MESSAGE("Waiting for client to connect...") << ENDL;

    unsigned int connection = 1; // the number of connections so far
    bool terminate = false; // specifies whether the server should be terminated

    while (!terminate) {
      fout << MESSAGE("Listening for connections...") << ENDL;
      StreamSocket socket(serverSocket.accept());

      fout << MESSAGE("Connection number ") << connection++ << MESSAGE(" established from") << ENDL;
      fout << MESSAGE("  remote: address=") << socket.getAddress() << MESSAGE(" port=") << socket.getPort() << ENDL;

      fout << MESSAGE("Talking with client...") << ENDL;
      {
        FormatOutputStream outstream(socket); // must be destroyed before socket is closed
        FormatInputStream instream(socket);

        fout << MESSAGE("Waiting for request") << FLUSH;
        while (!instream.wait(1000000)) {
          fout << '.' << FLUSH;
        }
        fout << ENDL;

        fout << MESSAGE("Processing request") << ENDL;
        fout << MESSAGE(">: ");
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }

        fout << MESSAGE("Sending acknowledge") << ENDL;
        outstream << MESSAGE("Hi, I'm the server and this is my response") << ENDL;

        fout << MESSAGE("Waiting for termination request") << FLUSH;
        while (!instream.wait(1000000)) {
          fout << '.' << FLUSH;
        }
        fout << ENDL;

        fout << MESSAGE("Processing terminating request") << ENDL;
        fout << MESSAGE(">: ");
        while (instream.available()) {
          char ch;
          instream >> ch;
          fout << ch;
        }
      }

      fout << MESSAGE("Closing connection...") << ENDL;
      socket.close();

      if (connection == 5) {
        terminate = true;
      }
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

    Array<String> arguments = getArguments();

    String address; // default address
    String service = "1234"; // default service

    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      service = arguments[0]; // the service
      break;
    case 2:
      address = arguments[0]; // the address
      service = arguments[1]; // the service
      break;
    default:
      fout << MESSAGE("server [address] [service]") << ENDL;
      return;
    }
    server(address, service);
  }
};

STUB(ServerApplication);
