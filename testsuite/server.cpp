/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/Integer.h>
#include <typeinfo>

void server(String<> a, String<> servicename) {

  fout << "Hostname: " << InetAddress::getLocalHost() << EOL;

  {
    List<InetInterface> interfaces = InetInterface::getInetInterfaces();
    List<InetInterface>::ReadOnlyEnumeration enu(interfaces);
    fout << "Available interfaces:\n";
    while (enu.hasNext()) {
      const InetInterface* i = enu.next();
      fout << "  interface: index=" << i->getIndex() << " name=" << i->getName() << EOL;
    }
  }

  InetAddress address; // the address to bind the server socket to
  if (a == "") { // should we find an address
    fout << "Local addresses:\n";
    List<InetAddress> addresses = InetAddress::getAddressesByName(InetAddress::getLocalHost());
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
           << "  protocol=" << service.getProtocol() << EOL;
    } catch(ServiceNotFound& e) {
      fout << "Warning: " << e.getMessage() << EOL;
      fout << "Service: port=" << port << EOL;
    }
  }

  fout << "Initializing server socket...\n";
  ServerSocket serverSocket(address, port, 1);

  fout << "Server address...\n";
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << EOL;

  fout << "Waiting for client to connect...\n";

  unsigned int connection = 1; // the number of connections so far
  bool terminate = false; // specifies whether the server should be terminated

  while (!terminate) {
    fout << "Listening for connections...\n";
    StreamSocket socket(serverSocket.accept());

    fout << "Connection number " << connection++ << " established from\n";
    fout << "  remote: address=" << socket.getAddress() << " port=" << socket.getPort() << EOL;

    fout << "Talking with client...\n";
    {
      FormatOutputStream outstream(socket); // must be destroyed before socket is closed
      FormatInputStream instream(socket);

      fout << "Waiting for request";
      while (!instream.wait(1000000)) {
        fout << ".";
      }
      fout << EOL;

      fout << "Processing request\n";
      fout << ">: ";
      while (instream.available()) {
        char ch;
        instream >> ch;
        fout << ch;
      }

      fout << "Sending acknowledge\n";
      outstream << "Hi, I'm the server and this is my response\n";

      fout << "Waiting for termination request";
      while (!instream.wait(1000000)) {
        fout << ".";
      }
      fout << EOL;

      fout << "Processing terminating request\n";
      fout << ">: ";
      while (instream.available()) {
        char ch;
        instream >> ch;
        fout << ch;
      }
    }

    fout << "Closing connection...\n";
    socket.close();

    if (connection == 5) {
      terminate = true;
    }
  }

  fout << "Closing server socket...\n";
  serverSocket.close();
}

int main(int argc, char* argv[]) {
  fout << "Testing ServerSocket...\n";

  String<> address; // default address
  String<> service = "1234"; // default service

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
    fout << "server [address] [service]\n";
    return 0; // stop
  }

  try {
    server(address, service);
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
