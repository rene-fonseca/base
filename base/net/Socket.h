/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/io/async/AsynchronousInputStream.h>
#include <base/io/async/AsynchronousOutputStream.h>
#include <base/concurrency/Synchronize.h>
#include <base/io/FileDescriptor.h>
#include <base/net/InetAddress.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Reference.h>
#include <base/OperatingSystem.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class MultipleSockets;

/**
  @defgroup net Network
*/

/**
  This class implements a socket. A socket is an endpoint for communication
  between two hosts on a network. MT-level is safe.

  @short Socket.
  @see StreamSocket ServerSocket
  @ingroup net
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Socket : public virtual Object,
                                      public virtual AsynchronousInputStream,
                                      public virtual AsynchronousOutputStream {
  friend class Initialization;
  friend class MultipleSockets;
public:

  /** Domain. */
  enum Domain {
    // UNIX, /**< UNIX domain. */
    IPV4, /**< IP version 4. */
    IPV6, /**< IP version 6. */
    DEFAULT_DOMAIN /**< The default domain. */
  };
  
  /** Socket type. */
  enum Kind {
    STREAM, /**< Stream socket. */
    DATAGRAM, /**< Datagram socket. */
    RAW /**< Raw socket. */
  };
private:
  
  class _COM_AZURE_DEV__BASE__API SocketImpl : public Handle {
  private:

    /** The socket domain. */
    Domain domain = DEFAULT_DOMAIN;
    /** The socket type. */
    Kind kind = STREAM;
    /** Specifies the remote address to which the socket is connected. */
    InetAddress remoteAddress;
    /**
      Specifies the remote port (in host byte order) to which the socket is
      connected (unconnected if 0).
    */
    unsigned short remotePort = 0;
    /** Specifies the local address to which the socket is bound. */
    InetAddress localAddress;
    /**
      Specifies the local port (in host byte order) to which the socket is
      bound (unbound if 0).
    */
    unsigned short localPort = 0;
  public:

    /** Invalid socket. */
    static SocketImpl* invalid;

    /** Returns invalid socket. */
    static SocketImpl* getInvalid() noexcept;
    
    /** Initializes the socket with the specified handle. */
    SocketImpl(
      OperatingSystem::Handle handle, Domain domain, Kind kind) noexcept;
    
    /** Returns the protocol. */
    inline Domain getDomain() const noexcept
    {
      return domain;
    }
    
    /** Returns the type. */
    inline Kind getKindype() const noexcept
    {
      return kind;
    }
    
    /** Returns the local address. */
    inline const InetAddress& getLocalAddress() const noexcept
    {
      return localAddress;
    }
    
    /** Sets the local address. */
    inline void setLocalAddress(const InetAddress& value) noexcept
    {
      localAddress = value;
    }
    
    /** Returns the local port. */
    inline unsigned short getLocalPort() const noexcept
    {
      return localPort;
    }
    
    /** Sets the local port. */
    inline void setLocalPort(unsigned short port) noexcept
    {
      localPort = port;
    }
    
    /** Returns the remote address. */
    inline const InetAddress& getRemoteAddress() const noexcept
    {
      return remoteAddress;
    }
    
    /** Sets the remote address. */
    inline void setRemoteAddress(const InetAddress& value) noexcept
    {
      remoteAddress = value;
    }
    
    /** Returns the remote port. */
    inline unsigned short getRemotePort() const noexcept
    {
      return remotePort;
    }
    
    /** Sets the remote port. */
    inline void setRemotePort(unsigned short port) noexcept
    {
      remotePort = port;
    }
    
    /** Returns true if the socket is valid. */
    inline bool isValid() const noexcept
    {
      return getHandle() != OperatingSystem::INVALID_HANDLE;
    }
    
    /** Returns true if socket is connected. */
    inline bool isConnected() const noexcept
    {
      return getRemotePort() != 0;
    }
    
    /** Returns true if socket is bound. */
    inline bool isBound() const noexcept
    {
      return getLocalPort() != 0;
    }
    
    /** Releases the resources use by the socket. */
    ~SocketImpl();
  };
protected:
  
  /** The internal socket representation. */
  Reference<SocketImpl> socket;

  /** Socket options. */
  enum {
    OPTION_DONT_ROUTE,
    OPTION_TCP_NO_DELAY,
    OPTION_TCP_DEFER_ACCEPT
  };
  
  /**
    Returns the handle of the socket.
  */
  inline OperatingSystem::Handle getHandle() const noexcept {
    return socket->getHandle();
  }
  
  /** Get boolean socket option. */
  bool getBooleanOption(int option) const;

  /** Set boolean socket option. */
  void setBooleanOption(int option, bool value);
