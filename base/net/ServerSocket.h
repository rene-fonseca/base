/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SERVER_SOCKET_H
#define _DK_SDU_MIP__BASE_NET__SERVER_SOCKET_H

#include "Socket.h"

/**
  This class implements a stream server socket. A socket is an endpoint for communication between two machines.

  @author René Møller Fonseca
  @version 1.0
*/

class ServerSocket : protected Socket {
public:

  ServerSocket() throw(IOException);

  /**
    Creates a stream socket and connects it to the specified port at the specified IP address.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host.
  */
  ServerSocket(const InetAddress& addr, unsigned short port) throw(IOException);

  /**
    Closes this socket.
  */
  void close() throw(IOException);

  /**
    Returns the IP address to which the socket is connected.
  */
  InetAddress getAddress();

  /**
    Returns the remote port to which the socket is connected.
  */
  short int getPort();

  /**
    Returns the local IP address to which the socket is bound.
  */
  InetAddress getLocalAddress();

  /**
    Returns the local port to which the socket is bound.
  */
  short int getLocalPort();

  /**
    Returns the input stream of socket.
  */
  InputStream* getInputStream();

  /**
    Returns the output stream of socket.
  */
  OutputStream* getOutputStream();

  /**
    Destroys the socket.
  */
  ~ServerSocket();
};

#endif
