/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SOCKET_H
#define _DK_SDU_MIP__BASE_NET__SOCKET_H

#include <base/Object.h>
#include <base/io/async/AsynchronousInputStream.h>
#include <base/io/async/AsynchronousOutputStream.h>
#include <base/concurrency/Synchronize.h>
#include <base/io/AccessDenied.h>
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/net/NetworkException.h>
#include <base/net/InetAddress.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a socket. A socket is an endpoint for communication
  between two hosts on a network. MT-level is safe.

  @see StreamSocket ServerSocket
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Socket : public virtual Object, public virtual AsynchronousInputStream, public virtual AsynchronousOutputStream, public Synchronizeable<Unsafe> {
private:

  typedef Unsafe LOCK;

  class SocketImpl : public virtual ReferenceCountedObject {
  private:

    /** Handle to the socket. */
    OperatingSystem::Handle handle;
    /** Specifies the remote address to which the socket is connected. */
    InetAddress remoteAddress;
    /** Specifies the remote port (in host byte order) to which the socket is connected (unconnected if 0). */
    unsigned short remotePort;
    /** Specifies the local address to which the socket is bound. */
    InetAddress localAddress;
    /** Specifies the local port (in host byte order) to which the socket is bound (unbound if 0). */
    unsigned short localPort;
    /** Specifies whether the end of the stream has been reached. */
    bool end;
  public:

    /** Initializes invalid socket. */
    SocketImpl() throw();
    /** Initializes the socket with the specified handle. */
    SocketImpl(OperatingSystem::Handle handle) throw();
    /** Returns the socket handle. */
    inline OperatingSystem::Handle getHandle() const throw() {return handle;}
    /** Returns the local address. */
    inline const InetAddress& getLocalAddress() const throw() {return localAddress;}
    /** Sets the local address. */
    inline void setLocalAddress(const InetAddress& value) throw() {localAddress = value;}
    /** Returns the local port. */
    inline unsigned short getLocalPort() const throw() {return localPort;}
    /** Sets the local port. */
    inline void setLocalPort(unsigned short port) throw() {localPort = port;}
    /** Returns the remote address. */
    inline const InetAddress& getRemoteAddress() const throw() {return remoteAddress;}
    /** Sets the remote address. */
    inline void setRemoteAddress(const InetAddress& value) throw() {remoteAddress = value;}
    /** Returns the remote port. */
    inline unsigned short getRemotePort() const throw() {return remotePort;}
    /** Sets the remote port. */
    inline void setRemotePort(unsigned short port) throw() {remotePort = port;}
    /** Returns true if socket has been created. */
    inline bool isCreated() const throw() {return getHandle() != OperatingSystem::INVALID_HANDLE;}
    /** Returns true if socket is connected. */
    inline bool isConnected() const throw() {return getRemotePort() != 0;}
    /** Returns true if socket is bound. */
    inline bool isBound() const throw() {return getLocalPort() != 0;}
    /** Returns true if the end has been reached. */
    inline bool atEnd() const throw() {return end;}
    /** Specifies that the end has been reached. */
    inline void onEnd() throw() {end = true;}
    /** Releases the resources use by the socket. */
    ~SocketImpl() throw(IOException);
  };
protected:

  /** The internal socket representation. */
  ReferenceCountedObjectPointer<SocketImpl> socket;

  /** Returns the handle. */
  inline OperatingSystem::Handle getHandle() const throw() {return socket->getHandle();}
  /** Get boolean socket option. */
  bool getBooleanOption(int option) const throw(IOException);
  /** Set boolean socket option. */
  void setBooleanOption(int option, bool value) throw(IOException);
public:

  /**
    Initializes an invalidated socket object (ie. unconnected and unbound).
  */
  Socket() throw();

  /**
    Initialization of socket from other socket.
  */
  inline Socket(const Socket& copy) throw() : socket(copy.socket) {}

  /**
    Assignment of socket to socket.
  */
  inline Socket& operator=(const Socket& eq) throw() {
    if (&eq != this) { // protect against self assignment
      socket = eq.socket;
    }
    return *this;
  }

  /**
    Accepts the first connection from the queue of pending connections on the
    specified socket. This function is used with a connection-oriented socket.

    @param socket Specifies the socket to accept a connection from.

    @return True if connection has been accepted. False, if connection could
    not be accepted without blocking.
  */
  bool accept(Socket& socket) throw(IOException);

  /**
    Associates a local name (address and port) with this socket.

    @param addr The IP address the socket should be bound to. If unspecified the assigned address may not be known until the socket has been connected with connect or accept.
    @param port The port the socket should be bound to. If zero the socket is assigned to a unique port.
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
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful accept or connect to determine the
    locally assigned address and port if unspecified.
  */
  void getName() throw();

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
    This function places the socket in a state where it is listening for
    incoming connections. It also sets the maximum length of the queue of
    pending connections. The backlog argument may be silently reduced. The
    socket must be bound and unconnected.

    @param backlog The maxium length of the queue.
  */
  void listen(unsigned int backlog) throw(IOException);

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
    Gets the linger interval.

    @return -1 if linger is disabled.
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
  void setReceiveBufferSize(int size) throw(IOException);

  /**
    Gets the size of the send buffer.
  */
  int getSendBufferSize() const throw(IOException);

  /**
    Sets the size of the send buffer.
  */
  void setSendBufferSize(int size) throw(IOException);

  /**
    Sets the blocking mode of the socket.
  */
  void setNonBlocking(bool value) throw(IOException);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Returns true if the end has been reached.
  */
  bool atEnd() const throw();

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException); 

  /**
    Fills the buffer with bytes from the socket input stream. Blocks if asked
    to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

  /**
    Sends the contents of the buffer to the specified address using an unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The Internet address of the remote host.
    @param port The port of the remote host.
    @return The number of bytes sent.
  */
  unsigned int sendTo(const char* buffer, unsigned int size, const InetAddress& address, unsigned short port) throw(IOException);

  /**
    Receives data from any address using an unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The address.
    @param port The port.

    @return The number of bytes received.
  */
  unsigned int receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException);

  void asyncCancel() throw(AsynchronousException);
  
  AsynchronousReadOperation read(char* buffer, unsigned int bytesToRead, AsynchronousReadEventListener* listener) throw(AsynchronousException);

  AsynchronousWriteOperation write(const char* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(AsynchronousException);
  
  /**
    Blocking wait for input to become available.
  */
  void wait() const throw(IOException);

  /**
    Blocking wait for input to become available.

    @param microseconds The timeout periode in microseconds.
    
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int microseconds) const throw(IOException);

  /**
    Writes a string representation of a Socket object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);
};

/**
  Writes a string representation of a Socket object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
