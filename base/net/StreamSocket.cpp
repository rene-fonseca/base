/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/StreamSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

StreamSocket::StreamSocket() throw(NetworkException) {
  create(STREAM);
}

StreamSocket::StreamSocket(
  const InetAddress& address, unsigned short port) throw(NetworkException) {
  create(STREAM);
  connect(address, port);
}

StreamSocket::StreamSocket(
  const InetAddress& address,
  unsigned short port,
  InetAddress& localAddress,
  unsigned short localPort) throw(NetworkException) {
  create(STREAM);
  bind(localAddress, localPort);
  connect(address, port);
}

StreamSocket::StreamSocket(ServerSocket& socket) throw(NetworkException) {
  accept(*Cast::pointer<Socket*>(&socket));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
