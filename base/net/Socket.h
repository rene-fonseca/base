/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SOCKET_H
#define _DK_SDU_MIP__BASE_NET__SOCKET_H

#include "base/concurrency/Synchronize.h"
#include "base/io/FileDescriptor.h"
#include "base/io/FileDescriptorInputStream.h"
#include "base/io/FileDescriptorOutputStream.h"
#include "NetworkException.h"
#include "InetAddress.h"
#include "base/string/FormatOutputStream.h"

/**
  This class implements a socket. A socket is an endpoint for communication between two machines. MT-level is safe.

  @author René Møller Fonseca
  @version 1.0
*/

class Socket : public FileDescriptor, public Synchronizeable<> {
private:

  /** Specifies the remote address to which the socket is connected. */
  InetAddress remoteAddress;
  /** Specifies the remote port (in host byte order) to which the socket is connected (unconnected if 0). */
  unsigned short remotePort;
  /** Specifies the local address to which the socket is bound. */
  InetAddress localAddress;
  /** Specifies the local port (in host byte order) to which the socket is bound (unbound if 0). */
  unsigned short localPort;

  FileDescriptorInputStream inputStream;
  FileDescriptorOutputStream outputStream;
protected:

  /** Returns true if socket has been created. */
  inline bool isCreated() const throw() {return getHandle() != -1;}; // read atomically
  /** Returns true if socket is connected. */
  inline bool isConnected() const throw() {return remotePort != 0;}; // read atomically
  /** Returns true if socket is bound. */
  inline bool isBound() const throw() {return localPort != 0;}; // read atomically
  /** Get socket option. */
  void getOption(int option, void* buffer, socklen_t* len) const throw(IOException);
  /** Set socket option. */
  void setOption(int option, const void* buffer, socklen_t len) throw(IOException);
  /** Get boolean socket option. */
  bool getBooleanOption(int option) const throw(IOException);
  /** Set boolean socket option. */
  void setBooleanOption(int option, bool value) throw(IOException);
public:

  /**
    Initializes socket object.
  */
  Socket() throw();

  /**
    Accepts a connection from specified socket.

    @param socket Specifies the socket to accept a connection from.
    @return True if connection has been accepted. False, if connection could not be accepted without blocking.
  */
  bool accept(Socket& socket) throw(IOException);

  /**
    Returns the number of bytes that can be read from this socket without blocking.
  */
  unsigned int available() const throw(IOException);

  /**
    Binds the socket to the address and port.

    @param addr The IP address the socket should be bound to.
    @param port The port the socket should be bound to.
  */
  void bind(const InetAddress& addr, unsigned short port) throw(IOException);

  /**
    Closes this socket.
  */
  void close() throw(IOException);

  /**
    Connects this socket to the specified address and port.

    @param addr The IP address to connect to.
    @param port The port to connect to.
  */
  void connect(const InetAddress& addr, unsigned  short port) throw(IOException);

  /**
    Creates either a stream or a datagram socket.

    @param stream True if stream socket should be created (otherwise a datagram socket is created).
  */
  void create(bool stream) throw(IOException);

  /**
    Returns true if socket is a stream socket.
  */
  bool isStream() const throw();

  /**
    Returns the IP address to which the socket is connected.
  */
  const InetAddress& getAddress() const throw();

  /**
    Returns the remote port to which the socket is connected.
  */
  unsigned short getPort() const throw();

  /**
    Returns the local IP address to which the socket is bound.
  */
  const InetAddress& getLocalAddress() const throw();

  /**
    Returns the local port to which the socket is bound.
  */
  unsigned short getLocalPort() const throw();

  /**
    Sets the maximum length the queue of pending connections may grow to. The backlog argument may be silently reduced.

    @param backlog The maxium length of the queue.
  */
  void listen(unsigned int backlog) throw(IOException);

  /**
    Returns the input stream of socket.
  */
  InputStream getInputStream();

  /**
    Returns the output stream of socket.
  */
  OutputStream getOutputStream();

  /**
    Disables the input stream for this socket.
  */
  void shutdownInputStream() throw(IOException);

  /**
    Disables the output stream for this socket.
  */
  void shutdownOutputStream() throw(IOException);

  /**
    Destroys the socket object.
  */
  ~Socket() throw(IOException);


  /**
    Returns true if 'bind' allows local addresses to be reused.
  */
  bool getReuseAddress() const throw(IOException);

  /**
    Sets the local address reuse flag of this socket.
  */
  void setReuseAddress(bool value) throw(IOException);

  /**
    Returns true if connection is kept alive.
  */
  bool getKeepAlive() const throw(IOException);

  /**
    Sets the keep alive flag of this socket.
  */
  void setKeepAlive(bool value) throw(IOException);

  /**
    Returns true if broadcast datagrams allowed on this socket.
  */
  bool getBroadcast() const throw(IOException);

  /**
    Sets the broadcast datagram flag of this socket.
  */
  void setBroadcast(bool value) throw(IOException);

  /**
    Gets the linger interval. Returns -1 if linger is disabled.
  */
  int getLinger() const throw(IOException);

  /**
    Sets the linger interval. Negative time disables the linger.
  */
  void setLinger(int seconds) throw(IOException);

  /**
    Gets the size of the receive buffer.
  */
  int getReceiveBufferSize() const throw(IOException);

  /**
    Sets the size of the receive buffer.
  */
  void setReceiveBufferSize(int size) const throw(IOException);

  /**
    Gets the size of the send buffer.
  */
  int getSendBufferSize() const throw(IOException);

  /**
    Sets the size of the send buffer.
  */
  void setSendBufferSize(int size) throw(IOException);

  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);
};

/**
  Writes a string representation of a socket to a stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);

#endif
