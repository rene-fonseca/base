/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/net/Socket.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>

#if defined(__win32__)
  #include <winsock2.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/socket.h>
  #include <netinet/in.h> // defines ntohs...
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <sys/time.h> // defines timeval on Linux systems
  #include <stropts.h> // defines FLUSH macros
  #include <string.h> // memset (required on solaris)

  #if defined(__solaris__)
    #define BSD_COMP 1 // request BSD flags - don't known if this is ok to do
  #endif
  #include <sys/ioctl.h> // defines FIONREAD
#endif

// do we need to repair bad header file
#if defined(__solaris__)
  #define _DK_SDU_MIP__BASE__SOCKET_BIND __xnet_bind
  #define _DK_SDU_MIP__BASE__SOCKET_CONNECT __xnet_connect
  #define _DK_SDU_MIP__BASE__SOCKET_RECVMSG __xnet_recvmsg
  #define _DK_SDU_MIP__BASE__SOCKET_SENDMSG __xnet_sendmsg
  #define _DK_SDU_MIP__BASE__SOCKET_SENDTO __xnet_sendto
  #define _DK_SDU_MIP__BASE__SOCKET_SOCKET __xnet_socket

  #undef bind
  #undef connect
  #undef recvmsg
  #undef sendmsg
  #undef sendto
  #undef socket

  inline int bind(int s, const struct sockaddr* name, int namelen) {
    return _DK_SDU_MIP__BASE__SOCKET_BIND(s, name, namelen);
  }

  inline int connect(int s, const struct sockaddr* name, int namelen) {
    return _DK_SDU_MIP__BASE__SOCKET_CONNECT(s, name, namelen);
  }

  inline ssize_t recvmsg(int s, struct msghdr* msg, int flags) {
    return _DK_SDU_MIP__BASE__SOCKET_RECVMSG(s, msg, flags);
  }

  inline ssize_t sendmsg(int s, const struct msghdr* msg, int flags) {
    return _DK_SDU_MIP__BASE__SOCKET_SENDMSG(s, msg, flags);
  }

  inline ssize_t sendto(int s, const void* msg, size_t len, int flags, const struct sockaddr* to, int tolen) {
    return _DK_SDU_MIP__BASE__SOCKET_SENDTO(s, msg, len, flags, to, tolen);
  }

  inline int socket(int domain, int type, int protocol) {
    return _DK_SDU_MIP__BASE__SOCKET_SOCKET(domain, type, protocol);
  }
#endif

#if !defined(_DK_SDU_MIP__BASE__SOCKLEN_T)
  typedef int socklen_t;
#endif

/*
  __win32__ specific:
    SOCKET is compatible with int (it is really an unsigned int).
    INVALID_SOCKET is equal to int(-1) (it is defined as unsigned int(~0)).

  I have decided not to use these definitions to avoid a platform specific Socket
  interface. Is this going to be a problem?
*/

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class SocketAddress {
private:

#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // _DK_SDU_MIP__BASE__INET_IPV6
public:

  /**
    Initializes socket address.
  */
  SocketAddress(InetAddress addr, unsigned short port) throw() {
    fill<char>((char*)&sa, sizeof(sa), 0);
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
//    switch (addr.getFamily()) {
//    case IPv4:
//      sa.sin_family = AF_INET;
//      sa.sin_port = htons(port);
//   #error PROBLEM HERE address not in beginning or what??
//      copy<char>((char*)&sa.sin_addr, addr.getAddress(), sizeof(struct in_addr));
//      break;
//    case IPv6:
#if defined(SIN6_LEN)
      sa.sin6_len = sizeof(sa);
#endif // SIN6_LEN
      sa.sin6_family = AF_INET6;
      sa.sin6_port = htons(port);
      copy<char>((char*)&sa.sin6_addr, addr.getAddress(), sizeof(struct in6_addr));
//      break;
//    }
#else
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    copy<char>((char*)&sa.sin_addr, addr.getAddress(), sizeof(struct in_addr));
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  }

  /**
    Returns pointer to socket address.
  */
  struct sockaddr* getValue() throw() {return (struct sockaddr*)&sa;}

  /**
    Returns pointer to socket address.
  */
  const struct sockaddr* getValue() const throw() {return (struct sockaddr*)&sa;}

  /**
    Returns the size of the socket address structure.
  */
  inline unsigned int getSize() const throw() {return sizeof(sa);}
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



Socket::SocketImpl::SocketImpl() throw() :  handle(-1), remotePort(0), localPort(0), end(false) {
}

Socket::SocketImpl::SocketImpl(int handle) throw() :  handle(handle), remotePort(0), localPort(0), end(false) {
}

Socket::SocketImpl::~SocketImpl() throw(IOException) {
  if (handle != -1) {
#if defined(__win32__)
    if (::closesocket(getHandle())) {
      throw NetworkException("Unable to close socket");
    }
#else // __unix__
    if (::close(getHandle())) {
      throw NetworkException("Unable to close socket");
    }
#endif
  }
}



Socket::Socket() throw() : socket(new SocketImpl()) {
}

bool Socket::accept(Socket& socket) throw(IOException) {
  //  SynchronizeExclusively();

  if (this->socket->isCreated()) {
    throw NetworkException("Attempt to overwrite socket");
  }

  // don't know if accept() fills 'sa' with something different from sockaddr_in6 - do you know this
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  socklen_t sl = sizeof(sa);

  int handle = ::accept(socket.getHandle(), (struct sockaddr*)&sa, &sl);
#if defined(__win32__)
  if (handle == -1) {
    switch (WSAGetLastError()) {
    case WSAEWOULDBLOCK:
      return false;
    default:
      throw NetworkException("Unable to accept connection");
    }
  }
#else // __unix__
  if (handle == -1) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      throw NetworkException("Unable to accept connection");
    }
  }
