/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/ByteOrder.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/io/BrokenStream.h>
#include <base/net/Socket.h>
#include <base/concurrency/Thread.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  undef interface
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/socket.h>
#  include <netinet/in.h> // defines ntohs...
#  include <netinet/tcp.h> // options
#  include <net/if.h>
// #  include <sys/ioctl.h>
// #  include <sys/sockio.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <sys/time.h> // defines timeval on Linux systems

#  if (_DK_SDU_MIP__BASE__OS != _DK_SDU_MIP__BASE__CYGWIN)
#    include <stropts.h> // defines FLUSH macros
#  endif

#  include <string.h> // memset (required on solaris)
#  include <limits.h> // defines SSIZE_MAX
#  include <arpa/inet.h>

#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
#    define BSD_COMP 1 // request BSD flags - don't known if this is ok to do
#  endif
#  include <sys/ioctl.h> // defines FIONREAD
#endif // flavor

// do we need to repair bad header file
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS) && defined(bind)
#  define _DK_SDU_MIP__BASE__SOCKET_BIND __xnet_bind
#  define _DK_SDU_MIP__BASE__SOCKET_CONNECT __xnet_connect
#  define _DK_SDU_MIP__BASE__SOCKET_RECVMSG __xnet_recvmsg
#  define _DK_SDU_MIP__BASE__SOCKET_SENDMSG __xnet_sendmsg
#  define _DK_SDU_MIP__BASE__SOCKET_SENDTO __xnet_sendto
#  define _DK_SDU_MIP__BASE__SOCKET_SOCKET __xnet_socket

#  undef bind
#  undef connect
#  undef recvmsg
#  undef sendmsg
#  undef sendto
#  undef socket

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

#if ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) || !defined(_DK_SDU_MIP__BASE__SOCKLEN_T))
  typedef int socklen;
#else
  typedef socklen_t socklen;
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class SocketAddress { // Internet end point
private:

#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif
public:

  inline SocketAddress() throw() {
  }

  /** Initializes socket address. */
  SocketAddress(const InetAddress& addr, unsigned short port) throw(NetworkException) {
    clear(sa);
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
#    if (defined(SIN6_LEN))
    sa.sin6_len = sizeof(sa);
#    endif // SIN6_LEN
    sa.sin6_family = AF_INET6;
    sa.sin6_port = ByteOrder::toBigEndian<unsigned short>(port);
    if (addr.getFamily() == InetAddress::IP_VERSION_4) {
      InetAddress temp(addr);
      temp.convertToIPv6();
      copy<uint8>(Cast::getAddress(sa.sin6_addr), temp.getAddress(), sizeof(struct in6_addr));
    } else {
      copy<uint8>(Cast::getAddress(sa.sin6_addr), addr.getAddress(), sizeof(struct in6_addr));
    }
#  else // only IPv4 support
    assert(
      (addr.getFamily() == InetAddress::IP_VERSION_4) || addr.isIPv4Mapped() || addr.isUnspecified(),
      NetworkException("Address not supported")
    );
    sa.sin_family = AF_INET;
    sa.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
    copy<uint8>(Cast::getAddress(sa.sin_addr), addr.getIPv4Address(), sizeof(struct in_addr));
#  endif // _DK_SDU_MIP__BASE__INET_IPV6
  }

  /** Returns pointer to socket address. */
  inline struct sockaddr* getValue() throw() {
    return Cast::pointer<struct sockaddr*>(&sa);
  }

  /** Returns pointer to socket address. */
  inline const struct sockaddr* getValue() const throw() {
    return Cast::pointer<const struct sockaddr*>(&sa);
  }

  /** Returns the size of the socket address structure. */
  inline unsigned int getSize() const throw() {return sizeof(sa);}

  inline Socket::Domain getDomain() const throw() {
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
    return (Cast::pointer<const struct sockaddr*>(&sa)->sa_family == AF_INET6) ? Socket::IPV6 : Socket::IPV4;
#else
    return Socket::IPV4;
#endif
  }
  
  /** Returns the address. */
  inline InetAddress getAddress() const throw() {
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
    switch (sa.sin6_family) {
    case AF_INET:
      return InetAddress(
        Cast::getAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr),
        InetAddress::IP_VERSION_4
      );
    case AF_INET6:
      return InetAddress(
        Cast::getAddress(Cast::pointer<const struct sockaddr_in6*>(&sa)->sin6_addr),
        InetAddress::IP_VERSION_6
      );
    default:
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
#  else
    if (sa.sin_family == AF_INET) {
      return InetAddress(
        Cast::getAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr),
        InetAddress::IP_VERSION_4
      );
    } else {
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
#  endif
  }

  /** Returns the port. */
  inline unsigned short getPort() const throw() {
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
    switch (sa.sin6_family) {
    case AF_INET:
      return ByteOrder::fromBigEndian<unsigned short>(
        Cast::pointer<const struct sockaddr_in*>(&sa)->sin_port
      );
    case AF_INET6:
      return ByteOrder::fromBigEndian<unsigned short>(
        Cast::pointer<const struct sockaddr_in6*>(&sa)->sin6_port
      );
    default:
      return 0; // TAG: or should I throw an exception
    }
#  else
    if (sa.sin_family == AF_INET) {
      return ByteOrder::fromBigEndian<unsigned short>(
        Cast::pointer<const struct sockaddr_in*>(&sa)->sin_port
      );
    } else {
      return 0; // TAG: or should I throw an exception
    }
#  endif
  }

  /** Sets the socket name from the specified socket. */
  inline void setSocket(int handle) throw() {
    socklen length = getSize();
    ::getsockname(handle, getValue(), &length);
  }
};

