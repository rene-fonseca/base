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
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>

using namespace com::azure::dev::base;

class ClientApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ClientApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("client", numberOfArguments, arguments, environment) {
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
          fout << "  address " << index++ << ": " << *temp << " (USING THIS)" << ENDL;
        } else {
          fout << "  address " << index++ << ": " << *temp << ENDL;
        }
      }
    }

    InetEndPoint endPoint(address, service);
    fout << "End point: address=" << endPoint.getAddress() << " port=" << endPoint.getPort() << ENDL;

    fout << "Initializing socket..." << ENDL;
    StreamSocket socket;

    fout << "Connecting socket..." << ENDL;
    socket.connect(endPoint.getAddress(), endPoint.getPort());

    fout << "socket: remote address=" << socket.getAddress() << " remote port=" << socket.getPort() << ENDL;

    fout << "Talking with server..." << ENDL;

    {
      FormatOutputStream outstream(socket); // must be destroyed before socket is closed
      FormatInputStream instream(socket);

      fout << "Press enter to continue" << ENDL;
      fin.wait();
      fin.skip(1);

      fout << "Sending request" << ENDL;
      outstream << "Hi, I'm the client" << ENDL;

      fout << "Waiting for response" << FLUSH;
      while (!instream.wait(1000000)) {
        fout << '.' << FLUSH;
      }
      fout << ENDL;

      fout << "Processing response" << ENDL;
      fout << ">: ";
      while (instream.available()) { // read response
        char ch;
        instream >> ch;
        fout << ch;
      }

      fout << "Sending termination request" << ENDL;
      outstream << "Thank you and have a nice day" << ENDL;
    }

    fout << "Closing socket" << ENDL;
    socket.close();
  }

  void main() throw() {
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
      host = arguments[0]; // the address
      break;
    case 2:
      host = arguments[0]; // the address
      service = arguments[1]; // the service
      break;
    default:
      fout << "client [host] [service]" << ENDL;
      return;
    }
    client(host, service);
  }
};

STUB(ClientApplication);
