/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

inline void initIPv6SocketAddress(struct sockaddr_in6& socketAddress, const InetAddress& addr, unsigned short port) throw() {
  memset(&socketAddress, 0, sizeof(socketAddress)); // clear structure
#ifdef SIN6_LEN
  socketAddress.sin6_len = sizeof(socketAddress);
#endif
  socketAddress.sin6_family = AF_INET6;
  socketAddress.sin6_port = htons(port);
  memcpy(&socketAddress.sin6_addr, addr.getAddress(), sizeof(struct in6_addr)); // copy address
}

Socket::Socket() throw() {
  remotePort = 0;
  localPort = 0;
//  inputStream = NULL;
//  outputStream = NULL;
}

bool Socket::accept(Socket& socket) throw(IOException) {
  SynchronizeExclusively();

  if (isCreated()) {
//    close(); // alternative
//  setHandle(-1);
    SynchronizeRelease();
    throw NetworkException("Attempt to overwrite socket");
  }

  // don't know if accept() fills 'sa' with something different from sockaddr_in6 - do you know this
  int handle;
  struct sockaddr_in6 sa;
  socklen_t sl = sizeof(sa);

  if ((handle = ::accept(socket.getHandle(), (struct sockaddr*)&sa, &sl)) != 0) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      SynchronizeRelease();
      throw NetworkException("Unable to accept connection");
    }
  }

  setHandle(handle);
  remoteAddress.setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  remotePort = ntohs(sa.sin6_port);
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();

  struct sockaddr_in6 socketAddress;
  initIPv6SocketAddress(socketAddress, addr, port);

  if (::bind(getHandle(), (struct sockaddr*)&socketAddress, sizeof(socketAddress)) != 0) {
    SynchronizeRelease();
    throw NetworkException("Unable to assign name to socket");
  }

  localAddress = addr;
  localPort = port;
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();

  if (::close(getHandle()) != 0) {
    SynchronizeRelease();
    throw NetworkException("Unable to close socket");
  }
  setHandle(-1); // invalidate socket handle
  remotePort = 0; // make unconnected
  localPort = 0; // make unbound
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();

  struct sockaddr_in6 socketAddress;
  initIPv6SocketAddress(socketAddress, addr, port);

  if (::connect(getHandle(), (struct sockaddr*)&socketAddress, sizeof(socketAddress)) != 0) {
    switch (errno) {
    case ECONNREFUSED:
//      throw ConnectionRefused("Connection refused");
    case ETIMEDOUT:
//      throw TimeOut("Connection request timed out");
    default:
      SynchronizeRelease();
      throw NetworkException("Unable to connect to socket");
    }
  }
  remoteAddress = addr;
  remotePort = port;
}

void Socket::create(bool stream) throw(IOException) {
  SynchronizeExclusively();
  int handle;
  if (isCreated() || ((handle = ::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)) {
    SynchronizeRelease();
    throw NetworkException("Unable to create socket");
  }
  setHandle(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  // synchronization not required
  if (backlog > INT_MAX) { // does backlog fit in 'int' type
    backlog = INT_MAX; // silently reduce the backlog argument
  }
  if (::listen(getHandle(), backlog) != 0) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections");
  }
}

InetAddress Socket::getAddress() const throw() {
  SynchronizeShared();
  return remoteAddress;
}

unsigned short Socket::getPort() const throw() {
  return remotePort;
}

InetAddress Socket::getLocalAddress() const throw() {
  SynchronizeShared();
  return localAddress;
}

unsigned short Socket::getLocalPort() const throw() {
  return localPort;
}

InputStream Socket::getInputStream() {
  SynchronizeExclusively();
  return inputStream;
}

void Socket::shutdownInputStream() throw(IOException) {
  SynchronizeExclusively();
  if (::shutdown(getHandle(), 0)) { // disallow further receives
    SynchronizeRelease();
    throw IOException()
  }
}

OutputStream Socket::getOutputStream() {
  SynchronizeExclusively();
  return outputStream;
}

void Socket::shutdownOutputStream() throw(IOException) {
  SynchronizeExclusively();
  if (::shutdown(getHandle(), 1)) { // disallow further sends
    SynchronizeRelease();
    throw IOException()
  }
}

Socket::~Socket() throw(IOException) {
}

void Socket::getOption(int option, void* buffer, socklen_t* len) const throw(IOException) {
  // getsockopt is MT-safe
  if (::getsockopt(getHandle(), SOL_SOCKET, option, buffer, len) != 0) {
    throw IOException("Unable to get socket option");
  }
}

void Socket::setOption(int option, const void* buffer, socklen_t len) throw(IOException) {
  // setsockopt is MT-safe
  if (::setsockopt(getHandle(), SOL_SOCKET, option, buffer, len) != 0) {
    throw IOException("Unable to set socket option");
  }
}

bool Socket::getBooleanOption(int option) const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getOption(option, &buffer, &len);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
  int buffer = value;
  setOption(option, &buffer, sizeof(buffer));
}

bool Socket::getReuseAddress() const throw(IOException) {
  return getBooleanOption(SO_REUSEADDR);
}

void Socket::setReuseAddress(bool value) throw(IOException) {
  setBooleanOption(SO_REUSEADDR, value);
}

bool Socket::getKeepAlive() const throw(IOException) {
  return getBooleanOption(SO_KEEPALIVE);
}

void Socket::setKeepAlive(bool value) throw(IOException) {
  setBooleanOption(SO_KEEPALIVE, value);
}

bool Socket::getBroadcast() const throw(IOException) {
  return getBooleanOption(SO_BROADCAST);
}

void Socket::setBroadcast(bool value) throw(IOException) {
  setBooleanOption(SO_BROADCAST, value);
}

int Socket::getLinger() const throw(IOException) {
  struct linger buffer;
  socklen_t len = sizeof(buffer);
  getOption(SO_LINGER, &buffer, &len);
  return (buffer.l_onoff != 0) ? buffer.l_linger : -1;
}

void Socket::setLinger(int seconds) throw(IOException) {
  struct linger buffer;
  if (seconds < 0) {
    buffer.l_onoff = 0; // disable linger
  } else {
    buffer.l_onoff = 1; // enable linger
    buffer.l_linger = seconds;
  }
  setOption(SO_LINGER, &buffer, sizeof(buffer));
}

int Socket::getReceiveBufferSize() const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getOption(SO_RCVBUF, &buffer, &len);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) const throw(IOException) {
  int buffer = size;
  setOption(SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getOption(SO_SNDBUF, &buffer, &len);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  int buffer = size;
  setOption(SO_SNDBUF, &buffer, sizeof(buffer));
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value) {
  stream << "{";
  stream << "connected to=";
  if (value.isConnected()) {
    stream << "{address=" << value.getAddress() << ",port="<< value.getPort() << "}";
  } else {
    stream << "UNCONNECTED";
  }
  stream << "bound to=";
  if (value.isBound()) {
    stream << "{address=" << value.getLocalAddress() << ",port="<< value.getLocalPort() << "}";
  } else {
    stream << "UNBOUND";
  }
  stream << "}";
  return stream;
}