namespace internal {

  class SocketImpl {
  public:

    static inline int getNativeError() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
      return ::WSAGetLastError();
#else // unix
      return errno;
#endif // flavor
    }
    
    static unsigned int getCause() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
      DWORD error = ::WSAGetLastError();
#else // unix
      int error = errno;
#endif // flavor
      switch (error) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
      case WSAEISCONN:
        return NetworkException::ALREADY_CONNECTED;
      case WSAENOTCONN:
        return NetworkException::NOT_CONNECTED;
      case WSAETIMEDOUT:
        return NetworkException::CONNECTION_TIMED_OUT;
      case WSAEADDRINUSE:
        return NetworkException::ADDRESS_IN_USE;
      case WSAEADDRNOTAVAIL:
        return NetworkException::ADDRESS_NOT_AVAILABLE;
      case WSAEPROTONOSUPPORT:
        return NetworkException::PROTOCOL_NOT_SUPPORTED;
      case WSAESOCKTNOSUPPORT:
        return NetworkException::TYPE_NOT_SUPPORTED;
      case WSAEOPNOTSUPP:
        return NetworkException::OPERATION_NOT_SUPPORTED;
      case WSAEPFNOSUPPORT: // TAG: check this
      case WSAEAFNOSUPPORT: // TAG: check this
        return NetworkException::FAMILY_NOT_SUPPORTED;
      case WSAEDESTADDRREQ:
        return NetworkException::DESTINATION_REQUIRED;
      case WSAEWOULDBLOCK:
        return NetworkException::NON_BLOCKING;
      case WSAEINPROGRESS:
      case WSAEALREADY:
        return NetworkException::IN_PROGRESS;

      // case WSAESHUTDOWN:
        
      case WSAECONNABORTED:
        return NetworkException::CONNECTION_ABORTED;
      case WSAECONNREFUSED:
        return NetworkException::CONNECTION_REFUSED;
      case WSAECONNRESET:
        return NetworkException::CONNECTION_RESET;
      case WSAEHOSTDOWN:
        return NetworkException::HOST_DOWN;
      case WSAEHOSTUNREACH:
        return NetworkException::HOST_UNREACHABLE;
      case WSAEMSGSIZE:
        return NetworkException::PREVIOUS_DATAGRAM_REJECTED;
      case WSANOTINITIALISED:
      case WSASYSNOTREADY:
      case WSAENETDOWN:
        return NetworkException::NETWORK_DOWN;
      case WSAENETRESET:
        return NetworkException::NETWORK_RESET;
      case WSAENETUNREACH:
        return NetworkException::NETWORK_UNREACHABLE;
#else // unix
      case EISCONN:
        return NetworkException::ALREADY_CONNECTED;
      case ENOTCONN:
        return NetworkException::NOT_CONNECTED;
      case ETIMEDOUT:
        return NetworkException::CONNECTION_TIMED_OUT;
      case EADDRINUSE:
        return NetworkException::ADDRESS_IN_USE;
      case EADDRNOTAVAIL:
        return NetworkException::ADDRESS_NOT_AVAILABLE;
      case EPROTONOSUPPORT:
        return NetworkException::PROTOCOL_NOT_SUPPORTED;
      case EPROTOTYPE:
        return NetworkException::TYPE_NOT_SUPPORTED;
      case EOPNOTSUPP:
        return NetworkException::OPERATION_NOT_SUPPORTED;
      case EAFNOSUPPORT:
        return NetworkException::FAMILY_NOT_SUPPORTED;
      case EDESTADDRREQ:
        return NetworkException::DESTINATION_REQUIRED;
      case ENOPROTOOPT: // TAG: option
        return NetworkException::PROTOCOL_NOT_SUPPORTED;
      case EWOULDBLOCK:
        return NetworkException::NON_BLOCKING;
      case EINPROGRESS:
        return NetworkException::IN_PROGRESS;
        
      case ECONNABORTED:
        return NetworkException::CONNECTION_ABORTED;
      case ECONNREFUSED:
        return NetworkException::CONNECTION_REFUSED;
      case ECONNRESET:
        return NetworkException::CONNECTION_RESET;
      case EHOSTDOWN:
        return NetworkException::HOST_DOWN;
      case EHOSTUNREACH:
        return NetworkException::HOST_UNREACHABLE;
      case EMSGSIZE:
        return NetworkException::PREVIOUS_DATAGRAM_REJECTED;
      case ENETDOWN:
        return NetworkException::NETWORK_DOWN;
      case ENETRESET:
        return NetworkException::NETWORK_RESET;
      case ENETUNREACH:
        return NetworkException::NETWORK_UNREACHABLE;
#endif // flavor
      default:        
        return PrimitiveTraits<unsigned int>::MAXIMUM;
      }
    }
    
    static inline void getOption(
      int handle,
      int level,
      int option,
      void* buffer,
      unsigned int* length) throw(NetworkException) {
      socklen temp = *length;
      if (::getsockopt(handle, level, option, static_cast<char*>(buffer), &temp) != 0) {
        throw bindCause(
          NetworkException("Unable to get IP option", Type::getType<Socket>()),
          getCause()
        );
      }
      *length = temp;
    }
    
    static inline void setOption(
      int handle,
      int level,
      int option,
      const void* buffer,
      unsigned int length) throw(NetworkException) {
      if (::setsockopt(handle, level, option, static_cast<const char*>(buffer), length) != 0) {
        throw bindCause(
          NetworkException("Unable to set IP option", Type::getType<Socket>()),
          getCause()
        );
      }
    }
    
    // TAG: add support for UDP options: IPPROTO_UDP
    // TAG: add support for RAW options: IPPROTO_RAW
  };
}; // end of namespace internal



