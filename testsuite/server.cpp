/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/net/ClientSocket.h>

void test() {
  fout << "Testing ServerSocket...\n";

  fout << "Hostname: " << InetAddress::getLocalHost() << EOL;

  InetAddress address;
  {
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
  }

  unsigned short port = 1234; // default
  try {
    InetService service("telnet");
    port = service.getPort();
    fout << "Service: name=" << service.getName()
         << " port=" << service.getPort()
         << " protocol=" << service.getProtocol() << EOL;
  } catch(ServiceNotFound& e) {
    fout << "Warning: " << e.getMessage() << EOL;
    fout << "Service: port=" << port << EOL;
  }
  port = 1234;

  fout << "Initializing server socket...\n";
  ServerSocket serverSocket(address, port, 1);

  fout << "Server address...\n";
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << EOL;

  fout << "Waiting for client to connect...\n";

  unsigned int connection = 1; // the number of connections so far

  while (connection < 4) {
    fout << "Initializing socket...\n";
    StreamSocket socket(serverSocket.accept());

    fout << "Connection number " << connection++ << " established from\n";
    fout << "  remote: address=" << socket.getAddress() << " port=" << socket.getPort() << EOL;

    fout << "Talking with client...\n";
    FormatOutputStream fstream(socket);

    char buffer[4096];
    socket.read((char*)&buffer, sizeof(buffer));
    fout << buffer;

    fstream << "Thanks for your reply\n";
    socket.shutdownOutputStream();

    fout << "Closing connection...\n";
    socket.close();
  }

  fout << "Closing server socket...\n";
  serverSocket.close();
}

int main() {
  try {
    test();
  } catch(NetworkException& e) {
    ferr << "NetworkException: " << e.getMessage() << "\n";
  } catch(IOException& e) {
    ferr << "IOException: " << e.getMessage() << "\n";
  } catch(...) {
    ferr << "Unknown exception\n";
  }
  fout << "Completed\n";
}
