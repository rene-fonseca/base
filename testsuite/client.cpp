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
  fout << "Testing StreamSocket...\n";

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
  StreamSocket socket;

  fout << "Connecting socket...\n";
  socket.connect(address, port);

  fout << "socket: remote address=" << socket.getAddress() << " remote port=" << socket.getPort() << EOL;

  fout << "Talking with server...\n";
  FormatOutputStream fstream(socket);

  fstream << "Hi, I'm the client\n";
  socket.shutdownOutputStream();

  char buffer[4096];
  socket.read((char*)&buffer, sizeof(buffer));
  fout << buffer;

  fout << "Closing socket\n";
  socket.close();
}

int main(int argc, char* argv[]) {

//  if (argc != 3) {
//    fout << "client hostname service\n";
//    return 0;
//  }
//  hostname = argv[1]; // the hostname of the server
//  servicename = argv[2]; // the name of the service

  hostname = "lendum";
  servicename = "1234";

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
