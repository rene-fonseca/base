/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Socket.h"
#include <base/Functor.h>

#ifdef __win32__
  #include <winsock2.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <unistd.h>
  #include <errno.h>
  #include <string.h>
#endif

#ifndef socklen_t
  #define socklen_t int
#endif

#ifndef INVALID_SOCKET
  #define INVALID_SOCKET -1
#endif


class SocketAddress {
private:

#ifdef HAVE_IPV6
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // HAVE_IPV6
public:

  /**
    Initializes socket address.
  */
  SocketAddress(InetAddress addr, unsigned short port) {
    fill<char>((char*)&sa, sizeof(sa), 0);
#ifdef HAVE_IPV6
#ifdef SIN6_LEN
    sa.sin6_len = sizeof(sa);
#endif // SIN6_LEN
    sa.sin6_family = AF_INET6;
    sa.sin6_port = htons(port);
    copy<char>((char*)&sa.sin6_addr, addr.getAddress(), sizeof(struct in6_addr));
#else
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    copy<char>((char*)&sa.sin_addr, addr.getAddress(), sizeof(struct in_addr));
#endif // HAVE_IPV6
  }

  /**
    Returns pointer to socket address.
  */
  const struct sockaddr* getValue() const {return (struct sockaddr*)&sa;}

  /**
    Returns the size of the socket address structure.
  */
  inline unsigned int getSize() const {return sizeof(sa);}
};



void getSocketOption(int handle, int option, void* buffer, socklen_t* len) throw(IOException) {
  // getsockopt is MT-safe
  if (::getsockopt(handle, SOL_SOCKET, option, (char*)buffer, len) != 0) {
    throw IOException("Unable to get socket option");
  }
}

void setSocketOption(int handle, int option, const void* buffer, socklen_t len) throw(IOException) {
  // setsockopt is MT-safe
  if (::setsockopt(handle, SOL_SOCKET, option, (const char*)buffer, len) != 0) {
    throw IOException("Unable to set socket option");
  }
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
//  setHandle(INVALID_SOCKET);
    SynchronizeRelease();
    throw NetworkException("Attempt to overwrite socket");
  }

  // don't know if accept() fills 'sa' with something different from sockaddr_in6 - do you know this
  int handle;
#ifdef HAVE_IPV6
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // HAVE_IPV6
  socklen_t sl = sizeof(sa);

#ifdef __win32__
#else
  if ((handle = ::accept(socket.fd.getHandle(), (struct sockaddr*)&sa, &sl)) != 0) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      SynchronizeRelease();
      throw NetworkException("Unable to accept connection");
    }
  }
#endif

  fd.setHandle(handle);
#ifdef HAVE_IPV6
  remoteAddress.setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  remotePort = ntohs(sa.sin6_port);
#else
  remoteAddress.setAddress((char*)&(sa.sin_addr), InetAddress::IPv4);
  remotePort = ntohs(sa.sin_port);
#endif // HAVE_IPV6
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  const SocketAddress sa(addr, port);
  if (::bind(fd.getHandle(), sa.getValue(), sa.getSize())) {
    throw NetworkException("Unable to associate name with socket");
  }
  localAddress = addr;
  localPort = port;
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();
#ifdef __win32__
  if (::closesocket(fd.getHandle())) {
    throw NetworkException("Unable to close socket");
  }
#else
  if (::close(fd.getHandle())) {
    throw NetworkException("Unable to close socket");
  }
#endif
  fd.setHandle(INVALID_SOCKET); // invalidate socket handle
  remotePort = 0; // make unconnected
  localPort = 0; // make unbound
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();

  SocketAddress sa(addr, port);

  if (::connect(fd.getHandle(), sa.getValue(), sa.getSize())) {
#ifdef __win32__
    switch (WSAGetLastError()) {
    case WSAECONNREFUSED:
      throw AccessDenied();
      break;
    case WSAETIMEDOUT:
      throw TimedOut();
      break;
    }
#else
    switch (errno) {
    case ECONNREFUSED: 
      throw AccessDenied();
      break;
    case ETIMEDOUT:
      throw TimedOut();
      break;
    }
#endif
    throw NetworkException("Unable to connect to socket");
  }
  remoteAddress = addr;
  remotePort = port;
}

void Socket::create(bool stream) throw(IOException) {
  SynchronizeExclusively();
  int handle;
#ifdef HAVE_IPV6
  if (isCreated() || ((handle = ::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == INVALID_SOCKET)) {
    throw NetworkException("Unable to create socket");
  }
#else
  if (isCreated() || ((handle = ::socket(PF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == INVALID_SOCKET)) {
    throw NetworkException("Unable to create socket");
  }
#endif
  fd.setHandle(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  // synchronization not required
  if (backlog > INT_MAX) { // does backlog fit in 'int' type
    backlog = INT_MAX; // silently reduce the backlog argument
  }
  if (::listen(fd.getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections");
  }
}

const InetAddress& Socket::getAddress() const throw() {
  SynchronizeShared();
  return remoteAddress;
}

unsigned short Socket::getPort() const throw() {
  return remotePort;
}

const InetAddress& Socket::getLocalAddress() const throw() {
  SynchronizeShared();
  return localAddress;
}

unsigned short Socket::getLocalPort() const throw() {
  return localPort;
}

FileDescriptorInputStream& Socket::getInputStream() const throw() {
  SynchronizeExclusively();
//  return inputStream;
}

void Socket::shutdownInputStream() throw(IOException) {
  SynchronizeExclusively();
  if (::shutdown(fd.getHandle(), 0)) { // disallow further receives
    throw IOException();
  }
}

FileDescriptorOutputStream& Socket::getOutputStream() const throw() {
  SynchronizeExclusively();
//  return &outputStream;
}

void Socket::shutdownOutputStream() throw(IOException) {
  SynchronizeExclusively();
  if (::shutdown(fd.getHandle(), 1)) { // disallow further sends
    throw IOException();
  }
}

Socket::~Socket() throw(IOException) {
}

bool Socket::getBooleanOption(int option) const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(fd.getHandle(), option, &buffer, &len);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
  int buffer = value;
  setSocketOption(fd.getHandle(), option, &buffer, sizeof(buffer));
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
  getSocketOption(fd.getHandle(), SO_LINGER, &buffer, &len);
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
  setSocketOption(fd.getHandle(), SO_LINGER, &buffer, sizeof(buffer));
}

int Socket::getReceiveBufferSize() const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(fd.getHandle(), SO_RCVBUF, &buffer, &len);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) const throw(IOException) {
  int buffer = size;
  setSocketOption(fd.getHandle(), SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(fd.getHandle(), SO_SNDBUF, &buffer, &len);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  int buffer = size;
  setSocketOption(fd.getHandle(), SO_SNDBUF, &buffer, sizeof(buffer));
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value) {
  stream << "Socket{";
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
