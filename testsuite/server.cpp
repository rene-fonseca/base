/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/net/InetService.h>
#include <base/net/Socket.h>
#include <base/net/ServerSocket.h>

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

  Socket serverSocket;

  fout << "Creating stream socket...\n";
  serverSocket.create(true);

  fout << "Binding to address...\n";
  serverSocket.bind(address, port);

  fout << "Server address...\n";
  fout << "  address=" << serverSocket.getLocalAddress() << " port=" << serverSocket.getLocalPort() << EOL;

  fout << "Listening for one client...\n";
  serverSocket.listen(1);

  fout << "Waiting for client to connect...\n";

  unsigned int connection = 1; // the number of connections so far

  while (connection < 4) {
    Socket client;
    client.accept(serverSocket);

    fout << "Connection number " << connection++ << " established from\n";
    fout << "  remote: address=" << client.getAddress() << " port=" << client.getPort() << EOL;

    fout << "Talking with client...\n";
    FileDescriptorOutputStream socketOutput = client.getOutputStream();
    FileDescriptorInputStream socketInput = client.getInputStream();
    FormatOutputStream fstream(socketOutput);

    char buffer[4096];
    socketInput.read((char*)&buffer, sizeof(buffer));
    fout << buffer;

    fstream << "Thanks for your reply\n";
    client.shutdownOutputStream();

//    fout << "Closing connection...\n";
//    client.close();
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