Socket::SocketImpl::SocketImpl(
  OperatingSystem::Handle _handle,
  Domain _domain,
  Kind _kind) throw()
  : Handle(_handle),
    domain(_domain),
    kind(_kind),
    remotePort(0),
    localPort(0) {
}

Socket::SocketImpl::~SocketImpl() throw(IOException) {
  if (isValid()) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (::closesocket((int)getHandle())) {
      throw IOException("Unable to close socket", this);
    }
#else // unix
    if (::close((int)getHandle())) {
      throw IOException("Unable to close socket", this);
    }
#endif // flavor
  }
}



Socket::Socket() throw() : socket(SocketImpl::invalid) {
}

bool Socket::accept(Socket& socket) throw(NetworkException) {
  // TAG: should return socket (invalid if non available)
  
  if (this->socket->isValid()) {
    throw NetworkException("Attempt to overwrite socket", this);
  }

  SocketAddress sa;
  socklen sl = sa.getSize();
  OperatingSystem::Handle handle =
    (OperatingSystem::Handle)::accept((int)socket.socket->getHandle(), sa.getValue(), &sl);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (handle == OperatingSystem::INVALID_HANDLE) {
    switch (::WSAGetLastError()) {
    case WSAEWOULDBLOCK:
      return false;
    default:
      throw NetworkException("Unable to accept connection", this);
    }
  }
#else // unix
  if (handle == OperatingSystem::INVALID_HANDLE) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      throw NetworkException("Unable to accept connection", this);
    }
  }
#endif // flavor
  this->socket = new SocketImpl(handle, sa.getDomain(), Socket::STREAM);
  this->socket->setRemoteAddress(sa.getAddress());
  this->socket->setRemotePort(sa.getPort());
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(NetworkException) {
  SocketAddress sa(addr, port);
  if (::bind((int)socket->getHandle(), sa.getValue(), sa.getSize())) {
    throw NetworkException("Unable to assign name to socket", this);
  }
//  if ((addr.isUnspecified()) || (port == 0)) { // do we need to determine assigned name
//    sa.setSocket((int)socket->getHandle());
//    socket->setLocalAddress(sa.getAddress());
//    socket->setLocalPort(sa.getPort());
//  } else {
    socket->setLocalAddress(addr);
    socket->setLocalPort(port);
//  }
}

void Socket::close() throw(NetworkException) {
  socket = SocketImpl::invalid;
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(NetworkException) {
  SocketAddress sa(addr, port);
  if (::connect((int)socket->getHandle(), sa.getValue(), sa.getSize())) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    switch (::WSAGetLastError()) {
    case WSAECONNREFUSED:
      throw AccessDenied(this);
    case WSAETIMEDOUT:
      throw TimedOut(this);
    default:
      throw NetworkException("Unable to connect to socket", this);
    }
#else // unix
    switch (errno) {
    case ECONNREFUSED:
      throw AccessDenied(this);
    case ETIMEDOUT:
      throw TimedOut(this);
    default:
      throw NetworkException("Unable to connect to socket", this);
    }
#endif // flavor
  }
//  sa.setSocket((int)socket->getHandle());
//  socket->setLocalAddress(sa.getAddress());
//  socket->setLocalPort(sa.getPort());
  socket->setRemoteAddress(addr);
  socket->setRemotePort(port);
}

// TAG: support domain specification
void Socket::create(Kind kind) throw(NetworkException) {
  // TAG: should return new socket not overwrite handle (static method)
  static const int SOCKET_KINDS[] = {SOCK_STREAM, SOCK_DGRAM, SOCK_RAW};
  assert(!socket->isValid(), NetworkException("Unable to create socket", this));
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::socket(PF_INET6, SOCKET_KINDS[kind], 0);
  assert(
    handle != OperatingSystem::INVALID_HANDLE,
    NetworkException("Unable to create socket", this)
  );
  socket = new SocketImpl(handle, Socket::IPV6, kind);
#else
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::socket(PF_INET, SOCKET_KINDS[kind], 0);
  assert(
    handle != OperatingSystem::INVALID_HANDLE,
    NetworkException("Unable to create socket", this)
  );
  socket = new SocketImpl(handle, Socket::IPV4, kind);
#endif
}

