/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/net/Socket.h>
#include <base/net/StreamSocket.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a stream server socket. A socket is an endpoint for
  communication between two hosts.

  @short Server socket.
  @ingroup net
  @version 1.0
*/

class ServerSocket : protected Socket {
public:

  /**
    Initializes an invalidated socket object (i.e. unconnected and unbound).
  */
  inline ServerSocket() throw() {
  }

  ServerSocket(const ServerSocket& eq) throw();

  /**
    Creates a server stream socket and binds it to the specified port and IP address.

    @param address The IP address of the host to connect to.
    @param port The port to connect to on the host. If zero the socket is bound to a unique port.
    @param backlog The maxium length of the queue.
  */
  ServerSocket(
    const InetAddress& address,
    unsigned short port,
    unsigned int backlog) throw(IOException);

  /**
    Accepts the first connection from the queue of pending connections on this
    socket. This function is used with a connection-oriented socket. This will
    block the calling thread for eternity if not connections are made.

    @return True if connection has been accepted. False, if connection could
    not be accepted without blocking.
  */
  inline StreamSocket accept() throw(IOException) {
    return StreamSocket(*this);
  }

  /**
    Accepts the first connection from the queue of pending connections on this
    socket. This function is used with a connection-oriented socket.

    @param milliseconds The maximum time to block.
    
    @return An invalid socket if the timeout period expired.
  */
  inline StreamSocket accept(unsigned int milliseconds) throw(IOException) {
    return StreamSocket(*this);
  }

  /**
    Closes this socket.
  */
  inline void close() throw(IOException) {
    Socket::close();
  }

  /**
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful accept determine the locally
    assigned address and port if unspecified.
  */
  inline void getName() throw() {
    Socket::getName();
  }

  /**
    Returns the local IP address to which the socket is bound.
  */
  inline const InetAddress& getLocalAddress() const throw() {
    return Socket::getLocalAddress();
  }

  /**
    Returns the local port to which the socket is bound.
  */
  inline unsigned short getLocalPort() const throw() {
    return Socket::getLocalPort();
  }

  /**
    Sets the blocking mode of the socket.
  */
  inline void setNonBlocking(bool value) throw(IOException) {
    Socket::setNonBlocking(value);
  }

  /**
    Blocking wait for incoming connection request.
  */
  inline void wait() const throw(IOException) {
    Socket::wait();
  }

  /**
    Blocking wait for incoming connection request.

    @param microseconds The timeout periode in microseconds.
    
    @return True, if incoming connection is available. False, if the timeout
    periode expired.
  */
  inline bool wait(unsigned int microseconds) const throw(IOException) {
    return Socket::wait(microseconds);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
