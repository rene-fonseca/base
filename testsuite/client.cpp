/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/net/ClientSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>

void test() {
  fout << "Testing client socket...\n";

  fout << "Inet interfaces\n";
  List<InetInterface> interfaces = InetInterface::getInetInterfaces();
  List<InetInterface>::ReadOnlyEnumeration enu(interfaces);
  while (enu.hasNext()) {
    const InetInterface* interface = enu.next();
    fout << "  index=" << interface->getIndex() << " name=" << interface->getName() << EOL;
  }

  InetService service("telnet");
  fout << "Service: name=" << service.getName()
       << " port=" << service.getPort()
       << " protocol=" << service.getProtocol() << EOL;

  InetAddress address("172.30.33.16");
  fout << "Server address: " << address << EOL;

  ClientSocket socket(address, service.getPort());
//  socket.connect(address, service.getPort());
  fout << "socket: local address=" << socket.getLocalAddress() << " local port=" << socket.getLocalPort() << EOL;

  fout.flush();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
  } catch(...) {
    ferr << "Unknown exception\n";
  }
}
