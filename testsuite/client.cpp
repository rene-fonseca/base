/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Integer.h>
#include <base/net/ClientSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/concurrency/Thread.h>
#include <typeinfo>

void client(String<> host, String<> service) {
  fout << "Server: " << host << EOL;

  InetAddress address; // the address of the remote host
  {
    fout << "Server addresses:\n";
    List<InetAddress> addresses = InetAddress::getAddressesByName(host);
    List<InetAddress>::ReadOnlyEnumeration enu(addresses);
    unsigned int index = 0;
    while (enu.hasNext()) {
      const InetAddress* temp = enu.next();
      if (index == 0) { // use the first address
        address = *temp;
        fout << "  address " << index++ << ": " << *temp << " (USING THIS)" << EOL;
      } else {
        fout << "  address " << index++ << ": " << *temp << EOL;
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
  } catch(InvalidFormat& e) {
    try {
      InetService s(service);
      port = s.getPort();
      fout << "Service: name=" << s.getName()
           << "  port=" << s.getPort()
           << "  protocol=" << s.getProtocol() << EOL;
    } catch(ServiceNotFound& e) {
      fout << "Warning: " << e.getMessage() << EOL;
      fout << "Service: port=" << port << EOL;
    }
  }

  fout << "Initializing socket...\n";
  StreamSocket socket;

  fout << "Connecting socket...\n";
  socket.connect(address, port);

  fout << "socket: remote address=" << socket.getAddress() << " remote port=" << socket.getPort() << EOL;

  fout << "Talking with server...\n";

  {
    FormatOutputStream outstream(socket); // must be destroyed before socket is closed
    FormatInputStream instream(socket);

    fout << "Press enter to continue\n";
    fin.wait();
    fin.skip(1);

    fout << "Sending request\n";
    outstream << "Hi, I'm the client\n";

    fout << "Waiting for response";
    while (!instream.wait(1000000)) {
      fout << ".";
    }
    fout << EOL;

    fout << "Processing response\n";
    fout << ">: ";
    while (instream.available()) { // read response
      char ch;
      instream >> ch;
      fout << ch;
    }

    fout << "Sending termination request\n";
    outstream << "Thank you and have a nice day\n";
  }

  fout << "Closing socket\n";
  socket.close();
}

int main(int argc, char* argv[]) {
  fout << "Testing ClientSocket...\n";

  String<> host = InetAddress::getLocalHost(); // default host
  String<> service = "1234"; // default service

  switch (argc) {
  case 1:
    // use defaults
    break;
  case 2:
    host = argv[1]; // the address
    break;
  case 3:
    host = argv[1]; // the address
    service = argv[2]; // the service
    break;
  default:
    fout << "client [host] [service]\n";
    return 0; // stop
  }

  try {
    client(host, service);
  } catch(Exception& e) {
    ferr << typeid(e).name() << ": "<< e.getMessage() << EOL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception\n";
    return 1;
  }
  fout << "Completed\n";
  return 0;
}
