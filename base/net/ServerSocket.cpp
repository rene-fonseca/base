/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/ServerSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ServerSocket::ServerSocket(const InetAddress& addr, unsigned short port, unsigned int backlog) throw(IOException) {
  create(true);
  bind(addr, port);
  listen(backlog);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