void Socket::listen(unsigned int backlog) throw(NetworkException) {
  // silently reduce the backlog argument
  backlog = minimum<unsigned int>(backlog, PrimitiveTraits<int>::MAXIMUM);
  if (::listen((int)socket->getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections", this);
  }
}

void Socket::getName() throw() {
  SocketAddress sa;
  sa.setSocket((int)socket->getHandle());
  socket->setLocalAddress(sa.getAddress());
  socket->setLocalPort(sa.getPort());
}

const InetAddress& Socket::getAddress() const throw() {
  return socket->getRemoteAddress();
}

unsigned short Socket::getPort() const throw() {
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const throw() {
  return socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const throw() {
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)socket->getHandle(), SD_RECEIVE)) { // disallow further receives
    throw bindCause(
      NetworkException("Unable to shutdown socket for reading", this),
      internal::SocketImpl::getCause()
    );
  }
#else // unix
  if (::shutdown((int)socket->getHandle(), 0)) { // disallow further receives
    throw NetworkException("Unable to shutdown socket for reading", this);
  }
#endif // flavor
}

void Socket::shutdownOutputStream() throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)socket->getHandle(), SD_SEND)) { // disallow further sends
    throw NetworkException("Unable to shutdown socket for writing", this);
  }
#else // unix
  if (::shutdown((int)socket->getHandle(), 1)) { // disallow further sends
    throw NetworkException("Unable to shutdown socket for writing", this);
  }
#endif // flavor
}

bool Socket::getBooleanOption(int option) const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    option,
    &buffer,
    &length
  );
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(NetworkException) {
  int buffer = value;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    option,
    &buffer,
    sizeof(buffer)
  );
}

int Socket::getErrorState() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_ERROR,
    &buffer,
    &length
  );
  return buffer; // TAG: map to socket::ErrorState enum
}

bool Socket::getReuseAddress() const throw(NetworkException) {
  return getBooleanOption(SO_REUSEADDR);
}

void Socket::setReuseAddress(bool value) throw(NetworkException) {
  setBooleanOption(SO_REUSEADDR, value);
}

bool Socket::getKeepAlive() const throw(NetworkException) {
  return getBooleanOption(SO_KEEPALIVE);
}

void Socket::setKeepAlive(bool value) throw(NetworkException) {
  setBooleanOption(SO_KEEPALIVE, value);
}

bool Socket::getBroadcast() const throw(NetworkException) {
  return getBooleanOption(SO_BROADCAST);
}

void Socket::setBroadcast(bool value) throw(NetworkException) {
  setBooleanOption(SO_BROADCAST, value);
}

int Socket::getLinger() const throw(NetworkException) {
  struct linger buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)socket->getHandle(), SOL_SOCKET, SO_LINGER, &buffer, &length);
  return (buffer.l_onoff != 0) ? buffer.l_linger : -1;
}

void Socket::setLinger(int seconds) throw(NetworkException) {
  struct linger buffer;
  if (seconds < 0) {
    buffer.l_onoff = 0; // disable linger
  } else {
    buffer.l_onoff = 1; // enable linger
    buffer.l_linger = seconds;
  }
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_LINGER,
    &buffer,
    sizeof(buffer)
  );
}

int Socket::getReceiveBufferSize() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVBUF,
    &buffer,
    &length
  );
  return buffer;
}

void Socket::setReceiveBufferSize(int size) throw(NetworkException) {
  int buffer = size;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVBUF,
    &buffer,
    sizeof(buffer)
  );
}

int Socket::getSendBufferSize() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)socket->getHandle(), SOL_SOCKET, SO_SNDBUF, &buffer, &length);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(NetworkException) {
  int buffer = size;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDBUF,
    &buffer,
    sizeof(buffer)
  );
}

// TAG: TCP_MAXSEG maximum segment size
// TAG: TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT interval between keepalives
// TAG: microsoft specific datagram options: SO_MAXDG, SO_MAXPATHDG
// TAG: time out options: SO_SNDTIMEO, SO_RCVTIMEO
// TAG: connection time (MS specific?): SO_CONNECT_TIME (0xffffffff is not connected)
// TAG: max backlog queue length: SOMAXCONN

bool Socket::getDontRoute() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_DONTROUTE,
    &buffer,
    &length
  );
  return buffer != 0;
}

void Socket::setDontRoute(bool value) throw(NetworkException) {
  int buffer = value;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_DONTROUTE,
    &buffer,
    sizeof(buffer)
  );
}

uint64 Socket::getReceiveTimeout() const throw(NetworkException) {
  struct timeval buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVTIMEO,
    &buffer,
    &length
  );
  return (static_cast<uint64>(buffer.tv_sec) * 1000000 + buffer.tv_usec) * 1000;
}

void Socket::setReceiveTimeout(uint64 nanoseconds) throw(NetworkException) {
  struct timeval buffer;
  if (nanoseconds <= 60*60*24*1000000000ULL) { // one day
    buffer.tv_sec = (nanoseconds + 999)/1000000000;
    buffer.tv_usec = (nanoseconds + 999)/1000;
  } else {
    buffer.tv_sec = 60*60*24;
    buffer.tv_usec = 0;
  }
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVTIMEO,
    &buffer,
    sizeof(buffer)
  );
}

uint64 Socket::getSendTimeout() const throw(NetworkException) {
  struct timeval buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDTIMEO,
    &buffer,
    &length
  );
  return (static_cast<uint64>(buffer.tv_sec) * 1000000 + buffer.tv_usec) * 1000;
}