#endif

  this->socket = new SocketImpl(handle);
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  this->socket->getRemoteAddress()->setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  this->socket->setRemotePort(ntohs(sa.sin6_port));
#else
  this->socket->getRemoteAddress()->setAddress((char*)&(sa.sin_addr), InetAddress::IPv4);
  this->socket->setRemotePort(ntohs(sa.sin_port));
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  SocketAddress sa(addr, port);
  if (::bind(getHandle(), sa.getValue(), sa.getSize())) {
    throw NetworkException("Unable to associate name with socket");
  }
  *socket->getLocalAddress() = addr;
  socket->setLocalPort(port);
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();
  socket = new SocketImpl(); // invalidate socket
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();

  SocketAddress sa(addr, port);

  if (::connect(getHandle(), sa.getValue(), sa.getSize())) {
#if defined(__win32__)
    switch (WSAGetLastError()) {
    case WSAECONNREFUSED:
      throw AccessDenied();
    case WSAETIMEDOUT:
      throw TimedOut();
    default:
      throw NetworkException("Unable to connect to socket");
    }
#else // __unix__
    switch (errno) {
    case ECONNREFUSED:
      throw AccessDenied();
    case ETIMEDOUT:
      throw TimedOut();
    default:
      throw NetworkException("Unable to connect to socket");
    }
#endif
  }
  *socket->getRemoteAddress() = addr;
  socket->setRemotePort(port);
}

