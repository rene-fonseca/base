/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/StreamSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

StreamSocket::StreamSocket() throw(IOException) {
  create(STREAM);
}

StreamSocket::StreamSocket(const InetAddress& addr, unsigned short port) throw(IOException) {
  create(STREAM);
  connect(addr, port);
}

StreamSocket::StreamSocket(
  const InetAddress& addr,
  unsigned short port,
  InetAddress& localAddr,
  unsigned short localPort) throw(IOException) {
  create(STREAM);
  bind(localAddr, localPort);
  connect(addr, port);
}

StreamSocket::StreamSocket(ServerSocket& socket) throw(IOException) {
  accept(*Cast::pointer<Socket*>(&socket));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
