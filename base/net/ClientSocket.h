/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__CLIENT_SOCKET_H
#define _DK_SDU_MIP__BASE_NET__CLIENT_SOCKET_H

#include <base/net/Socket.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ServerSocket;

/**
  This class implements a client stream socket. A socket is an endpoint for
  communication between two hosts.

  @author René Møller Fonseca
  @version 1.0
*/

class StreamSocket : protected Socket, public virtual InputStream, public virtual OutputStream {
public:

  /**
    Creates an unconnected stream socket.
  */
  StreamSocket() throw(IOException);

  /**
    Creates a stream socket and connects it to the specified port at the
    specified IP address.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host.
  */
  StreamSocket(const InetAddress& addr, unsigned short port) throw(IOException);

  /**
    Creates a stream socket and connects it to the specified port at the
    specified IP address and also binds the socket to the local address and
    local port.

    @param addr The IP address of the host to connect to.
    @param port The port to connect to on the host.
    @param localAddr The local IP address the socket should be bound to.
    @param localPort The local port the socket should be bound to.
  */
  StreamSocket(const InetAddress& addr, unsigned short port, InetAddress& localAddr, unsigned short localPort) throw(IOException);

  /**
    Accept connect from
  */
  StreamSocket(ServerSocket& socket) throw(IOException);

  /**
    Initialization of socket from other socket.
  */
  inline StreamSocket(const StreamSocket& copy) throw() : Socket(copy) {}

  /**
    Associates a local name (address and port) with this socket.

    @param addr The IP address the socket should be bound to.
    @param port The port the socket should be bound to.
  */
  inline void bind(const InetAddress& addr, unsigned short port) throw(IOException) {Socket::bind(addr, port);}

  /**
    Closes this socket.
  */
  inline void close() throw(IOException) {Socket::close();}

  /**
    Connects this socket to the specified address and port.

    @param addr The IP address to connect to.
    @param port The port to connect to.
  */
  inline void connect(const InetAddress& addr, unsigned short port) throw(IOException) {Socket::connect(addr, port);}

  /**
    Returns the IP address to which the socket is connected.
  */
  inline const InetAddress& getAddress() const throw() {return Socket::getAddress();}

  /**
    Caches the locally assigned address and port of the socket. This member
    function can be used after a succesful connect to determine the locally
    assigned address and port if unspecified.
  */
  inline void getName() throw() {return Socket::getName();}

  /**
    Returns the remote port to which the socket is connected.
  */
  inline unsigned short getPort() const throw() {return Socket::getPort();}

  /**
    Returns the local IP address to which the socket is bound.
  */
  inline const InetAddress& getLocalAddress() const throw() {return Socket::getLocalAddress();}

  /**
    Returns the local port to which the socket is bound.
  */
  inline unsigned short getLocalPort() const throw() {return Socket::getLocalPort();}

  /**
    Disables the input stream for this socket.
  */
  inline void shutdownInputStream() throw(IOException) {Socket::shutdownInputStream();}

  /**
    Disables the output stream for this socket.
  */
  inline void shutdownOutputStream() throw(IOException) {Socket::shutdownOutputStream();}

  /**
    Returns true if 'bind' allows local addresses to be reused.
  */
  inline bool getReuseAddress() const throw(IOException) {return Socket::getReuseAddress();}

  /**
    Sets the local address reuse flag of this socket.
  */
  inline void setReuseAddress(bool value) throw(IOException) {Socket::setReuseAddress(value);}

  /**
    Returns true if connection is kept alive.
  */
  inline bool getKeepAlive() const throw(IOException) {return Socket::getKeepAlive();}

  /**
    Sets the keep alive flag of this socket.
  */
  inline void setKeepAlive(bool value) throw(IOException) {Socket::setKeepAlive(value);}

  /**
    Gets the linger interval.

    @return -1 if linger is disabled.
  */
  inline int getLinger() const throw() {return Socket::getLinger();}

  /**
    Sets the linger interval. Negative time disables the linger.
  */
  inline void setLinger(int seconds) throw(IOException) {Socket::setLinger(seconds);}

  /**
    Gets the size of the receive buffer.
  */
  inline int getReceiveBufferSize() const {return Socket::getReceiveBufferSize();}

  /**
    Sets the size of the receive buffer.
  */
  inline void setReceiveBufferSize(int size) throw(IOException) {Socket::setReceiveBufferSize(size);}

  /**
    Gets the size of the send buffer.
  */
  inline int getSendBufferSize() const throw(IOException) {return Socket::getSendBufferSize();}

  /**
    Sets the size of the send buffer.
  */
  inline void setSendBufferSize(int size) throw(IOException) {Socket::setSendBufferSize(size);}

  /**
    Sets the blocking mode of the socket.
  */
  inline void setNonBlocking(bool value) throw(IOException) {Socket::setNonBlocking(value);}

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  inline unsigned int available() const throw(IOException) {return Socket::available();}

  /**
    Fills the buffer with bytes from the socket input stream. Blocks if asked
    to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  inline unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(IOException) {return Socket::read(buffer, size, nonblocking);}

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  inline unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(IOException) {return Socket::write(buffer, size, nonblocking);}

  /**
    Blocking wait for input to become available.
  */
  inline void wait() const throw(IOException) {Socket::wait();}

  /**
    Waits for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  inline bool wait(unsigned int timeout) const throw(IOException) {return Socket::wait(timeout);}

  /**
    Return true if the end of the stream has been reached.
  */
  inline bool atEnd() const throw(IOException) {return Socket::atEnd();}

  /**
  */
  inline unsigned int skip(unsigned int count) throw(IOException) {return 0;}

  /**
    Forces any buffered bytes to be written out.
  */
  inline void flush() throw(IOException) {Socket::flush();}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
