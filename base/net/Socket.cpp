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
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/socket.h>
#  include <netinet/in.h> // defines ntohs...
#  include <netinet/tcp.h> // options
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

#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
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
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  #if defined(SIN6_LEN)
    sa.sin6_len = sizeof(sa);
  #endif // SIN6_LEN
    sa.sin6_family = AF_INET6;
    sa.sin6_port = ByteOrder::toBigEndian<unsigned short>(port);
    if (addr.getFamily() == InetAddress::IP_VERSION_4) {
      InetAddress temp(addr);
      temp.convertToIPv6();
      copy<byte>(getByteAddress(sa.sin6_addr), temp.getAddress(), sizeof(struct in6_addr));
    } else {
      copy<byte>(getByteAddress(sa.sin6_addr), addr.getAddress(), sizeof(struct in6_addr));
    }
  #else // only IPv4 support
    assert(
      (addr.getFamily() == InetAddress::IP_VERSION_4) || addr.isIPv4Mapped() || addr.isUnspecified(),
      NetworkException("Address not supported")
    );
    sa.sin_family = AF_INET;
    sa.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
    copy<byte>(getByteAddress(sa.sin_addr), addr.getIPv4Address(), sizeof(struct in_addr));
  #endif // _DK_SDU_MIP__BASE__INET_IPV6
  }

  /** Returns pointer to socket address. */
  inline struct sockaddr* getValue() throw() {return Cast::pointer<struct sockaddr*>(&sa);}

  /** Returns pointer to socket address. */
  inline const struct sockaddr* getValue() const throw() {return Cast::pointer<const struct sockaddr*>(&sa);}

  /** Returns the size of the socket address structure. */
  inline unsigned int getSize() const throw() {return sizeof(sa);}

  /** Returns the address. */
  inline InetAddress getAddress() const throw() {
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
    switch (sa.sin6_family) {
    case AF_INET:
      return InetAddress(getByteAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr), InetAddress::IP_VERSION_4);
    case AF_INET6:
      return InetAddress(getByteAddress(Cast::pointer<const struct sockaddr_in6*>(&sa)->sin6_addr), InetAddress::IP_VERSION_6);
    default:
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
  #else
    if (sa.sin_family == AF_INET) {
      return InetAddress(getByteAddress(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_addr), InetAddress::IP_VERSION_4);
    } else {
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
  #endif
  }

  /** Returns the port. */
  inline unsigned short getPort() const throw() {
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
    switch (sa.sin6_family) {
    case AF_INET:
      return ByteOrder::fromBigEndian<unsigned short>(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_port);
    case AF_INET6:
      return ByteOrder::fromBigEndian<unsigned short>(Cast::pointer<const struct sockaddr_in6*>(&sa)->sin6_port);
    default:
      return 0; // TAG: or should I throw an exception
    }
  #else
    if (sa.sin_family == AF_INET) {
      return ByteOrder::fromBigEndian<unsigned short>(Cast::pointer<const struct sockaddr_in*>(&sa)->sin_port);
    } else {
      return 0; // TAG: or should I throw an exception
    }
  #endif
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
    
    static inline void getOption(int handle, int level, int option, void* buffer, unsigned int* length) throw(IOException) {
      socklen temp = *length;
      if (::getsockopt(handle, level, option, static_cast<char*>(buffer), &temp) != 0) {
        throw IOException("Unable to get IP option");
      }
      *length = temp;
    }
    
    static inline void setOption(int handle, int level, int option, const void* buffer, unsigned int length) throw(IOException) {
      if (::setsockopt(handle, level, option, static_cast<const char*>(buffer), length) != 0) {
        throw IOException("Unable to set IP option");
      }
    }
    
    static inline void getSocketOption(int handle, int option, void* buffer, unsigned int* length) throw(IOException) {
      socklen temp = *length;
      if (::getsockopt(handle, SOL_SOCKET, option, static_cast<char*>(buffer), &temp) != 0) {
        throw IOException("Unable to get socket option");
      }
      *length = temp;
    }
    
    // TAG: add support for UDP options: IPPROTO_UDP
    // TAG: add support for RAW options: IPPROTO_RAW
    
    static inline void setSocketOption(int handle, int option, const void* buffer, unsigned int length) throw(IOException) {
      if (::setsockopt(handle, SOL_SOCKET, option, static_cast<const char*>(buffer), length) != 0) {
        throw IOException("Unable to set socket option");
      }
    }
    
    static inline void getTcpOption(int handle, int option, void* buffer, unsigned int* length) throw(IOException) {
      socklen temp = *length;
      if (::getsockopt(handle, IPPROTO_TCP, option, static_cast<char*>(buffer), &temp) != 0) {
        throw IOException("Unable to get TCP option");
      }
      *length = temp;
    }
    
    static inline void setTcpOption(int handle, int option, const void* buffer, unsigned int length) throw(IOException) {
      // setsockopt is MT-safe
      if (::setsockopt(handle, IPPROTO_TCP, option, static_cast<const char*>(buffer), length) != 0) {
        throw IOException("Unable to set TCP option");
      }
    }
  };
}; // end of namespace internal



