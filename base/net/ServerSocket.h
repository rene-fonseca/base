/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SERVER_SOCKET_H
#define _DK_SDU_MIP__BASE_NET__SERVER_SOCKET_H

#include <base/net/Socket.h>
#include <base/net/ClientSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a stream server socket. A socket is an endpoint for
  communication between two hosts.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ServerSocket : protected Socket {
public:

  /**
    Initializes an invalidated socket object (ie. unconnected and unbound).
  */
  inline ServerSocket() throw() {}

  /**
    Creates a server stream socket and binds it to the specified port and IP address.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host. If zero the socket is bound to a unique port.
    @param backlog The maxium length of the queue.
  */
  ServerSocket(const InetAddress& addr, unsigned short port, unsigned int backlog) throw(IOException);

  /**
    Accepts the first connection from the queue of pending connections on this
    socket. This function is used with a connection-oriented socket.

    @return True if connection has been accepted. False, if connection could
    not be accepted without blocking.
  */
  inline StreamSocket accept() throw(IOException) {return StreamSocket(*this);}

  /**
    Closes this socket.
  */
  inline void close() throw(IOException) {return Socket::close();}

  /**
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful accept determine the locally
    assigned address and port if unspecified.
  */
  inline void getName() throw() {Socket::getName();}

  /**
    Returns the local IP address to which the socket is bound.
  */
  inline const InetAddress& getLocalAddress() const throw() {return Socket::getLocalAddress();}

  /**
    Returns the local port to which the socket is bound.
  */
  inline unsigned short getLocalPort() const throw() {return Socket::getLocalPort();}

  /**
    Sets the blocking mode of the socket.
  */
  inline void setNonBlocking(bool value) throw(IOException) {Socket::setNonBlocking(value);}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
