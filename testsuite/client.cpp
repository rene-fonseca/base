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
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/Type.h>

using namespace base;

void client(String host, String service) {
  fout << "Server: " << host << ENDL;

  InetAddress address; // the address of the remote host
  {
    fout << "Server addresses:" << ENDL;
    List<InetAddress> addresses = InetAddress::getAddressesByName(host);
    List<InetAddress>::ReadOnlyEnumeration enu(addresses);
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
      fout << "." << FLUSH;
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

int main(int argc, char* argv[]) {
  fout << "Testing ClientSocket..." << ENDL;

  String host = InetAddress::getLocalHost(); // default host
  String service = "1234"; // default service

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
    fout << "client [host] [service]" << ENDL;
    return 0; // stop
  }

  try {
    client(host, service);
  } catch(Exception& e) {
    ferr << getTypename(e) << ": "<< e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  fout << "Completed" << ENDL;
  return 0;
}
