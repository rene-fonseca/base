/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/ServerSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ServerSocket::ServerSocket(const InetAddress& addr, unsigned short port, unsigned int backlog) throw(IOException) {
  create(true);
  bind(addr, port);
  listen(backlog);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
