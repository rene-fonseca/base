/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SOCKET_H
#define _DK_SDU_MIP__BASE_NET__SOCKET_H

#include <base/Object.h>
#include <base/concurrency/Synchronize.h>
#include <base/io/AccessDenied.h>
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/net/NetworkException.h>
#include <base/net/InetAddress.h>
#include <base/string/FormatOutputStream.h>

/**
  This class implements a socket. A socket is an endpoint for communication
  between two machines. MT-level is safe.

  @see ClientSocket ServerSocket
  @author Ren� M�ller Fonseca
  @version 1.1
*/

class Socket : public virtual Object, public Synchronizeable<DefaultLock> {
private:

  typedef DefaultLock LOCK;

  /** Specifies the remote address to which the socket is connected. */
  InetAddress remoteAddress;
  /** Specifies the remote port (in host byte order) to which the socket is connected (unconnected if 0). */
  unsigned short remotePort;
  /** Specifies the local address to which the socket is bound. */
  InetAddress localAddress;
  /** Specifies the local port (in host byte order) to which the socket is bound (unbound if 0). */
  unsigned short localPort;
  /** Handle to the socket. */
  FileDescriptor fd;
protected:

  /** Returns true if socket has been created. */
  inline bool isCreated() const throw() {return fd.getHandle() != -1;}; // read atomically
  /** Returns true if socket is connected. */
  inline bool isConnected() const throw() {return remotePort != 0;}; // read atomically
  /** Returns true if socket is bound. */
  inline bool isBound() const throw() {return localPort != 0;}; // read atomically
  /** Get boolean socket option. */
  bool getBooleanOption(int option) const throw(IOException);
  /** Set boolean socket option. */
  void setBooleanOption(int option, bool value) throw(IOException);
public:

  /**
    Initializes unconnected socket object.
  */
  Socket() throw();

  /**
    Accepts a connection from specified socket. This is not allowed with a
    stream socket.

    @param socket Specifies the socket to accept a connection from.

    @return True if connection has been accepted. False, if connection could
    not be accepted without blocking.
  */
  bool accept(Socket& socket) throw(IOException);

  /**
    Associates name (address and port) with this socket.

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

    @param stream True if stream socket should be created (otherwise a
    datagram socket is created).
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
    Sets the maximum length the queue of pending connections may grow to. The
    backlog argument may be silently reduced. This is not allowed with a stream
    socket.

    @param backlog The maxium length of the queue.
  */
  void listen(unsigned int backlog) throw(IOException);

  /**
    Returns the input stream of socket.
  */
  FileDescriptorInputStream getInputStream() throw();

  /**
    Returns the output stream of socket.
  */
  FileDescriptorOutputStream getOutputStream() throw();

  /**
    Disables the input stream for this socket.
  */
  void shutdownInputStream() throw(IOException);

  /**
    Disables the output stream for this socket.
  */
  void shutdownOutputStream() throw(IOException);

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

  /**
    Sends the contents of the buffer to the specified address using an unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The address.
    @param port The port.
    @return The number of bytes sent.
  */
  unsigned int sendTo(const char* buffer, unsigned int size, InetAddress address, unsigned short port) throw(IOException);

  /**
    Receives data from any address using an unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The address.
    @param port The port.

    @return The number of bytes received.
  */
  unsigned int receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException);

  /**
    Writes a string representation of a Socket object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);
};

/**
  Writes a string representation of a Socket object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);

#endif
