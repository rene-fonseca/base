/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/Integer.h>
#include <base/net/ClientSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>

char* hostname = 0;
char* servicename = 0;

void test() {
  fout << "Datagram network client...\n";

  fout << "Server: " << hostname << EOL;

  InetAddress address;
  {
    fout << "Server addresses:\n";
    List<InetAddress> addresses = InetAddress::getAddressesByName(hostname);
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
    // does not work
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
           << " port=" << service.getPort()
           << " protocol=" << service.getProtocol() << EOL;
    } catch(ServiceNotFound& e) {
      fout << "Warning: " << e.getMessage() << EOL;
      fout << "Service: port=" << port << EOL;
    }
  }
  port = 1234;

  fout << "Initializing socket...\n";
  Socket socket;

  fout << "Creating datagram socket...\n";
  socket.create(false);

  fout << "Requesting permission to send broadcast messages...\n";
  socket.setBroadcast(true);

  fout << "Sending datagram...\n";
  char sendBuffer[] = "THIS IS A DATAGRAM\n";
  unsigned int bytesSent = socket.sendTo((char*)&sendBuffer, sizeof(sendBuffer), address, port);

  char buffer[4096];
  InetAddress remoteAddress;
  unsigned short remotePort;

  fout << "Waiting for datagram...\n";
  unsigned int bytesReceived = socket.receiveFrom((char*)&buffer, sizeof(buffer), remoteAddress, remotePort);
  fout << "Datagram: bytesReceived=" << bytesReceived << EOL;
  fout << buffer;

  socket.close();
}

int main(int argc, char* argv[]) {

  if (argc != 3) {
    fout << "client hostname service\n";
    return 0;
  }
  hostname = argv[1]; // the hostname of the server
  servicename = argv[2]; // the name of the service

  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
    return 1;
  } catch(...) {
    ferr << "Unknown exception\n";
    return 1;
  }
  fout << "Completed" << EOL;
}