void Socket::setSendTimeout(uint64 nanoseconds) throw(NetworkException) {
  struct timeval buffer;
  if (nanoseconds <= 60*60*24*1000000000ULL) { // one day
    buffer.tv_sec = (nanoseconds + 999)/1000000000;
    buffer.tv_usec = (nanoseconds + 999)/1000;
  } else {
    buffer.tv_sec = 60*60*24;
    buffer.tv_usec = 0;
  }
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDTIMEO,
    &buffer,
    sizeof(buffer)
  );
}

bool Socket::getTcpNoDelay() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    IPPROTO_TCP,
    TCP_NODELAY,
    &buffer,
    &length
  );
  return buffer != 0;
}

void Socket::setTcpNoDelay(bool value) throw(NetworkException) {
  int buffer = value;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_TCP,
    TCP_NODELAY,
    &buffer,
    sizeof(buffer)
  );
}

unsigned int Socket::getTimeToLive() const throw(NetworkException) {
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_TTL,
    &buffer,
    &length
  );
  return buffer != 0;
}

void Socket::setTimeToLive(unsigned int value) throw(NetworkException) {
  int buffer = value;
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_TTL,
    &buffer,
    sizeof(buffer)
  );
}

uint8 Socket::getMulticastHops() const throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_HOPS,
      &buffer,
      &length
    );
    return buffer;
  } else {
#endif
    u_char buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_TTL,
      &buffer,
      &length
    );
    return buffer;
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

void Socket::setMulticastHops(uint8 value) throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = value;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_HOPS,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    u_char buffer = value;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_TTL,
      &buffer,
      sizeof(buffer)
    );
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

bool Socket::getMulticastLoopback() const throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    u_char buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_LOOP,
      &buffer,
      &length
    );
    return buffer != 0;
  } else {
#endif
    u_char buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_LOOP,
      &buffer,
      &length
    );
    return buffer != 0;
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

void Socket::setMulticastLoopback(bool value) throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = value;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_LOOP,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    u_char buffer = value;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_LOOP,
      &buffer,
      sizeof(buffer)
    );
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

InetAddress Socket::getMulticastInterface() const throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_IF,
      &buffer,
      &length
    );
    // TAG: fixme - resolve index
    return InetAddress();
  } else {
    int buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_IF,
      &buffer,
      &length
    );
    return InetAddress(Cast::getAddress(buffer), InetAddress::IP_VERSION_4);
  }
#else
  u_char buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_MULTICAST_IF,
    &buffer,
    &length
  );
  return InetAddress(Cast::getAddress(buffer), InetAddress::IP_VERSION_4);
#endif
}

void Socket::setMulticastInterface(const InetAddress& interface) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  InetAddress i = interface;
  struct in_addr buffer;
  assert(i.convertToIPv4(), NetworkException(this));
  copy<uint8>(Cast::getAddress(buffer), i.getIPv4Address(), sizeof(buffer));
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_MULTICAST_IF,
    &buffer,
    sizeof(buffer)
  );
  // TAG: optval = 0 for selecting default interface
#endif
}

uint8 Socket::getUnicastHops() const throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_UNICAST_HOPS,
      &buffer,
      &length
    );
    return buffer;
  } else {
#endif
    throw bindCause(
      NetworkException(this),
      NetworkException::OPERATION_NOT_SUPPORTED
    );
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

void Socket::setUnicastHops(uint8 value) throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_UNICAST_HOPS,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    throw bindCause(
      NetworkException(this),
      NetworkException::OPERATION_NOT_SUPPORTED
    );
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  }
#endif
}