Socket::SocketImpl::SocketImpl(OperatingSystem::Handle _handle) throw()
  : Handle(_handle), remotePort(0), localPort(0) {
}

Socket::SocketImpl::~SocketImpl() throw(IOException) {
  if (isValid()) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (::closesocket((int)getHandle())) {
      throw NetworkException("Unable to close socket", this);
    }
#else // unix
    if (::close((int)getHandle())) {
      throw NetworkException("Unable to close socket", this);
    }
#endif // flavor
  }
}



Socket::Socket() throw() : socket(SocketImpl::invalid) {
}

bool Socket::accept(Socket& socket) throw(IOException) {
  // ExclusiveSynchronize<Guard>(*this);

  if (this->socket->isValid()) {
    throw NetworkException("Attempt to overwrite socket", this);
  }

  SocketAddress sa;
  socklen sl = sa.getSize();
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::accept((int)socket.getHandle(), sa.getValue(), &sl);
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
  this->socket = new SocketImpl(handle);
  this->socket->setRemoteAddress(sa.getAddress());
  this->socket->setRemotePort(sa.getPort());
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  ExclusiveSynchronize<Guard>(*this);
  SocketAddress sa(addr, port);
  if (::bind((int)getHandle(), sa.getValue(), sa.getSize())) {
    throw NetworkException("Unable to assign name to socket", this);
  }
//  if ((addr.isUnspecified()) || (port == 0)) { // do we need to determine assigned name
//    sa.setSocket((int)getHandle());
//    socket->setLocalAddress(sa.getAddress());
//    socket->setLocalPort(sa.getPort());
//  } else {
    socket->setLocalAddress(addr);
    socket->setLocalPort(port);
//  }
}

