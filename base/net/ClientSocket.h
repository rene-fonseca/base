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
  inline InetAddress getLocalAddress() {return Socket::getLocalAddress();}

  /**
    Returns the local port to which the socket is bound.
  */
  inline unsigned short getLocalPort() {return Socket::getLocalPort();}

  /**
    Returns the input stream of socket.
  */
//  inline FileDescriptorInputStream* getInputStream() {return Socket::getInputStream();}

  /**
    Returns the output stream of socket.
  */
//  inline FileDescriptorOutputStream* getOutputStream() {return Socket::getOutputStream();}

  /**
    Gets the size of the receive buffer.
  */
  inline unsigned int getReceiveBufferSize() const {return Socket::getReceiveBufferSize();}

  /**
    Sets the size of the receive buffer.
  */
  void setReceiveBufferSize(unsigned int size) const;

  /**
    Gets the size of the send buffer.
  */
  inline unsigned int getSendBufferSize() const {return Socket::getSendBufferSize();}

  /**
    Sets the size of the send buffer.
  */
  void setSendBufferSize(unsigned int size);

  /**
    Closes this socket.
  */
  inline void close() throw(IOException) {Socket::close();}

  inline bool getKeepAlive() const throw() {return Socket::getKeepAlive();}

  inline unsigned int getLinger() const throw() {return Socket::getLinger();}

//  inline  getTimeout() const throw(IOException) {return Socket::getTimeout();}

//  inline void getTcpNoDelay() const throw(IOException) {return Socket::getTcpNoDelay();}

//  void setKeepAlive(bool value) throw(IOException);


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
};

#endif