void Socket::joinGroup(const InetAddress& group) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  InetAddress g = group;
  assert(g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  clear(mreq.imr_interface.s_addr); // unspecified
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    struct ipv6_mreq mreq;
    mreq.ipv6mr_interface = 0; // use default interface
    copy<uint8>(
      Cast::getAddress(mreq.ipv6mr_multiaddr),
      group.getAddress(), // if IPv4 we get error
      sizeof(mreq.ipv6mr_multiaddr)
    );
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_JOIN_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else { // IPv4
    InetAddress g = group;
    assert(g.convertToIPv4(), NetworkException(this));
    struct ip_mreq mreq;
    copy<uint8>(
      Cast::getAddress(mreq.imr_multiaddr.s_addr),
      g.getIPv4Address(),
      sizeof(mreq.imr_multiaddr.s_addr)
    );
    clear(mreq.imr_interface.s_addr); // unspecified
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_ADD_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#  else
  InetAddress i = interface;
  InetAddress g = group;
  assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  copy<uint8>(
    Cast::getAddress(mreq.imr_interface.s_addr),
    i.getIPv4Address(),
    sizeof(mreq.imr_interface.s_addr)
  );
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

void Socket::joinGroup(const InetAddress& interface, const InetAddress& group) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  InetAddress i = interface;
  InetAddress g = group;
  assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  copy<uint8>(
    Cast::getAddress(mreq.imr_interface.s_addr),
    i.getIPv4Address(),
    sizeof(mreq.imr_interface.s_addr)
  );
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    struct ipv6_mreq mreq;
    mreq.ipv6mr_interface = 0;
    if (!interface.isUnspecified()) {
      struct ifconf ifc;
      ifc.ifc_len = Thread::getLocalStorage()->getSize();
      ifc.ifc_buf = Thread::getLocalStorage()->getElements();
      if (ioctl((int)socket->getHandle(), SIOCGIFCONF, &ifc)) {
        throw NetworkException("Unable to resolve interface", this);
      }
      const struct ifreq* current = ifc.ifc_req;
      int offset = 0;
      while (offset < ifc.ifc_len) {
        if (ioctl((int)socket->getHandle(), SIOCGIFADDR, current) != 0) {
          continue;
        }
        bool isSynonymous = false;
        if (current->ifr_addr.sa_family == AF_INET6) {
          isSynonymous = interface.isSynonymous(
            InetAddress(
              Cast::getAddress(
                Cast::pointer<const struct sockaddr_in6*>(&current->ifr_addr)->sin6_addr
              ),
              InetAddress::IP_VERSION_6
            )
          );
        } else { // AF_INET
          isSynonymous = interface.isSynonymous(
            InetAddress(
              Cast::getAddress(
                Cast::pointer<const struct sockaddr_in*>(&current->ifr_addr)->sin_addr
              ),
              InetAddress::IP_VERSION_4
            )
          );
        }
        if (isSynonymous) {
          // TAG: is name always null terminated
          mreq.ipv6mr_interface = if_nametoindex(current->ifr_name);
          break; // always exit loop
        }
        ++current;
        offset += sizeof(*current);
      }
      assert(
        mreq.ipv6mr_interface > 0,
        NetworkException("Unable to resolve interface", this)
      );
    }
    copy<uint8>(
      Cast::getAddress(mreq.ipv6mr_multiaddr),
      group.getAddress(), // if IPv4 we get error
      sizeof(mreq.ipv6mr_multiaddr)
    );
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_JOIN_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else { // IPv4
    InetAddress i = interface;
    InetAddress g = group;
    assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
    struct ip_mreq mreq;
    copy<uint8>(
      Cast::getAddress(mreq.imr_multiaddr.s_addr),
      g.getIPv4Address(),
      sizeof(mreq.imr_multiaddr.s_addr)
    );
    copy<uint8>(
      Cast::getAddress(mreq.imr_interface.s_addr),
      i.getIPv4Address(),
      sizeof(mreq.imr_interface.s_addr)
    );
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_ADD_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#  else
  InetAddress i = interface;
  InetAddress g = group;
  assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  copy<uint8>(
    Cast::getAddress(mreq.imr_interface.s_addr),
    i.getIPv4Address(),
    sizeof(mreq.imr_interface.s_addr)
  );
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

void Socket::leaveGroup(const InetAddress& interface, const InetAddress& group) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  InetAddress i = interface;
  InetAddress g = group;
  assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  copy<uint8>(
    Cast::getAddress(mreq.imr_interface.s_addr),
    i.getIPv4Address(),
    sizeof(mreq.imr_interface.s_addr)
  );
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_DK_SDU_MIP__BASE__INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    InetAddress i = interface;
    InetAddress g = group;
    i.convertToIPv6();
    g.convertToIPv6();
    struct ipv6_mreq mreq;
    // TAG: map interface address to interface index
    mreq.ipv6mr_interface = 1; // TAG: fixme
    copy<uint8>(
      Cast::getAddress(mreq.ipv6mr_multiaddr),
      g.getAddress(),
      sizeof(mreq.ipv6mr_multiaddr)
    );    
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_LEAVE_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else {
    InetAddress i = interface;
    InetAddress g = group;
    assert(i.convertToIPv4() && g.convertToIPv4(), bindCause(NetworkException(this), 0));
    struct ip_mreq mreq;
    copy<uint8>(
      Cast::getAddress(mreq.imr_multiaddr.s_addr),
      g.getIPv4Address(),
      sizeof(mreq.imr_multiaddr.s_addr)
    );
    copy<uint8>(
      Cast::getAddress(mreq.imr_interface.s_addr),
      i.getIPv4Address(),
      sizeof(mreq.imr_interface.s_addr)
    );
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IP,
      IP_DROP_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#  else
  InetAddress i = interface;
  InetAddress g = group;
  assert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  copy<uint8>(
    Cast::getAddress(mreq.imr_interface.s_addr),
    i.getIPv4Address(),
    sizeof(mreq.imr_interface.s_addr)
  );
  internal::SocketImpl::setOption(
    (int)socket->getHandle(),
    IPPROTO_IP,
    IP_DROP_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

bool Socket::getIPv6Restriction() const throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6) && defined(IPV6_V6ONLY))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_V6ONLY,
      &buffer,
      &length
    );
    return buffer;
  } else {
#endif
    // IPv6 restriction is not possible if only IPv4 is supported
    return false;
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6) && defined(IPV6_V6ONLY))
  }
#endif
}

void Socket::setIPv6Restriction(bool value) throw(NetworkException) {
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6) && defined(IPV6_V6ONLY))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer;
    internal::SocketImpl::setOption(
      (int)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_V6ONLY,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    throw bindCause(
      NetworkException(this),
      NetworkException::OPERATION_NOT_SUPPORTED
    );
#if (defined(_DK_SDU_MIP__BASE__INET_IPV6) && defined(IPV6_V6ONLY))
  }
#endif
}

void Socket::setNonBlocking(bool value) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket((int)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
    throw NetworkException("Unable to set blocking mode", this);
  }
