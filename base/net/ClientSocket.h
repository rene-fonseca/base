/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__CLIENT_SOCKET_H
#define _DK_SDU_MIP__BASE_NET__CLIENT_SOCKET_H

#include <base/net/Socket.h>

/**
  This class implements a client stream socket. A socket is an endpoint for communication between two machines.

  @author René Møller Fonseca
  @version 1.0
*/

class ClientSocket : protected Socket {
public:

  /**
    Creates an unconnected stream socket.
  */
  ClientSocket() throw(IOException);

  /**
    Creates a stream socket and connects it to the specified port at the specified IP address.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host.
  */
  ClientSocket(const InetAddress& addr, unsigned short port) throw(IOException);

  /**
    Creates a stream socket and connects it to the specified port at the specified IP address and also binds the socket to the local address and local port.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host.
    @param localAddr The local IP address the socket should be bound to.
    @param localPort The local port the socket should be bound to.
  */
  ClientSocket(const InetAddress& addr, unsigned short port, InetAddress& localAddr, unsigned short localPort) throw(IOException);

  /**
    Returns the IP address to which the socket is connected.
  */
  InetAddress getAddress();

  /**
    Returns the remote port to which the socket is connected.
  */
  unsigned short getPort();

  /**
    Returns the local IP address to which the socket is bound.
  */
  InetAddress getLocalAddress();

  /**
    Returns the local port to which the socket is bound.
  */
  unsigned short getLocalPort();

  /**
    Returns the input stream of socket.
  */
  InputStream* getInputStream();

  /**
    Returns the output stream of socket.
  */
  OutputStream* getOutputStream();

  /**
    Gets the size of the receive buffer.
  */
  unsigned int getReceiveBufferSize() const;

  /**
    Sets the size of the receive buffer.
  */
  void setReceiveBufferSize(unsigned int size) const;

  /**
    Gets the size of the send buffer.
  */
  unsigned int getSendBufferSize() const;

  /**
    Sets the size of the send buffer.
  */
  void setSendBufferSize(unsigned int size);

  /**
    Closes this socket.
  */
  void close() throw(IOException);

  bool getKeepAlive() const throw();
  unsigned int getLinger() const throw();
  unsigned int getTimeout() const throw();
  bool getTcpNoDelay() const throw();
  void setKeepAlive(bool value) throw(IOException);
  void getLinger(unsigned int value) throw(IOException);
  void getTimeout(unsigned int value) throw(IOException);
  void getTcpNoDelay(bool value) throw(IOException);

  /**
    Returns the input stream of socket.
  */
//  InputStream* getInputStream();

  void shutdownInputStream() throw(IOException);

  /**
    Returns the output stream of socket.
  */
//  OutputStream* getOutputStream();

  void shutdownOutputStream() throw(IOException);

  /**
    Writes string representation of socket to a stream.
  */
  FormatOutputStream& operator<<(FormatOutputStream& stream);

  /**
    Destroys the socket.
  */
  ~ClientSocket();
};

#endif
