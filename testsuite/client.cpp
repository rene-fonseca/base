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
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Integer.h>
#include <base/net/StreamSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>

using namespace base;

class ClientApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ClientApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("client"), numberOfArguments, arguments, environment) {
  }

  void client(String host, String service) {
    fout << MESSAGE("Server: ") << host << ENDL;

    InetAddress address; // the address of the remote host
    {
      fout << MESSAGE("Server addresses:") << ENDL;
      List<InetAddress> addresses = InetAddress::getAddressesByName(host);
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
    }

    InetEndPoint endPoint(address, service);
    fout << MESSAGE("End point: address=") << endPoint.getAddress() << MESSAGE(" port=") << endPoint.getPort() << ENDL;

    fout << MESSAGE("Initializing socket...") << ENDL;
    StreamSocket socket;

    fout << MESSAGE("Connecting socket...") << ENDL;
    socket.connect(endPoint.getAddress(), endPoint.getPort());

    fout << MESSAGE("socket: remote address=") << socket.getAddress() << MESSAGE(" remote port=") << socket.getPort() << ENDL;

    fout << MESSAGE("Talking with server...") << ENDL;

    {
      FormatOutputStream outstream(socket); // must be destroyed before socket is closed
      FormatInputStream instream(socket);

      fout << MESSAGE("Press enter to continue") << ENDL;
      fin.wait();
      fin.skip(1);

      fout << MESSAGE("Sending request") << ENDL;
      outstream << MESSAGE("Hi, I'm the client") << ENDL;

      fout << MESSAGE("Waiting for response") << FLUSH;
      while (!instream.wait(1000000)) {
        fout << '.' << FLUSH;
      }
      fout << ENDL;

      fout << MESSAGE("Processing response") << ENDL;
      fout << MESSAGE(">: ");
      while (instream.available()) { // read response
        char ch;
        instream >> ch;
        fout << ch;
      }

      fout << MESSAGE("Sending termination request") << ENDL;
      outstream << MESSAGE("Thank you and have a nice day") << ENDL;
    }

    fout << MESSAGE("Closing socket") << ENDL;
    socket.close();
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    String host = InetAddress::getLocalHost(); // default host
    String service = "1234"; // default service
    
    const Array<String> arguments = getArguments();
    
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      host = arguments[0]; // the address
      break;
    case 2:
      host = arguments[1]; // the address
      service = arguments[2]; // the service
      break;
    default:
      fout << MESSAGE("client [host] [service]") << ENDL;
      return;
    }
    client(host, service);
  }
};

STUB(ClientApplication);
