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

#include <base/net/Socket.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class ServerSocket;

/**
  This class implements a stream socket. A socket is an endpoint for
  communication between two hosts.
  
  @short Stream socket.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API StreamSocket : public Socket,
                                               public virtual InputStream,
                                               public virtual OutputStream {
public:

  using Socket::read;
  using Socket::write;

  /**
    Creates an unconnected stream socket.
  */
  StreamSocket();

  /**
    Creates a stream socket and connects it to the specified port at the
    specified IP address.

    @param address The IP address of the host to connect to.
    @param port The port to connect to on the host.
  */
  StreamSocket(const InetAddress& address, unsigned short port);

  /**
    Creates a stream socket and connects it to the specified port at the
    specified IP address.

    @param endPoint The end point to connect to.
  */
  StreamSocket(const InetEndPoint& endPoint);

  /**
    Creates a stream socket and connects it to the specified port at the
    specified IP address and also binds the socket to the local address and
    local port.

    @param address The IP address of the host to connect to.
    @param port The port to connect to on the host.
    @param localAddress The local IP address the socket should be bound to.
    @param localPort The local port the socket should be bound to.
  */
  StreamSocket(
    const InetAddress& address,
    unsigned short port,
    const InetAddress& localAddress,
    unsigned short localPort);
  
  /**
    Accept connect from server socket.
  */
  StreamSocket(ServerSocket& socket);

  /**
    Initialization of socket from other socket.
  */
  inline StreamSocket(const StreamSocket& copy) noexcept
    : Socket(copy)
  {
  }

  /**
    Associates a local name (address and port) with this socket.

    @param address The IP address the socket should be bound to.
    @param port The port the socket should be bound to.
  */
  inline void bind(const InetAddress& address, unsigned short port)
  {
    Socket::bind(address, port);
  }

  /**
    Closes this socket.
  */
  inline void close()
  {
    Socket::close();
  }

  /**
    Connects this socket to the specified address and port.

    @param address The IP address to connect to.
    @param port The port to connect to.
  */
  inline void connect(const InetAddress& address, unsigned short port)
  {
    Socket::connect(address, port);
  }

  /**
    Connects this socket to the specified address and port.

    @param endPoint The end-point to connect to.
  */
  inline void connect(const InetEndPoint& endPoint)
  {
    Socket::connect(endPoint);
  }

  /**
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful connect to determine the locally
    assigned address and port if unspecified.
  */
  inline void getName()
  {
    return Socket::getName();
  }
  
  /**
    Returns the IP address to which the socket is connected.
  */
  inline const InetAddress& getAddress() const noexcept
  {
    return Socket::getAddress();
  }

  /**
    Returns the remote port to which the socket is connected.
  */
  inline unsigned short getPort() const noexcept
  {
    return Socket::getPort();
  }

  /**
    Returns the remote end-point to which the socket is connected.
  */
  inline InetEndPoint getEndPoint() const noexcept
  {
    return Socket::getEndPoint();
  }

  /**
    Returns the local IP address to which the socket is bound.
  */
  inline const InetAddress& getLocalAddress() const noexcept
  {
    return Socket::getLocalAddress();
  }

  /**
    Returns the local port to which the socket is bound.
  */
  inline unsigned short getLocalPort() const noexcept
  {
    return Socket::getLocalPort();
  }

  /**
    Returns the local end-point to which the socket is connected.
  */
  inline InetEndPoint getLocalEndPoint() const noexcept
  {
    return Socket::getLocalEndPoint();
  }

  /**
    Disables the input stream for this socket.
  */
  inline void shutdownInputStream()
  {
    Socket::shutdownInputStream();
  }

  /**
    Disables the output stream for this socket.
  */
  inline void shutdownOutputStream()
  {
    Socket::shutdownOutputStream();
  }

  /**
    Returns the error state of the socket.
  */
  inline int getErrorState() const
  {
    return Socket::getErrorState();
  }
  
  /**
    Returns true if 'bind' allows local addresses to be reused.
  */
  inline bool getReuseAddress() const
  {
    return Socket::getReuseAddress();
  }

  /**
    Sets the local address reuse flag of this socket.
  */
  inline void setReuseAddress(bool value)
  {
    Socket::setReuseAddress(value);
  }

  /**
    Returns true if connection is kept alive.
  */
  inline bool getKeepAlive() const
  {
    return Socket::getKeepAlive();
  }

  /**
    Sets the keep alive flag of this socket.
  */
  inline void setKeepAlive(bool value)
  {
    Socket::setKeepAlive(value);
  }

  /**
    Gets the linger interval.

    @return -1 if linger is disabled.
  */
  inline int getLinger() const
  {
    return Socket::getLinger();
  }

  /**
    Sets the linger interval. Negative time disables the linger.
  */
  inline void setLinger(int seconds)
  {
    Socket::setLinger(seconds);
  }

  /**
    Gets the size of the receive buffer.
  */
  inline int getReceiveBufferSize() const
  {
    return Socket::getReceiveBufferSize();
  }

  /**
    Sets the size of the receive buffer.
  */
  inline void setReceiveBufferSize(int size)
  {
    Socket::setReceiveBufferSize(size);
  }

  /**
    Gets the size of the send buffer.
  */
  inline int getSendBufferSize() const
  {
    return Socket::getSendBufferSize();
  }

  /**
    Sets the size of the send buffer.
  */
  inline void setSendBufferSize(int size)
  {
    Socket::setSendBufferSize(size);
  }

  /**
    Returns true of the Nagle's algorithm is disabled.
  */
  inline bool getTcpNoDelay() const
  {
    return Socket::getTcpNoDelay();
  }

  /**
    Disables/enables the Nagle's algorithm.
  */
  inline void setTcpNoDelay(bool value)
  {
    Socket::setTcpNoDelay(value);
  }
  
  /**
    Returns the current time to live (TTL) value.
  */
  inline unsigned int getTimeToLive() const
  {
    return Socket::getTimeToLive();
  }
  
  /**
    Sets the time to live (TTL) value.
  */
  inline void setTimeToLive(unsigned int value)
  {
    Socket::setTimeToLive(value);
  }
  
  /**
    Sets the blocking mode of the socket.
  */
  inline void setNonBlocking(bool value)
  {
    Socket::setNonBlocking(value);
  }

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.
    
    @return Available number of bytes in stream.
  */
  inline unsigned int available() const
  {
    return Socket::available();
  }

  /**
    Fills the buffer with bytes from the socket input stream. Blocks if asked
    to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  inline unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false)
  {
    return Socket::read(buffer, size, nonblocking);
  }

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  inline unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false)
  {
    return Socket::write(buffer, size, nonblocking);
  }

  /**
    Blocking wait for input to become available.
  */
  inline void wait() const
  {
    Socket::wait();
  }

  /**
    Waits for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  inline bool wait(unsigned int timeout) const
  {
    return Socket::wait(timeout);
  }

  /**
    Returns true if the socket is valid.
  */
  inline operator bool() const noexcept
  {
    return Socket::operator bool();
  }

  /**
    Skips the specified number of bytes.
  */
  inline unsigned int skip(unsigned int count)
  {
    return 0;
  }

  /**
    Forces any buffered bytes to be written out.
  */
  inline void flush()
  {
    Socket::flush();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