#else // unix
  int flags;
  if ((flags = fcntl((int)socket->getHandle(), F_GETFL)) == -1) {
    throw NetworkException("Unable to get flags for socket", this);
  }
  if (value) {
    if ((flags & O_NONBLOCK) == 0) { // do we need to set flag
      flags |= O_NONBLOCK;
      if (fcntl((int)socket->getHandle(), F_SETFL, flags) != 0) {
        throw NetworkException("Unable to set flags of socket", this);
      }
    }
  } else {
    if ((flags & O_NONBLOCK) != 0) { // do we need to clear flag
      flags &= ~O_NONBLOCK;
      if (fcntl((int)socket->getHandle(), F_SETFL, flags) != 0) {
        throw NetworkException("Unable to set flags of socket", this);
      }
    }
  }
#endif // flavor
}

#if 0
bool Socket::getAsynchronous() throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((int)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     throw NetworkException("Unable to set blocking mode", this);
//   }
#else // unix
  int flags;
  if ((flags = fcntl((int)socket->getHandle(), F_GETFL)) == -1) {
    throw NetworkException("Unable to get flags for socket", this);
  }
  return flags & FASYNC;
#endif // flavor
}

void Socket::setAsynchronous(bool value) throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((int)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     throw NetworkException("Unable to set blocking mode", this);
//   }
#else // unix
  int flags;
  if ((flags = fcntl((int)socket->getHandle(), F_GETFL)) == -1) {
    throw NetworkException("Unable to get flags for socket", this);
  }
  if (value) {
    if ((flags & FASYNC) == 0) { // do we need to set flag
      flags |= FASYNC;
      if (fcntl((int)socket->getHandle(), F_SETFL, flags) != 0) {
        throw NetworkException("Unable to set flags of socket", this);
      }
    }
  } else {
    if ((flags & FASYNC) != 0) { // do we need to clear flag
      flags &= ~FASYNC;
      if (fcntl((int)socket->getHandle(), F_SETFL, flags) != 0) {
        throw NetworkException("Unable to set flags of socket", this);
      }
    }
  }
#endif // flavor
}
#endif

unsigned int Socket::available() const throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result;
  if (ioctlsocket((int)socket->getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
    throw NetworkException("Unable to determine the amount of data pending in the input buffer", this);
  }
  return result;
#else // unix
  // this implementation is not very portable?
  int result;
  if (ioctl((int)socket->getHandle(), FIONREAD, &result)) {
    throw NetworkException("Unable to determine the amount of data pending in the incomming queue", this);
  }
  return result;
#endif // flavor
}

#if 0
unsigned int Socket::pending() const throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result = 0;
//   if (ioctlsocket((int)socket->getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
//     throw NetworkException("Unable to determine the amount of data pending in the input buffer", this);
//   }
  return result;
#else // unix
  // this implementation is not very portable?
  int result;
  if (ioctl((int)socket->getHandle(), TIOCOUTQ, &result)) {
    throw NetworkException("Unable to determine the amount of data pending in the outgoing queue", this);
  }
  return result;
#endif // flavor
}
#endif

void Socket::flush() throw(NetworkException) {
}

unsigned int Socket::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(NetworkException) {
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::recv(
      (int)socket->getHandle(),
      buffer,
      minimum<unsigned int>(bytesToRead, PrimitiveTraits<int>::MAXIMUM),
      0
    );
    if (result < 0) { // has an error occured
      switch (::WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was read
        continue; // try again
      case WSAEWOULDBLOCK: // no data available (only in nonblocking mode)
        return bytesRead; // try later
      default:
        throw bindError(
          NetworkException("Unable to read from socket", this),
          internal::SocketImpl::getNativeError()
        );
      }
    }
#else // unix
    int result = ::recv(
      (int)socket->getHandle(),
      buffer,
      minimum<unsigned int>(bytesToRead, SSIZE_MAX),
      0
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in nonblocking mode)
        return bytesRead; // try later
      default:
        throw bindError(
          NetworkException("Unable to read from socket", this),
          internal::SocketImpl::getNativeError()
        );
      }
    }
#endif // flavor
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      if (bytesRead) {
        break;
      }
      throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int Socket::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(NetworkException) {
  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::send(
      (int)socket->getHandle(),
      buffer,
      minimum<unsigned int>(bytesToWrite, PrimitiveTraits<int>::MAXIMUM),
      0
    );
    if (result < 0) { // has an error occured
      switch (::WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was written
        continue; // try again
      case WSAEWOULDBLOCK: // no data could be written without blocking (only in nonblocking mode)
        return bytesWritten; // try later
      case WSAESHUTDOWN:
        throw BrokenStream(this);
      default:
        throw bindError(
          NetworkException("Unable to write to socket", this),
          internal::SocketImpl::getNativeError()
        );
      }
    }
#else // unix
    int result = ::send(
      (int)socket->getHandle(),
      buffer,
      minimum<unsigned int>(bytesToWrite, SSIZE_MAX),
      0
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in nonblocking mode)
        return bytesWritten; // try later
      case EPIPE:
        throw BrokenStream(this);
      default:
        throw bindError(
          NetworkException("Unable to write to socket", this),
          internal::SocketImpl::getNativeError()
        );
      }
    }
#endif // flavor
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
    if (nonblocking) {
      break;
    }
  }
  return bytesWritten;
}