void Socket::close() throw(IOException) {
  ExclusiveSynchronize<Guard>(*this);
  socket = SocketImpl::invalid;
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  ExclusiveSynchronize<Guard>(*this);
  SocketAddress sa(addr, port);
  if (::connect((int)getHandle(), sa.getValue(), sa.getSize())) {
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
//  sa.setSocket((int)getHandle());
//  socket->setLocalAddress(sa.getAddress());
//  socket->setLocalPort(sa.getPort());
  socket->setRemoteAddress(addr);
  socket->setRemotePort(port);
}

void Socket::create(bool stream) throw(IOException) {
  ExclusiveSynchronize<Guard>(*this);
  OperatingSystem::Handle handle;
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (socket->isValid() ||
      ((handle = (OperatingSystem::Handle)::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == OperatingSystem::INVALID_HANDLE)) {
    throw NetworkException("Unable to create socket", this);
  }
#else
  if (socket->isValid() ||
      ((handle = (OperatingSystem::Handle)::socket(PF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == OperatingSystem::INVALID_HANDLE)) {
    throw NetworkException("Unable to create socket", this);
  }
#endif
  socket = new SocketImpl(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  // silently reduce the backlog argument
  backlog = minimum<unsigned int>(backlog, PrimitiveTraits<int>::MAXIMUM);
  if (::listen((int)getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections", this);
  }
}

void Socket::getName() throw() {
  SharedSynchronize<Guard>(*this);
  SocketAddress sa;
  sa.setSocket((int)getHandle());
  socket->setLocalAddress(sa.getAddress());
  socket->setLocalPort(sa.getPort());
}

const InetAddress& Socket::getAddress() const throw() {
  SharedSynchronize<Guard>(*this);
  return socket->getRemoteAddress();
}

unsigned short Socket::getPort() const throw() {
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const throw() {
  SharedSynchronize<Guard>(*this);
  return socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const throw() {
  SharedSynchronize<Guard>(*this);
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)getHandle(), SD_RECEIVE)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading", this);
  }
#else // unix
  if (::shutdown((int)getHandle(), 0)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading", this);
  }
#endif // flavor
}

void Socket::shutdownOutputStream() throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)getHandle(), SD_SEND)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing", this);
  }
#else // unix
  if (::shutdown((int)getHandle(), 1)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing", this);
  }
#endif // flavor
}

bool Socket::getBooleanOption(int option) const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getSocketOption((int)getHandle(), option, &buffer, &length);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer = value;
  internal::SocketImpl::setSocketOption((int)getHandle(), option, &buffer, sizeof(buffer));
}

int Socket::getErrorState() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getSocketOption((int)getHandle(), SO_ERROR, &buffer, &length);
  return buffer; // TAG: map to socket::ErrorState enum
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
  SharedSynchronize<Guard>(*this);
  struct linger buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getSocketOption((int)getHandle(), SO_LINGER, &buffer, &length);
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
  internal::SocketImpl::setSocketOption((int)getHandle(), SO_LINGER, &buffer, sizeof(buffer));
}

int Socket::getReceiveBufferSize() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getSocketOption((int)getHandle(), SO_RCVBUF, &buffer, &length);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer = size;
  internal::SocketImpl::setSocketOption((int)getHandle(), SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getSocketOption((int)getHandle(), SO_SNDBUF, &buffer, &length);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer = size;
  internal::SocketImpl::setSocketOption((int)getHandle(), SO_SNDBUF, &buffer, sizeof(buffer));
}

// TAG: TCP_MAXSEG maximum segment size
// TAG: TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT interval between keepalives
// TAG: microsoft specific datagram options: SO_MAXDG, SO_MAXPATHDG
// TAG: time out options: SO_SNDTIMEO, SO_RCVTIMEO
// TAG: connection time (MS specific?): SO_CONNECT_TIME (0xffffffff is not connected)
// TAG: max backlog queue length: SOMAXCONN

bool Socket::getTcpNoDelay() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getTcpOption((int)getHandle(), TCP_NODELAY, &buffer, &length);
  return buffer != 0;
}

void Socket::setTcpNoDelay(bool value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer = value;
  internal::SocketImpl::setTcpOption((int)getHandle(), TCP_NODELAY, &buffer, sizeof(buffer));
}

unsigned int Socket::getTimeToLive() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)getHandle(), IPPROTO_IP, IP_TTL, &buffer, &length);
  return buffer != 0;
}

void Socket::setTimeToLive(unsigned int value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int buffer = value;
  internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_TTL, &buffer, sizeof(buffer));
}

uint8 Socket::getMulticastTTL() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  // TAG: fixme
  int buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_TTL, &buffer, &length);
  return buffer;
#else
  u_char buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_TTL, &buffer, &length);
  return buffer;
#endif
}

