/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/net/InetService.h>
#include <base/net/Socket.h>
#include <base/net/ServerSocket.h>

void test() {
  fout << "Datagram network server...\n";

  fout << "Hostname: " << InetAddress::getLocalHost() << EOL;

  InetAddress address("0.0.0.0");
/*  {
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
  }*/

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

  Socket serverSocket;

  fout << "Creating datagram socket...\n";
  serverSocket.create(false);

  fout << "Binding to address...\n";
  serverSocket.bind(address, port);

  fout << "Server address...\n";
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << EOL;

  fout << "Requesting permission to send/receive broadcast messages...\n";
  serverSocket.setBroadcast(true);

  unsigned int datagrams = 10; // the number of connections to accept

  while (datagrams--) {
    char buffer[4096];
    InetAddress remoteAddress;
    unsigned short remotePort;

    fout << "Waiting for datagram...\n";
    unsigned int bytesReceived = serverSocket.receiveFrom((char*)&buffer, sizeof(buffer), remoteAddress, remotePort);

    fout << "Datagram of " << bytesReceived << " bytes received from " << remoteAddress
         << " on port " << remotePort << EOL;
    fout << buffer;

    fout << "Sending datagram back to client...\n";
    unsigned int bytesSent = serverSocket.sendTo((char*)&buffer, bytesReceived, remoteAddress, remotePort);
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
