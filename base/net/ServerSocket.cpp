/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/ServerSocket.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ServerSocket::ServerSocket(
  const InetAddress& address,
  unsigned short port,
  unsigned int backlog) throw(IOException)
{
  create(STREAM);
  bind(address, port);
  listen(backlog);
  getName();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