unsigned int Socket::receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(NetworkException) {
  int result = 0;
  SocketAddress sa;
  socklen sl = sa.getSize();
  if ((result = ::recvfrom((int)socket->getHandle(),  buffer, size, 0, sa.getValue(), &sl)) == -1) {
    throw bindError(
      NetworkException("Unable to receive from", this),
      internal::SocketImpl::getNativeError()
    );
  }
  address = sa.getAddress();
  port = sa.getPort();
  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, const InetAddress& address, unsigned short port) throw(NetworkException) {
  int result = 0;
  const SocketAddress sa(address, port);
  if ((result = ::sendto((int)socket->getHandle(), buffer, size, 0, sa.getValue(), sa.getSize())) == -1) {
    throw bindError(
      NetworkException("Unable to send to", this),
      internal::SocketImpl::getNativeError()
    );
  }
  return result;
}

void Socket::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(socket->getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation Socket::read(char* buffer, unsigned int bytesToRead, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(
    socket->getHandle(),
    buffer,
    bytesToRead,
    listener
  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

AsynchronousWriteOperation Socket::write(const char* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(
    socket->getHandle(),
    buffer,
    bytesToWrite,
    listener
  );
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void Socket::wait() const throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);
  
  int result = ::select((int)socket->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == SOCKET_ERROR) {
    throw NetworkException("Unable to wait for input", this);
  }
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((int)socket->getHandle(), &rfds);
  
  int result = ::select((int)socket->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    throw NetworkException("Unable to wait for input", this);
  }
#endif // flavor
}

bool Socket::wait(unsigned int microseconds) const throw(NetworkException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);
  
  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;
  
  int result = ::select((int)socket->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == SOCKET_ERROR) {
    throw NetworkException("Unable to wait for input", this);
  }
  return result != 0; // return true if data available
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((int)socket->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;

  int result = ::select((int)socket->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    throw NetworkException("Unable to wait for input", this);
  }
  return result != 0; // return true if data available
#endif // flavor
}

Socket::~Socket() throw(IOException) {
}

//class FileDescriptorEvent {
//public:
//
//  /** File descriptor events. */
//  enum {
//    INPUT = 1, /* There is data to read */
//    PRIORITYINPUT = 2, /* There is urgent data to read */
//    OUTPUT = 4, /* Writing now will not block */
//    ERROR = 8, /* Error condition */
//    HUNGUP = 16, /* Hung up */
//    INVALID = 32 /* Invalid request: fd not open */
//  };
//private:
//
//  int handle;
//  unsigned int requestedEvents;
//  unsigned int events;
//public:
//
//  /**
//    Initializes event.
//
//    @param handle The file descriptor.
//    @param events The requested events.
//  */
//  FileDescriptorEvent(int handle, unsigned int events) throw(OutOfDomain) {
//    assert(events & ~(INPUT | PRIORITYINPUT | OUTPUT), OutOfDomain());
//    this->handle = handle;
//    this->requestedEvents = events;
//    this->events = 0;
//  }
//
//  /**
//  */
//  inline int getFileDescriptor() const throw() {return poll.fd;}
//
//  /**
//  */
//  inline unsigned int getRequestedEvents() const throw() {return poll.events;}
//
//  /**
//  */
//  inline unsigned int getEvents() const throw() {return poll.revents;}
//
//  /**
//    Resets the events.
//  */
//  inline void reset() throw() {events = 0;}
//
//  bool wait(Array<FileDescriptorEvent>& objects, int timeout) throw(NetworkException) {
//    Allocator<struct pollfd> buffer(objects.getSize());
//    struct pollfd* p = buffer.getElements();
//    for (unsigned int i = 0; i < objects.getSize(); ++i) {
//      p->fd = objects[i].getFileDescriptor();
//      p->events = objects[i].getRequestedEvents();
//      p->revents = 0;
//      ++p;
//    }
//    int result = poll(buffer.getElements(), buffer.getSize(), timeout);
//    if (result == 0) { // timeout
//      return false;
//    } else if (result < 0) { // error
//      int err = errno;
//      if (err == EINTR) {
//        return false;
//      }
//      assert(true, NetworkException());
//    } else {
//      struct pollfd* p = buffer.getElements();
//      for (unsigned int i = 0; i < objects.getSize(); ++i) {
//        objects[i].requestedEvents = p->revents;
//        ++p;
//      }
//      return true;
//    }
//  }
//};
//
//class FDEventArrayImpl {
//private:
//
//  Array<struct pollfd> buffer;
//  unsigned int modified;
//public:
//
//  FDEventArray() throw() {
//  }
//
//  void registerHandle(int handle, unsigned int events) throw() {
//    // find and add
//  }
//
//  void deregisterHandle(int handle) throw() {
//    // remove handle
//  }
//
//  bool wait(int timeout) throw(NetworkException) {
//    struct pollfd* elements = buffer.getElements();
//    struct pollfd* p = elements;
//    const struct pollfd* q = elements + buffer.getSize();
//    for (; p < q; ++p) { // reset envets
//      p->revents = 0;
//    }
//    int result = poll(elements, buffer.getSize(), timeout);
//    if (result == 0) { // timeout
//      return false;
//    } else if (result < 0) { // error
//      int err = errno;
//      if (err == EINTR) {
//        return false;
//      }
//      throw NetworkException();
//    } else {
//      return true;
//    }
//  }
//};
//
//class FDEventArray : public Object {
//private:
//public:
//};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