void Socket::setMulticastTTL(uint8 value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (false) {
    uint_l buffer;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &buffer, sizeof(buffer));
  } else {
    u_char buffer = value;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_TTL, &buffer, sizeof(buffer));
  }
#else
  u_char buffer = value;
  internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_TTL, &buffer, sizeof(buffer));
#endif
}

bool Socket::getMulticastLoopback() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  u_char buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_LOOP, &buffer, &length);
  return buffer != 0;
#else
  u_char buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_LOOP, &buffer, &length);
  return buffer != 0;
#endif
}

void Socket::setMulticastLoopback(bool value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (false) {
    uint_t loop;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &buffer, sizeof(buffer));
  } else {
    u_char buffer = value;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_LOOP, &buffer, sizeof(buffer));
  }
#else
  u_char buffer = value;
  internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_MULTICAST_LOOP, &buffer, sizeof(buffer));
#endif
}

#if 0
void Socket::joinGroup() throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket((int)getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
    throw IOException("Unable to set blocking mode", this);
  }
  IPPROTO_IP
  IP_MREQ FAR *structure
  // IP_ADD_MEMBERSHIP
  // IP_DROP_MEMBERSHIP
#else // unix
#endif // flavor
//   struct ip_mreq buffer;
//   internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_ADD_MEMBERSHIP, &buffer, sizeof(buffer));
// // struct ip_mreq {
// //   struct in_addr imr_multiaddr;   /* multicast group to join */
// //   struct in_addr imr_interface;   /* interface to join on */
// // }
    struct ipv6_mreq {
        struct in6_addr ipv6mr_multiaddr;   /* IPv6 multicast addr */
        unsigned int    ipv6mr_interface;   /* interface index */
    }  

  struct ipv6_mreq mreq;
    setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq, sizeof(mreq)) 
}

// Family Socket::getType() const throw() {
// }

void Socket::leaveGroup(unsigned int interface, const InetAddress& address) throw(IOException) {
#if 0 && defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (false) { // get INET type of socket
    struct ipv6_mreq buffer;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IPV6, IP_LEAVE_GROUP, &buffer, sizeof(buffer));  
  } else {
    struct ip_mreq buffer;
    internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_DROP_MEMBERSHIP, &buffer, sizeof(buffer));
  }
#else
  // resolve interface address by index
  assert(ioctl(handle, SIOCGIFADDR, current) == 0, IOException(this));
  interface.address = internal::InetInterface::getAddress(current->ifr_addr);
  InetAddress temp = address;
  assert(temp.convertToIPv4(), IOException(this));
  struct ip_mreq buffer;
  
  internal::SocketImpl::setOption((int)getHandle(), IPPROTO_IP, IP_DROP_MEMBERSHIP, &buffer, sizeof(buffer));
#endif
}
#endif