void Socket::create(bool stream) throw(IOException) {
  SynchronizeExclusively();
  int handle;
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (socket->isCreated() || ((handle = ::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)) {
    throw NetworkException("Unable to create socket");
  }
#else
  if (socket->isCreated() || ((handle = ::socket(PF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)) {
    throw NetworkException("Unable to create socket");
  }
#endif
  socket = new SocketImpl(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  SynchronizeShared();
  if (backlog > INT_MAX) { // does backlog fit in 'int' type
    backlog = INT_MAX; // silently reduce the backlog argument
  }
  if (::listen(getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections");
  }
}

const InetAddress& Socket::getAddress() const throw() {
  SynchronizeShared();
  return *socket->getRemoteAddress();
}

unsigned short Socket::getPort() const throw() {
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const throw() {
  SynchronizeShared();
  return *socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const throw() {
  SynchronizeShared();
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(IOException) {
  SynchronizeShared();
#if defined(__win32__)
  if (::shutdown(getHandle(), SD_RECEIVE)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading");
  }
#else // __unix__
  if (::shutdown(getHandle(), 0)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading");
  }
#endif
}

void Socket::shutdownOutputStream() throw(IOException) {
  SynchronizeShared();
#if defined(__win32__)
  if (::shutdown(getHandle(), SD_SEND)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing");
  }
#else // __unix__
  if (::shutdown(getHandle(), 1)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing");
  }
#endif
}

bool Socket::getBooleanOption(int option) const throw(IOException) {
  SynchronizeShared();
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), option, &buffer, &len);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
  SynchronizeShared();
  int buffer = value;
  setSocketOption(getHandle(), option, &buffer, sizeof(buffer));
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
  SynchronizeShared();
  struct linger buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), SO_LINGER, &buffer, &len);
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
  setSocketOption(getHandle(), SO_LINGER, &buffer, sizeof(buffer));
}

int Socket::getReceiveBufferSize() const throw(IOException) {
  SynchronizeShared();
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), SO_RCVBUF, &buffer, &len);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) throw(IOException) {
  SynchronizeShared();
  int buffer = size;
  setSocketOption(getHandle(), SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  SynchronizeShared();
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), SO_SNDBUF, &buffer, &len);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  SynchronizeShared();
  int buffer = size;
  setSocketOption(getHandle(), SO_SNDBUF, &buffer, sizeof(buffer));
}

void Socket::setNonBlocking(bool value) throw(IOException) {
  SynchronizeShared();
#if defined(__win32__)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket(getHandle(), FIONBIO, (u_long*)&buffer)) {
    throw IOException("Unable to set blocking mode");
  }
#else // __unix__
  int flags;
  if ((flags = fcntl(getHandle(), F_GETFL)) == -1) {
    throw IOException("Unable to get flags for socket");
  }
  if (value) {
    if (flags & O_NONBLOCK == 0) { // do we need to set flag
      flags |= O_NONBLOCK;
      if (fcntl(getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket");
      }
    }
  } else {
    if (flags & O_NONBLOCK != 0) { // do we need to clear flag
      flags &= ~O_NONBLOCK;
      if (fcntl(getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket");
      }
    }
  }
#endif
}

unsigned int Socket::available() const throw(IOException) {
  SynchronizeShared();
#if defined(__win32__)
  unsigned int result;
  if (ioctlsocket(getHandle(), FIONREAD, (u_long*)&result)) {
    throw IOException("Unable to determine the amount of data pending in the input buffer");
  }
  return result;
#else // __unix__
  // this implementation is not very portable?
  int result;
  if (ioctl(getHandle(), FIONREAD, &result)) {
    throw IOException("Unable to determine the amount of data pending in the input buffer");
  }
  return result;
#endif
}

bool Socket::atEnd() const throw() {
  SynchronizeShared();
  return socket->atEnd();
}

void Socket::flush() throw(IOException) {
}

unsigned int Socket::read(char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  SynchronizeShared();

  // TAG: currently always blocks
  assert(!socket->atEnd(), EndOfFile());
  unsigned int bytesRead = 0;
  while (bytesRead < size) {
#if defined(__win32__)
    int result = ::recv(fd->getHandle(), buffer, (size <= INT_MAX) ? size : INT_MAX, 0);
    if (result < 0) { // has an error occured
      switch (WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was read
        continue; // try again
      case WSAEWOULDBLOCK: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from socket");
      }
    }
#else // __unix__
    int result = ::recv(socket->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX, 0);
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from socket");
      }
    }
#endif
    if (result == 0) { // has end been reached
      socket->onEnd(); // remember end of file
      if (bytesRead < size) {
        throw EndOfFile(); // attempt to read beyond end of stream
      }
    }
    bytesRead += result;
  }
  return bytesRead;
}

unsigned int Socket::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
#if defined(__win32__)
    int result = ::send(getHandle(), buffer, (size <= INT_MAX) ? size : INT_MAX, 0);
    if (result < 0) { // has an error occured
      switch (WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was written
        continue; // try again
      case WSAEWOULDBLOCK: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to socket");
      }
    }
#else // __unix__
    int result = ::send(socket->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX, 0);
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to socket");
      }
    }
#endif
    bytesWritten += result;
  }
  return bytesWritten;
}

unsigned int Socket::receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException) {
  int result = 0;

#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // _DK_SDU_MIP__BASE__INET_IPV6
  socklen_t sl = sizeof(sa);

  if ((result = ::recvfrom(getHandle(),  buffer, size, 0, (struct sockaddr*)&sa, &sl)) == -1) {
    throw IOException("Unable to receive from");
  }

#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  // check if really an IPv4 address
  address.setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  port = ntohs(sa.sin6_port);
#else
  address.setAddress((char*)&(sa.sin_addr), InetAddress::IPv4);
  port = ntohs(sa.sin_port);
#endif // _DK_SDU_MIP__BASE__INET_IPV6

  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, InetAddress& address, unsigned short port) throw(IOException) {
  SynchronizeShared();
  int result = 0;
  const SocketAddress sa(address, port);
  if ((result = ::sendto(getHandle(), buffer, size, 0, sa.getValue(), sa.getSize())) == -1) {
    throw IOException("Unable to send to");
  }
  return result;
}

void Socket::wait() const throw(IOException) {
  SynchronizeShared();
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(getHandle(), &rfds);

  int result = ::select(getHandle() + 1, &rfds, NULL, NULL, NULL);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
}

bool Socket::wait(unsigned int timeout) const throw(IOException) {
  SynchronizeShared();
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(getHandle() + 1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
  return result; // return true if data available
}

Socket::~Socket() {
  TRACE_MEMBER();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Socket& value) {
  stream << "Socket{";
  stream << "connected to=";
  if (value.socket->isConnected()) {
    stream << "{address=" << value.getAddress() << ",port="<< value.getPort() << "}";
  } else {
    stream << "UNCONNECTED";
  }
  stream << "bound to=";
  if (value.socket->isBound()) {
    stream << "{address=" << value.getLocalAddress() << ",port="<< value.getLocalPort() << "}";
  } else {
    stream << "UNBOUND";
  }
  stream << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