public:

  /**
    Initializes an invalidated socket object (ie. unconnected and unbound).
  */
  Socket() noexcept;

  /**
    Returns a reference to the socket.
  */
  inline Reference<ReferenceCountedObject> getReference() noexcept
  {
    return socket;
  }
  
  /**
    Initialization of socket from other socket.
  */
  inline Socket(const Socket& copy) noexcept
    : socket(copy.socket)
  {
  }

  /**
    Assignment of socket to socket.
  */
  inline Socket& operator=(const Socket& assign) noexcept
                                        {
    if (&assign != this) { // protect against self assignment
      socket = assign.socket;
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
  bool accept(Socket& socket);

  /**
    Associates a local name (address and port) with this socket.

    @param address The IP address the socket should be bound to. If unspecified
    the assigned address may not be known until the socket has been connected
    with connect or accept.
    @param port The port the socket should be bound to. If zero the socket is
    assigned to a unique port.
  */
  void bind(
    const InetAddress& address, unsigned short port);

  /**
    Closes this socket.
  */
  void close();

  /**
    Connects this socket to the specified address and port.

    @param address The IP address to connect to.
    @param port The port to connect to.
  */
  void connect(const InetAddress& address, unsigned  short port);

  /**
    Creates either a stream or a datagram socket.

    @param kind The socket type (e.g. STREAM).
    @param domain The domain (the default is DEFAULT_DOMAIN).
  */
  void create(Kind kind, Domain domain = DEFAULT_DOMAIN);

  /**
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful accept or connect to determine the
    locally assigned address and port if unspecified.
  */
  void getName() noexcept;

  /**
    Returns the IP address to which the socket is connected.
  */
  const InetAddress& getAddress() const noexcept;

  /**
    Returns the remote port to which the socket is connected.
  */
  unsigned short getPort() const noexcept;

  /**
    Returns the local IP address to which the socket is bound.
  */
  const InetAddress& getLocalAddress() const noexcept;

  /**
    Returns the local port to which the socket is bound.
  */
  unsigned short getLocalPort() const noexcept;

  /**
    This function places the socket in a state where it is listening for
    incoming connections. It also sets the maximum length of the queue of
    pending connections. The backlog argument may be silently reduced. The
    socket must be bound and unconnected.

    @param backlog The maxium length of the queue.
  */
  void listen(unsigned int backlog);

  /**
    Disables the input stream for this socket.
  */
  void shutdownInputStream();

  /**
    Disables the output stream for this socket.
  */
  void shutdownOutputStream();

  /**
    Returns the error state of the socket.
  */
  int getErrorState() const;
  
  /**
    Returns true if 'bind' allows local addresses to be reused.
  */
  bool getReuseAddress() const;

  /**
    Sets the local address reuse flag of this socket.
  */
  void setReuseAddress(bool value);

  /**
    Returns true if connection is kept alive.
  */
  bool getKeepAlive() const;

  /**
    Sets the keep alive flag of this socket.
  */
  void setKeepAlive(bool value);

  /**
    Returns true if broadcast datagrams allowed on this socket.
  */
  bool getBroadcast() const;

  /**
    Sets the broadcast datagram flag of this socket.
  */
  void setBroadcast(bool value);

  /**
    Gets the linger interval.

    @return -1 if linger is disabled.
  */
  int getLinger() const;

  /**
    Sets the linger interval. Negative time disables the linger.
  */
  void setLinger(int seconds);

  /**
    Gets the size of the receive buffer.
  */
  int getReceiveBufferSize() const;

  /**
    Sets the size of the receive buffer.
  */
  void setReceiveBufferSize(int size);

  /**
    Gets the size of the send buffer.
  */
  int getSendBufferSize() const;

  /**
    Sets the size of the send buffer.
  */
  void setSendBufferSize(int size);

  /**
    Returns true if the standard routing facilities have been disabled.
  */
  bool getDontRoute() const;

  /**
    Disables/enables the standard routing facilities.
  */
  void setDontRoute(bool value);

  /**
    Returns the timeout period in nanoseconds for receive.
  */
  uint64 getReceiveTimeout() const;

  /**
    Sets the timeout period for receive (clamped to one day).
  */
  void setReceiveTimeout(uint64 nanoseconds);

  /**
    Returns the timeout period in nanoseconds for send.
  */
  uint64 getSendTimeout() const;

  /**
    Sets the timeout period for send (clamped to one day).
  */
  void setSendTimeout(uint64 nanoseconds);
  
  /**
    Returns true if the Nagle's algorithm is disabled.
  */
  bool getTcpNoDelay() const;

  /**
    Disables/enables the Nagle's algorithm.
  */
  void setTcpNoDelay(bool value);

  /**
    Returns the timeout period in nanoseconds for send.
  */
  uint64 getTcpDeferAccept() const;

  /**
    Sets the timeout period for send (clamped to one day).
  */
  void setTcpDeferAccept(uint64 nanoseconds);
  
  /**
    Returns the current time to live (TTL) value.
  */
  unsigned int getTimeToLive() const;
  
  /**
    Sets the time to live (TTL) value.
  */
  void setTimeToLive(unsigned int value);

  /**
    Returns the maximum number of multicast hops (time to live).
  */
  uint8 getMulticastHops() const;
  
  /**
    Sets the maximum number of multicast hops (time to live).
  */
  void setMulticastHops(uint8 value);

  /**
    Returns the multicast loop-back value.
  */
  bool getMulticastLoopback() const;

  /**
    Sets the multicast loop-back value.
  */
  void setMulticastLoopback(bool value);

  /**
    Returns the default interface for outgoing multicast packets.
  */
  InetAddress getMulticastInterface() const;
  
  /**
    Sets the default interface for outgoing multicast packets.
  */
  void setMulticastInterface(
    const InetAddress& interface);

  /**
    Returns the maximum number of unicast hops (time to live).
  */
  uint8 getUnicastHops() const;
  
  /**
    Sets the maximum number of unicast hops (time to live).
  */
  void setUnicastHops(uint8 value);
  
  /**
    Joins the specified multicast group on the default interface.
  */
  void joinGroup(const InetAddress& group);
  
  /**
    Joins the specified multicast group on the specified interface.
  */
  void joinGroup(
    const InetAddress& interface,
    const InetAddress& group);

  /**
    Leaves the specified multicast group of the specified interface.
  */
  void leaveGroup(
    const InetAddress& interface,
    const InetAddress& group);

  /**
    Returns the IPv6 packet restriction flag.
  */
  bool getIPv6Restriction() const;

  /**
    Sets the IPv6 packet restriction flag.
  */
  void setIPv6Restriction(bool value);
  
  /**
    Sets the blocking mode of the socket.
  */
  void setNonBlocking(bool value);

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const;

  /**
    Returns true if the socket is valid.
  */
  inline bool isValid() const noexcept
  {
    return socket->isValid();
  }
  
  /**
    Forces any buffered bytes to be written out.
  */
  void flush();

  /**
    Fills the buffer with bytes from the socket input stream. Blocks if asked
    to read more bytes than available. Raises EndOfFile if end of stream has
    been reached.
    
    @param buffer The buffer.
    @param size The size of the buffer.
    @param nonblocking Select nonblocking mode.
    @return The actual number of bytes read.
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Select nonblocking mode.
    @return The actual number of bytes written.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Sends the contents of the buffer to the specified address using an
    unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The Internet address of the remote host.
    @param port The port of the remote host.
    @return The number of bytes sent.
  */
  unsigned int sendTo(
    const uint8* buffer,
    unsigned int size,
    const InetAddress& address,
    unsigned short port);

  /**
    Receives data from any address using an unconnected socket.

    @param buffer The buffer.
    @param size The size of the buffer.
    @param address The address.
    @param port The port.

    @return The number of bytes received.
  */
  unsigned int receiveFrom(
    uint8* buffer,
    unsigned int size,
    InetAddress& address,
    unsigned short& port);

  void asyncCancel();
  
  AsynchronousReadOperation read(
    uint8* buffer,
    unsigned int bytesToRead,
    AsynchronousReadEventListener* listener);

  AsynchronousWriteOperation write(
    const uint8* buffer,
    unsigned int bytesToWrite,
    AsynchronousWriteEventListener* listener);
  
  /**
    Blocking wait for input to become available.
  */
  void wait() const;

  /**
    Blocking wait for input to become available.

    @param microseconds The timeout periode in microseconds.
    
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int microseconds) const;

  /**
    Releases the socket.
  */
  ~Socket();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
