/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/ClientSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  accept(*pointer_cast<Socket*>(&socket));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
