/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/ClientSocket.h>

StreamSocket::StreamSocket() throw(IOException) {
  create(true);
}

StreamSocket::StreamSocket(const InetAddress& addr, unsigned short port) throw(IOException) {
  create(true);
  connect(addr, port);
}

StreamSocket::StreamSocket(const InetAddress& addr, unsigned short port, InetAddress& localAddr, unsigned short localPort) throw(IOException) {
  create(true);
  bind(localAddr, localPort);
  connect(addr, port);
}

StreamSocket::StreamSocket(ServerSocket& socket) throw(IOException) {
  accept(*(Socket*)&socket);
}
