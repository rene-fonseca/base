/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/ClientSocket.h>

ClientSocket::ClientSocket() throw(IOException) :
  Socket() {
}

ClientSocket::ClientSocket(const InetAddress& addr, unsigned short port) throw(IOException) :
  Socket() {
  create(true);
  connect(addr, port);
}

ClientSocket::ClientSocket(const InetAddress& addr, unsigned short port, InetAddress& localAddr, unsigned short localPort) throw(IOException) :
  Socket() {
  create(true);
  connect(addr, port);
}