void Socket::setNonBlocking(bool value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket((int)getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
    throw IOException("Unable to set blocking mode", this);
  }
#else // unix
  int flags;
  if ((flags = fcntl((int)getHandle(), F_GETFL)) == -1) {
    throw IOException("Unable to get flags for socket", this);
  }
  if (value) {
    if ((flags & O_NONBLOCK) == 0) { // do we need to set flag
      flags |= O_NONBLOCK;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  } else {
    if ((flags & O_NONBLOCK) != 0) { // do we need to clear flag
      flags &= ~O_NONBLOCK;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  }
#endif // flavor
}

#if 0
bool Socket::getAsynchronous() throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((int)getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     throw IOException("Unable to set blocking mode", this);
//   }
#else // unix
  int flags;
  if ((flags = fcntl((int)getHandle(), F_GETFL)) == -1) {
    throw IOException("Unable to get flags for socket", this);
  }
  return flags & FASYNC;
#endif // flavor
}

void Socket::setAsynchronous(bool value) throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((int)getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     throw IOException("Unable to set blocking mode", this);
//   }
#else // unix
  int flags;
  if ((flags = fcntl((int)getHandle(), F_GETFL)) == -1) {
    throw IOException("Unable to get flags for socket", this);
  }
  if (value) {
    if ((flags & FASYNC) == 0) { // do we need to set flag
      flags |= FASYNC;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  } else {
    if ((flags & FASYNC) != 0) { // do we need to clear flag
      flags &= ~FASYNC;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  }
#endif // flavor
}
#endif

unsigned int Socket::available() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result;
  if (ioctlsocket((int)getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
    throw IOException("Unable to determine the amount of data pending in the input buffer", this);
  }
  return result;
#else // unix
  // this implementation is not very portable?
  int result;
  if (ioctl((int)getHandle(), FIONREAD, &result)) {
    throw IOException("Unable to determine the amount of data pending in the incomming queue", this);
  }
  return result;
#endif // flavor
}

#if 0
unsigned int Socket::pending() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result = 0;
//   if (ioctlsocket((int)getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
//     throw IOException("Unable to determine the amount of data pending in the input buffer", this);
//   }
  return result;
#else // unix
  // this implementation is not very portable?
  int result;
  if (ioctl((int)getHandle(), TIOCOUTQ, &result)) {
    throw IOException("Unable to determine the amount of data pending in the outgoing queue", this);
  }
  return result;
#endif // flavor
}
#endif

void Socket::flush() throw(IOException) {
}

unsigned int Socket::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(IOException) {
  SharedSynchronize<Guard>(*this);
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
        throw IOException("Unable to read from socket", this);
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
        throw IOException("Unable to read from socket", this);
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

unsigned int Socket::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(IOException) {
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
        throw IOException("Unable to write to socket", this);
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
        throw IOException("Unable to write to socket", this);
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

unsigned int Socket::receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException) {
  int result = 0;
  SocketAddress sa;
  socklen sl = sa.getSize();
  if ((result = ::recvfrom((int)getHandle(),  buffer, size, 0, sa.getValue(), &sl)) == -1) {
    throw IOException("Unable to receive from", this);
  }
  address = sa.getAddress();
  port = sa.getPort();
  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, const InetAddress& address, unsigned short port) throw(IOException) {
  SharedSynchronize<Guard>(*this);
  int result = 0;
  const SocketAddress sa(address, port);
  if ((result = ::sendto((int)getHandle(), buffer, size, 0, sa.getValue(), sa.getSize())) == -1) {
    throw IOException("Unable to send to", this);
  }
  return result;
}

void Socket::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation Socket::read(char* buffer, unsigned int bytesToRead, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(getHandle(), buffer, bytesToRead, listener);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

AsynchronousWriteOperation Socket::write(const char* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(getHandle(), buffer, bytesToWrite, listener);
#else // unix
  throw NotImplemented(this);
#endif // flavor
}

void Socket::wait() const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)getHandle(), &rfds);
  
  int result = ::select((int)getHandle() + 1, &rfds, 0, 0, 0);
  if (result == SOCKET_ERROR) {
    throw IOException("Unable to wait for input", this);
  }
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((int)getHandle(), &rfds);
  
  int result = ::select((int)getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    throw IOException("Unable to wait for input", this);
  }
#endif // flavor
}

bool Socket::wait(unsigned int microseconds) const throw(IOException) {
  SharedSynchronize<Guard>(*this);
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)getHandle(), &rfds);
  
  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;
  
  int result = ::select((int)getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == SOCKET_ERROR) {
    throw IOException("Unable to wait for input", this);
  }
  return result != 0; // return true if data available
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((int)getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;

  int result = ::select((int)getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input", this);
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
//  bool wait(Array<FileDescriptorEvent>& objects, int timeout) throw(IOException) {
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
//      assert(true, IOException());
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
//  bool wait(int timeout) throw(IOException) {
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
//      throw IOException();
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
