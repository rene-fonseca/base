/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/ServerSocket.h>

ServerSocket::ServerSocket(const InetAddress& addr, unsigned short port, unsigned int backlog) throw(IOException) {
  create(true);
  bind(addr, port);
  listen(backlog);
}
