/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/StreamSocket.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

StreamSocket::StreamSocket() {
  create(STREAM);
}

StreamSocket::StreamSocket(
  const InetAddress& address, unsigned short port)
{
  create(STREAM);
  connect(address, port);
}

StreamSocket::StreamSocket(
  const InetAddress& address,
  unsigned short port,
  InetAddress& localAddress,
  unsigned short localPort)
{
  create(STREAM);
  bind(localAddress, localPort);
  connect(address, port);
}

StreamSocket::StreamSocket(ServerSocket& socket)
{
  accept(*Cast::pointer<Socket*>(&socket));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
