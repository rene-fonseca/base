/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/ByteOrder.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/io/AccessDenied.h>
#include <base/io/TimedOut.h>
#include <base/io/BrokenStream.h>
#include <base/net/Socket.h>
#include <base/concurrency/Thread.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>
#include <base/build.h>

// TAG: profile socket for entire use

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
#  include <windows.h>
#  include <winsock.h>
// #  include <winsock2.h>
// #  include <ws2tcpip.h>
#  undef interface
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
#  include <sys/socket.h>
#endif
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
#  include <netinet/in.h> // defines ntohs...
#  include <netinet/tcp.h> // options
#endif
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <net/if.h>
#endif
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <sys/time.h> // defines timeval on Linux systems

#  if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__CYGWIN) && \
      (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS)
#    if defined(__has_include)
#      if __has_include(<stropts.h>)
#        include <stropts.h> // defines FLUSH macros
#      endif
#    endif
#  endif

#  include <string.h> // memset (required on solaris)
#  include <limits.h> // defines SSIZE_MAX

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif

#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
#  include <arpa/inet.h>
#endif

#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#    define BSD_COMP 1 // request BSD flags - don't known if this is ok to do
#  endif
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
#  include <sys/ioctl.h> // defines FIONREAD
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS)
int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds,
         struct timeval *restrict timeout)
{
  return EINVAL;
}
#else
#  include <sys/select.h>
#endif

#endif // flavor

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32)
typedef int SOCKET;
#endif

// do we need to repair bad header file
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) && defined(bind))
#  define _COM_AZURE_DEV__BASE__SOCKET_BIND __xnet_bind
#  define _COM_AZURE_DEV__BASE__SOCKET_CONNECT __xnet_connect
#  define _COM_AZURE_DEV__BASE__SOCKET_RECVMSG __xnet_recvmsg
#  define _COM_AZURE_DEV__BASE__SOCKET_SENDMSG __xnet_sendmsg
#  define _COM_AZURE_DEV__BASE__SOCKET_SENDTO __xnet_sendto
#  define _COM_AZURE_DEV__BASE__SOCKET_SOCKET __xnet_socket

#  undef bind
#  undef connect
#  undef recvmsg
#  undef sendmsg
#  undef sendto
#  undef socket

  inline int bind(int s, const struct sockaddr* name, int namelen)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_BIND(s, name, namelen);
  }

  inline int connect(int s, const struct sockaddr* name, int namelen)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_CONNECT(s, name, namelen);
  }

  inline ssize_t recvmsg(int s, struct msghdr* msg, int flags)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_RECVMSG(s, msg, flags);
  }

  inline ssize_t sendmsg(int s, const struct msghdr* msg, int flags)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_SENDMSG(s, msg, flags);
  }

  inline ssize_t sendto(int s, const void* msg, size_t len, int flags, const struct sockaddr* to, int tolen)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_SENDTO(s, msg, len, flags, to, tolen);
  }

  inline int socket(int domain, int type, int protocol)
  {
    return _COM_AZURE_DEV__BASE__SOCKET_SOCKET(domain, type, protocol);
  }
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)    
  typedef int socklen;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  typedef int socklen;
#else
  typedef socklen_t socklen;
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
extern "C" struct hostent* gethostbyaddr(const void *addr, socklen len, int type)
{
  return nullptr;
}
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class SocketAddress { // Internet end point
private:

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  struct sockaddr {
    int reserved;
  };
  struct sockaddr_in {
    int reserved;
  };
#endif

  union {
    struct sockaddr sa;
    struct sockaddr_in ipv4;
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    struct sockaddr_in6 ipv6;
    struct sockaddr_storage storage;
#else
    struct sockaddr_in storage;
#endif
  };
public:
  
  inline SocketAddress() noexcept
  {
    clear(sa);
  }
  
  /** Initializes socket address. */
  SocketAddress(
    const InetAddress& address,
    unsigned short port,
    Socket::Domain domain)
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#else
    clear(sa);
#  if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    if (domain == Socket::IPV6) {
#    if (defined(SIN6_LEN))
      ipv6.sin6_len = sizeof(ipv6);
#    endif // SIN6_LEN
      ipv6.sin6_family = AF_INET6;
      ipv6.sin6_port = ByteOrder::toBigEndian<unsigned short>(port);
      if (address.getFamily() == InetAddress::IP_VERSION_4) {
        InetAddress temp(address);
        temp.convertToIPv6();
        copy<uint8>(
          Cast::getAddress(ipv6.sin6_addr),
          temp.getAddress(),
          sizeof(ipv6.sin6_addr)
        );
      } else {
        copy<uint8>(
          Cast::getAddress(ipv6.sin6_addr),
          address.getAddress(),
          sizeof(ipv6.sin6_addr)
        );
      }
    } else { // IPv4 domain
      ipv4.sin_family = AF_INET;
      ipv4.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
      InetAddress temp(address);
      bassert(
        temp.convertToIPv4(),
        NetworkException(
          "Address not supported",
          Type::getType<Socket>()
        )
      );
      copy<uint8>(
        Cast::getAddress(ipv4.sin_addr),
        address.getIPv4Address(),
        sizeof(ipv4.sin_addr)
      );
    }
#  else // only IPv4 support
    InetAddress temp(address);
    temp.convertToIPv4();
    bassert(
      temp.convertToIPv4(),
      NetworkException(
        "Address not supported",
        Type::getType<Socket>()
      )
    );
    ipv4.sin_family = AF_INET;
    ipv4.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
    copy<uint8>(
      Cast::getAddress(ipv4.sin_addr),
      address.getIPv4Address(),
      sizeof(struct in_addr)
    );
#  endif // _COM_AZURE_DEV__BASE__HAVE_INET_IPV6
#endif
  }

  /** Returns pointer to socket address. */
  inline struct sockaddr* getValue() noexcept
  {
    return &sa;
  }
  
  /** Returns pointer to socket address. */
  inline const struct sockaddr* getValue() const noexcept
  {
    return &sa;
  }
  
  /** Returns the size of the socket address structure. */
  inline unsigned int getSize() const noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    return 0;
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    return (sa.sa_family == AF_INET6) ? sizeof(ipv6) : sizeof(ipv4);
#else
    return sizeof(ipv4);
#endif
  }
  
  inline Socket::Domain getDomain() const noexcept
  {
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    return (sa.sa_family == AF_INET6) ? Socket::IPV6 : Socket::IPV4;
#else
    return Socket::IPV4;
#endif
  }
  
  /** Returns the maximum size of the supported socket addresses. */
  inline unsigned int getAnySize() const noexcept
  {
    return sizeof(storage);
  }
  
  /** Returns the address. */
  inline InetAddress getAddress() const noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    return InetAddress();
#else
    switch (sa.sa_family) {
    case AF_INET:
      return InetAddress(
        Cast::getAddress(ipv4.sin_addr),
        InetAddress::IP_VERSION_4
      );
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    case AF_INET6:
      return InetAddress(
        Cast::getAddress(ipv6.sin6_addr),
        InetAddress::IP_VERSION_6
      );
#endif
    default: // not possible
      return InetAddress(); // TAG: or should we raise an exception
    }
#endif
  }
  
  /** Returns the port. */
  inline unsigned short getPort() const noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    return 0;
#else
    switch (sa.sa_family) {
    case AF_INET:
      return ByteOrder::fromBigEndian<unsigned short>(ipv4.sin_port);
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
    case AF_INET6:
      return ByteOrder::fromBigEndian<unsigned short>(ipv6.sin6_port);
#endif
    default: // not possible
      return 0; // TAG: or should we raise an exception
    }
#endif
  }

  /** Sets the socket name from the specified socket. */
  inline void setSocket(int handle) noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
    socklen length = getAnySize();
    ::getsockname(handle, getValue(), &length);
#endif
  }
};

namespace internal {

  class SocketImpl {
  public:
    
    static inline int getNativeError() noexcept
    {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
      return ::WSAGetLastError();
#else // unix
      return errno;
#endif // flavor
    }

    static unsigned int getCause(unsigned int error) noexcept
    {
      switch (error) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
      case EHOSTDOWN:
        return NetworkException::HOST_DOWN;
#endif
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

    static inline void raiseNetwork(const char* message)
    {
      const unsigned int error = getNativeError();
      const unsigned int cause = getCause(error);
      if (cause != PrimitiveTraits<unsigned int>::MAXIMUM) {
        _throw NetworkException(message, Type::getType<Socket>(), cause);
      } else {
        _throw NetworkException(message, Type::getType<Socket>(), error, 0);
      }
    }
    
    static inline void getOption(
      int handle,
      int level,
      int option,
      void* buffer,
      unsigned int* length)
    {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
      raiseNetwork("Unable to get option.");
#else
      socklen temp = *length;
      if (::getsockopt(handle, level, option, static_cast<char*>(buffer), &temp)) {
        raiseNetwork("Unable to get option.");
      }
      *length = temp;
#endif
    }
    
    static inline void setOption(
      int handle,
      int level,
      int option,
      const void* buffer,
      unsigned int length)
    {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
        raiseNetwork("Unable to set option.");
#else
      if (::setsockopt(handle, level, option, static_cast<const char*>(buffer), length)) {
        raiseNetwork("Unable to set option.");
      }
#endif
    }
    
    // TAG: add support for UDP options: IPPROTO_UDP
    // TAG: add support for RAW options: IPPROTO_RAW
  };
}; // end of namespace internal



Socket::SocketImpl::SocketImpl(
  OperatingSystem::Handle _handle,
  Domain _domain,
  Kind _kind) noexcept
  : Handle(_handle),
    domain(_domain),
    kind(_kind),
    remotePort(0),
    localPort(0)
{
}

Socket::SocketImpl::~SocketImpl()
{
  if (isValid()) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (::closesocket((SOCKET)getHandle())) {
#else // unix
    if (::close((int)getHandle())) {
#endif // flavor
      IOException e("Unable to close socket.", this);
      const unsigned int error = internal::SocketImpl::getNativeError();
      const unsigned int cause = internal::SocketImpl::getCause(error);
      if (cause != PrimitiveTraits<unsigned int>::MAXIMUM) {
        e.setCause(cause);
      } else {
        e.setError(error);
      }
      throw e; // will copy
    }
  }
}

Socket::SocketImpl* Socket::SocketImpl::getInvalid() noexcept
{
  return invalid;
}

Socket::Socket() noexcept
  : socket(SocketImpl::invalid)
{
}

bool Socket::accept(Socket& socket)
{
  Profiler::ResourceCreateTask profile("Socket::accept()");

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return false;
#else
  // TAG: should return socket (invalid if non available)
  
  if (this->socket->isValid()) {
    internal::SocketImpl::raiseNetwork("Attempt to overwrite socket");
  }

  SocketAddress sa;
  socklen sl = sa.getAnySize();
  OperatingSystem::Handle handle =
    (OperatingSystem::Handle)::accept((SOCKET)socket.socket->getHandle(), sa.getValue(), &sl);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (handle == OperatingSystem::INVALID_HANDLE) {
    switch (::WSAGetLastError()) {
    case WSAEWOULDBLOCK:
      return false;
    default:
      internal::SocketImpl::raiseNetwork("Unable to accept connection.");
    }
  }
#else // unix
  if (handle == OperatingSystem::INVALID_HANDLE) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      internal::SocketImpl::raiseNetwork("Unable to accept connection.");
    }
  }
#endif // flavor
  this->socket = new SocketImpl(handle, sa.getDomain(), Socket::STREAM);
  this->socket->setRemoteAddress(sa.getAddress());
  this->socket->setRemotePort(sa.getPort());
  return true;
#endif
}

void Socket::bind(const InetAddress& address, unsigned short port)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  SocketAddress sa(address, port, socket->getDomain());
  
  if (::bind((SOCKET)socket->getHandle(), sa.getValue(), sa.getSize())) {
    internal::SocketImpl::raiseNetwork("Unable to assign name to socket.");
  }
  if (address.isUnspecified() || (port == 0)) {
    sa.setSocket((SOCKET)socket->getHandle());
    socket->setLocalAddress(sa.getAddress());
    socket->setLocalPort(sa.getPort());
  } else {
    socket->setLocalAddress(address);
    socket->setLocalPort(port);
  }
#endif
}

void Socket::close()
{
  socket = SocketImpl::invalid;
}

void Socket::connect(const InetAddress& address, unsigned short port)
{
  Profiler::WaitTask profile("Socket::connect()");

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  SocketAddress sa(address, port, socket->getDomain());
  if (::connect((SOCKET)socket->getHandle(), sa.getValue(), sa.getSize())) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    switch (::WSAGetLastError()) {
    case WSAECONNREFUSED:
      _throw AccessDenied(this);
    case WSAETIMEDOUT:
      _throw TimedOut(this);
    default:
      internal::SocketImpl::raiseNetwork("Unable to connect to socket.");
    }
#else // unix
    switch (errno) {
    case ECONNREFUSED:
      _throw AccessDenied(this);
    case ETIMEDOUT:
      _throw TimedOut(this);
    default:
      internal::SocketImpl::raiseNetwork("Unable to connect to socket.");
    }
#endif // flavor
  }
//  sa.setSocket((SOCKET)socket->getHandle());
//  socket->setLocalAddress(sa.getAddress());
//  socket->setLocalPort(sa.getPort());
  socket->setRemoteAddress(address);
  socket->setRemotePort(port);
#endif
}

void Socket::create(Kind kind, Domain domain)
{
  Profiler::ResourceCreateTask profile("Socket::create()");

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  // TAG: should return new socket not overwrite handle (static method)
  static const int SOCKET_KINDS[] = {SOCK_STREAM, SOCK_DGRAM, SOCK_RAW};
  bassert(
    !socket->isValid(),
    NetworkException("Unable to create socket.", this)
  );
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::socket(
    (domain != Socket::IPV4) ? PF_INET6 : PF_INET,
    SOCKET_KINDS[kind],
    0
  );
  bassert(
    handle != OperatingSystem::INVALID_HANDLE,
    NetworkException("Unable to create socket.", this)
  );
  socket = new SocketImpl(
    handle,
    (domain != Socket::IPV4) ? Socket::IPV6 : Socket::IPV4,
    kind
  );
#else
  bassert(
    (domain == Socket::IPV4) || (domain == Socket::DEFAULT_DOMAIN),
    NetworkException("Domain not supported.")
  );
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::socket(
    PF_INET,
    SOCKET_KINDS[kind],
    0
  );
  bassert(
    handle != OperatingSystem::INVALID_HANDLE,
    NetworkException("Unable to create socket.", this)
  );
  socket = new SocketImpl(handle, Socket::IPV4, kind);
#endif
#endif
}

void Socket::listen(unsigned int backlog)
{
  Profiler::WaitTask profile("Socket::listen()");

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  // silently reduce the backlog argument
  backlog = minimum<unsigned int>(backlog, PrimitiveTraits<int>::MAXIMUM);
  if (::listen((SOCKET)socket->getHandle(), backlog)) { // may also silently limit backlog
    internal::SocketImpl::raiseNetwork("Unable to set queue limit for incoming connections.");
  }
#endif
}

void Socket::getName() noexcept
{
  SocketAddress sa;
  sa.setSocket((SOCKET)socket->getHandle());
  socket->setLocalAddress(sa.getAddress());
  socket->setLocalPort(sa.getPort());
}

const InetAddress& Socket::getAddress() const noexcept
{
  return socket->getRemoteAddress();
}

unsigned short Socket::getPort() const noexcept
{
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const noexcept
{
  return socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const noexcept
{
  return socket->getLocalPort();
}

void Socket::shutdownInputStream()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::shutdown((SOCKET)socket->getHandle(), 0 /*SD_RECEIVE*/)) { // disallow further receives
    internal::SocketImpl::raiseNetwork("Unable to shutdown socket for reading.");
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  if (::shutdown((SOCKET)socket->getHandle(), 0)) { // disallow further receives
    internal::SocketImpl::raiseNetwork("Unable to shutdown socket for reading.");
  }
#endif // flavor
}

void Socket::shutdownOutputStream()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::shutdown((SOCKET)socket->getHandle(), 1 /*SD_SEND*/)) { // disallow further sends
    internal::SocketImpl::raiseNetwork("Unable to shutdown socket for writing.");
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  if (::shutdown((SOCKET)socket->getHandle(), 1)) { // disallow further sends
    internal::SocketImpl::raiseNetwork("Unable to shutdown socket for writing.");
  }
#endif // flavor
}

bool Socket::getBooleanOption(int option) const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    option,
    &buffer,
    &length
  );
  return buffer != 0;
#endif
}

void Socket::setBooleanOption(int option, bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = value;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    option,
    &buffer,
    sizeof(buffer)
  );
#endif
}

int Socket::getErrorState() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return -1;
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_ERROR,
    &buffer,
    &length
  );
  return buffer; // TAG: map to socket::ErrorState enum
#endif
}

bool Socket::getReuseAddress() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return false;
#else
  return getBooleanOption(SO_REUSEADDR);
#endif
}

void Socket::setReuseAddress(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  setBooleanOption(SO_REUSEADDR, value);
#endif
}

bool Socket::getKeepAlive() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return false;
#else
  return getBooleanOption(SO_KEEPALIVE);
#endif
}

void Socket::setKeepAlive(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  setBooleanOption(SO_KEEPALIVE, value);
#endif
}

bool Socket::getBroadcast() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return false;
#else
  return getBooleanOption(SO_BROADCAST);
#endif
}

void Socket::setBroadcast(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  setBooleanOption(SO_BROADCAST, value);
#endif
}

int Socket::getLinger() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return -1;
#else
  struct linger buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((SOCKET)socket->getHandle(), SOL_SOCKET, SO_LINGER, &buffer, &length);
  return (buffer.l_onoff != 0) ? buffer.l_linger : -1;
#endif
}

void Socket::setLinger(int seconds)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  struct linger buffer;
  if (seconds < 0) {
    buffer.l_onoff = 0; // disable linger
  } else {
    buffer.l_onoff = 1; // enable linger
    buffer.l_linger = seconds;
  }
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_LINGER,
    &buffer,
    sizeof(buffer)
  );
#endif
}

int Socket::getReceiveBufferSize() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return -1;
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVBUF,
    &buffer,
    &length
  );
  return buffer;
#endif
}

void Socket::setReceiveBufferSize(int size)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  int buffer = size;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVBUF,
    &buffer,
    sizeof(buffer)
  );
#endif
}

int Socket::getSendBufferSize() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption((SOCKET)socket->getHandle(), SOL_SOCKET, SO_SNDBUF, &buffer, &length);
  return buffer;
#endif
}

void Socket::setSendBufferSize(int size)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  int buffer = size;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDBUF,
    &buffer,
    sizeof(buffer)
  );
#endif
}

// TAG: TCP_MAXSEG maximum segment size
// TAG: TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT interval between keepalives
// TAG: microsoft specific datagram options: SO_MAXDG, SO_MAXPATHDG
// TAG: time out options: SO_SNDTIMEO, SO_RCVTIMEO
// TAG: connection time (MS specific?): SO_CONNECT_TIME (0xffffffff is not connected)
// TAG: max backlog queue length: SOMAXCONN

bool Socket::getDontRoute() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return false;
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_DONTROUTE,
    &buffer,
    &length
  );
  return buffer != 0;
#endif
}

void Socket::setDontRoute(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  int buffer = value;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_DONTROUTE,
    &buffer,
    sizeof(buffer)
  );
#endif
}

uint64 Socket::getReceiveTimeout() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else
  struct timeval buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVTIMEO,
    &buffer,
    &length
  );
  return (static_cast<uint64>(buffer.tv_sec) * 1000000 + buffer.tv_usec) * 1000;
#endif
}

void Socket::setReceiveTimeout(uint64 nanoseconds)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  struct timeval buffer;
  if (nanoseconds <= 60*60*24*1000000000ULL) { // one day
    buffer.tv_sec = static_cast<uint32>((nanoseconds + 999)/1000000000);
    buffer.tv_usec = static_cast<uint32>((nanoseconds + 999)/1000);
  } else {
    buffer.tv_sec = 60*60*24;
    buffer.tv_usec = 0;
  }
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_RCVTIMEO,
    &buffer,
    sizeof(buffer)
  );
#endif
}

uint64 Socket::getSendTimeout() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else
  struct timeval buffer;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDTIMEO,
    &buffer,
    &length
  );
  return (static_cast<uint64>(buffer.tv_sec) * 1000000 + buffer.tv_usec) * 1000;
#endif
}

void Socket::setSendTimeout(uint64 nanoseconds)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
  struct timeval buffer;
  if (nanoseconds <= 60*60*24*1000000000ULL) { // one day
    buffer.tv_sec = static_cast<uint32>((nanoseconds + 999)/1000000000);
    buffer.tv_usec = static_cast<uint32>((nanoseconds + 999)/1000);
  } else {
    buffer.tv_sec = 60*60*24;
    buffer.tv_usec = 0;
  }
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_SOCKET,
    SO_SNDTIMEO,
    &buffer,
    sizeof(buffer)
  );
#endif
}

bool Socket::getTcpNoDelay() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_TCP,
    TCP_NODELAY,
    &buffer,
    &length
  );
  return buffer != 0;
#endif
}

void Socket::setTcpNoDelay(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = value;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_TCP,
    TCP_NODELAY,
    &buffer,
    sizeof(buffer)
  );
#endif
}

uint64 Socket::getTcpDeferAccept() const
{
#if (defined(TCP_DEFER_ACCEPT))
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    SOL_TCP,
    TCP_DEFER_ACCEPT,
    &buffer,
    &length
  );
  return buffer;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED(); // TAG: fixme
#endif
}

void Socket::setTcpDeferAccept(uint64 value)
{
#if (defined(TCP_DEFER_ACCEPT))
  int buffer = value;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    SOL_TCP,
    TCP_DEFER_ACCEPT,
    &buffer,
    sizeof(buffer)
  );
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED(); // TAG: fixme
#endif
}
 
unsigned int Socket::getTimeToLive() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = 0;
  unsigned int length = sizeof(buffer);
  internal::SocketImpl::getOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_TTL,
    &buffer,
    &length
  );
  return buffer != 0;
#endif
}

void Socket::setTimeToLive(unsigned int value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  int buffer = value;
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_TTL,
    &buffer,
    sizeof(buffer)
  );
#endif
}

uint8 Socket::getMulticastHops() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_HOPS,
      &buffer,
      &length
    );
    return buffer;
  } else {
#endif
    u_char buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_TTL,
      &buffer,
      &length
    );
    return buffer;
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
#endif
}

void Socket::setMulticastHops(uint8 value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = value;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_HOPS,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    u_char buffer = value;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_TTL,
      &buffer,
      sizeof(buffer)
    );
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
#endif
}

bool Socket::getMulticastLoopback() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    u_char buffer;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
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
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_LOOP,
      &buffer,
      &length
    );
    return buffer != 0;
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
#endif
}

void Socket::setMulticastLoopback(bool value)
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = value;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_LOOP,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    u_char buffer = value;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_MULTICAST_LOOP,
      &buffer,
      sizeof(buffer)
    );
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
#endif
}

InetAddress Socket::getMulticastInterface() const
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    unsigned int buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_MULTICAST_IF,
      &buffer,
      &length
    );
    // TAG: fixme - resolve index
    return InetAddress();
  } else {
    int buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
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
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_MULTICAST_IF,
    &buffer,
    &length
  );
  return InetAddress(Cast::getAddress(buffer), InetAddress::IP_VERSION_4);
#endif
#endif
}

void Socket::setMulticastInterface(const InetAddress& interface)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  InetAddress i = interface;
  struct in_addr buffer;
  bassert(i.convertToIPv4(), NetworkException(this));
  copy<uint8>(Cast::getAddress(buffer), i.getIPv4Address(), sizeof(buffer));
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_MULTICAST_IF,
    &buffer,
    sizeof(buffer)
  );
  // TAG: optval = 0 for selecting default interface
#endif
}

uint8 Socket::getUnicastHops() const
{
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_UNICAST_HOPS,
      &buffer,
      &length
    );
    return buffer;
  } else {
#endif
    _throw bindCause(NetworkException(this), NetworkException::OPERATION_NOT_SUPPORTED);
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
}

void Socket::setUnicastHops(uint8 value)
{
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer = 0;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_UNICAST_HOPS,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    _throw bindCause(NetworkException(this), NetworkException::OPERATION_NOT_SUPPORTED);
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  }
#endif
}

void Socket::joinGroup(const InetAddress& group)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  InetAddress g = group;
  bassert(g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  clear(mreq.imr_interface.s_addr); // unspecified
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    struct ipv6_mreq mreq;
    mreq.ipv6mr_interface = 0; // use default interface
    copy<uint8>(
      Cast::getAddress(mreq.ipv6mr_multiaddr),
      group.getAddress(), // if IPv4 we get error
      sizeof(mreq.ipv6mr_multiaddr)
    );
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_JOIN_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else { // IPv4
    InetAddress g = group;
    bassert(g.convertToIPv4(), NetworkException(this));
    struct ip_mreq mreq;
    copy<uint8>(
      Cast::getAddress(mreq.imr_multiaddr.s_addr),
      g.getIPv4Address(),
      sizeof(mreq.imr_multiaddr.s_addr)
    );
    clear(mreq.imr_interface.s_addr); // unspecified
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_ADD_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#  else
  InetAddress g = group;
  bassert(g.convertToIPv4(), NetworkException(this));
  struct ip_mreq mreq;
  copy<uint8>(
    Cast::getAddress(mreq.imr_multiaddr.s_addr),
    g.getIPv4Address(),
    sizeof(mreq.imr_multiaddr.s_addr)
  );
  clear(mreq.imr_interface.s_addr); // unspecified
  internal::SocketImpl::setOption(
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

void Socket::joinGroup(const InetAddress& interface, const InetAddress& group)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  InetAddress i = interface;
  InetAddress g = group;
  bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
  if (socket->getDomain() == Socket::IPV6) {
    struct ipv6_mreq mreq;
    mreq.ipv6mr_interface = 0;
    if (!interface.isUnspecified()) {
      Thread::UseThreadLocalBuffer _buffer;
      Allocator<uint8>& buffer = _buffer;
      struct ifconf ifc;
      ifc.ifc_len = buffer.getSize()/sizeof(char);
      ifc.ifc_buf = (char*)buffer.getElements();
      if (ioctl((SOCKET)socket->getHandle(), SIOCGIFCONF, &ifc)) {
        internal::SocketImpl::raiseNetwork("Unable to resolve interface.");
      }
      const struct ifreq* current = ifc.ifc_req;
      int offset = 0;
      while (offset < ifc.ifc_len) {
        if (ioctl((SOCKET)socket->getHandle(), SIOCGIFADDR, current) != 0) {
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
      bassert(
        mreq.ipv6mr_interface > 0,
        NetworkException("Unable to resolve interface.", this)
      );
    }
    copy<uint8>(
      Cast::getAddress(mreq.ipv6mr_multiaddr),
      group.getAddress(), // if IPv4 we get error
      sizeof(mreq.ipv6mr_multiaddr)
    );
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_JOIN_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else { // IPv4
    InetAddress i = interface;
    InetAddress g = group;
    bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_ADD_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#  else
  InetAddress i = interface;
  InetAddress g = group;
  bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

void Socket::leaveGroup(const InetAddress& interface, const InetAddress& group)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  InetAddress i = interface;
  InetAddress g = group;
  bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_ADD_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#else // unix
#  if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6))
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
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_LEAVE_GROUP,
      &mreq,
      sizeof(mreq)
    );
  } else {
    InetAddress i = interface;
    InetAddress g = group;
    bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
      (SOCKET)socket->getHandle(),
      IPPROTO_IP,
      IP_DROP_MEMBERSHIP,
      &mreq,
      sizeof(mreq)
    );
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#  else
  InetAddress i = interface;
  InetAddress g = group;
  bassert(i.convertToIPv4() && g.convertToIPv4(), NetworkException(this));
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
    (SOCKET)socket->getHandle(),
    IPPROTO_IP,
    IP_DROP_MEMBERSHIP,
    &mreq,
    sizeof(mreq)
  );
#  endif
#endif // flavor
}

bool Socket::getIPv6Restriction() const
{
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6) && defined(IPV6_V6ONLY))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer = 0;
    unsigned int length = sizeof(buffer);
    internal::SocketImpl::getOption(
      (SOCKET)socket->getHandle(),
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
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6) && defined(IPV6_V6ONLY))
  }
#endif
}

void Socket::setIPv6Restriction(bool value)
{
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6) && defined(IPV6_V6ONLY))
  if (socket->getDomain() == Socket::IPV6) {
    int buffer = 0;
    internal::SocketImpl::setOption(
      (SOCKET)socket->getHandle(),
      IPPROTO_IPV6,
      IPV6_V6ONLY,
      &buffer,
      sizeof(buffer)
    );
  } else {
#endif
    _throw bindCause(NetworkException(this), NetworkException::OPERATION_NOT_SUPPORTED);
#if (defined(_COM_AZURE_DEV__BASE__HAVE_INET_IPV6) && defined(IPV6_V6ONLY))
  }
#endif
}

void Socket::setNonBlocking(bool value)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket((SOCKET)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
    internal::SocketImpl::raiseNetwork("Unable to set blocking mode.");
  }
#else // unix
  int flags = 0;
  if ((flags = fcntl((SOCKET)socket->getHandle(), F_GETFL)) == -1) {
    internal::SocketImpl::raiseNetwork("Unable to get flags for socket.");
  }
  if (value) {
    if ((flags & O_NONBLOCK) == 0) { // do we need to set flag
      flags |= O_NONBLOCK;
      if (fcntl((SOCKET)socket->getHandle(), F_SETFL, flags) != 0) {
        internal::SocketImpl::raiseNetwork("Unable to set flags of socke.");
      }
    }
  } else {
    if ((flags & O_NONBLOCK) != 0) { // do we need to clear flag
      flags &= ~O_NONBLOCK;
      if (fcntl((SOCKET)socket->getHandle(), F_SETFL, flags) != 0) {
        internal::SocketImpl::raiseNetwork("Unable to set flags of socket.");
      }
    }
  }
#endif // flavor
}

#if 0
bool Socket::getAsynchronous()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((SOCKET)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     internal::SocketImpl::raiseNetwork("Unable to set blocking mode.");
//   }
#else // unix
  int flags = 0;
  if ((flags = fcntl((SOCKET)socket->getHandle(), F_GETFL)) == -1) {
    internal::SocketImpl::raiseNetwork("Unable to get flags for socket.");
  }
  return flags & FASYNC;
#endif // flavor
}

void Socket::setAsynchronous(bool value)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   unsigned int buffer = value; // set to zero to disable nonblocking
//   if (ioctlsocket((SOCKET)socket->getHandle(), FIONBIO, Cast::pointer<u_long*>(&buffer))) {
//     internal::SocketImpl::raiseNetwork("Unable to set blocking mode.");
//   }
#else // unix
  int flags = 0;
  if ((flags = fcntl((SOCKET)socket->getHandle(), F_GETFL)) == -1) {
    internal::SocketImpl::raiseNetwork("Unable to get flags for socket.");
  }
  if (value) {
    if ((flags & FASYNC) == 0) { // do we need to set flag
      flags |= FASYNC;
      if (fcntl((SOCKET)socket->getHandle(), F_SETFL, flags) != 0) {
        internal::SocketImpl::raiseNetwork("Unable to set flags of socket.");
      }
    }
  } else {
    if ((flags & FASYNC) != 0) { // do we need to clear flag
      flags &= ~FASYNC;
      if (fcntl((SOCKET)socket->getHandle(), F_SETFL, flags) != 0) {
        internal::SocketImpl::raiseNetwork("Unable to set flags of socket.");
      }
    }
  }
#endif // flavor
}
#endif

unsigned int Socket::available() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  unsigned int result = 0;
  if (ioctlsocket((SOCKET)socket->getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
    internal::SocketImpl::raiseNetwork("Unable to determine the amount of data pending in the input buffer.");
  }
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  // this implementation is not very portable?
  int result = 0;
  if (ioctl((SOCKET)socket->getHandle(), FIONREAD, &result)) {
    internal::SocketImpl::raiseNetwork("Unable to determine the amount of data pending in the incoming queue.");
  }
  return result;
#endif // flavor
}

#if 0
unsigned int Socket::pending() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  unsigned int result = 0;
//   if (ioctlsocket((SOCKET)socket->getHandle(), FIONREAD, Cast::pointer<u_long*>(&result))) {
//     internal::SocketImpl::raiseNetwork("Unable to determine the amount of data pending in the input buffer.");
//   }
  return result;
#else // unix
  // this implementation is not very portable?
  int result = 0;
  if (ioctl((SOCKET)socket->getHandle(), TIOCOUTQ, &result)) {
    internal::SocketImpl::raiseNetwork("Unable to determine the amount of data pending in the outgoing queue.");
  }
  return result;
#endif // flavor
}
#endif

void Socket::flush()
{
  Profiler::IOFlushTask profile("Socket::flush()");
}

unsigned int Socket::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking)
{
  Profiler::IOReadTask profile("Socket::read()", buffer);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    int result = ::recv(
      (SOCKET)socket->getHandle(),
      (char*)buffer,
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
        internal::SocketImpl::raiseNetwork("Unable to read from socket.");
      }
    }
#else // unix
    int result = (int)::recv(
      (SOCKET)socket->getHandle(),
      buffer,
      minimum<size_t>(bytesToRead, SSIZE_MAX),
      0
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in nonblocking mode)
        return bytesRead; // try later
      default:
        internal::SocketImpl::raiseNetwork("Unable to read from socket.");
      }
    }
#endif // flavor
    profile.onBytesRead(result);
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
      _throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
#endif
}

unsigned int Socket::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking)
{
  Profiler::IOWriteTask profile("Socket::write()", buffer);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else
  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    int result = ::send(
      (SOCKET)socket->getHandle(),
      (const char*)buffer,
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
        _throw BrokenStream(this);
      default:
        internal::SocketImpl::raiseNetwork("Unable to write to socket.");
      }
    }
#else // unix
    int result = (int)::send(
      (SOCKET)socket->getHandle(),
      buffer,
      minimum<size_t>(bytesToWrite, SSIZE_MAX),
      0
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in nonblocking mode)
        return bytesWritten; // try later
      case EPIPE:
        _throw BrokenStream(this);
      default:
        internal::SocketImpl::raiseNetwork("Unable to write to socket.");
      }
    }
#endif // flavor
    profile.onBytesWritten(result);
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
    if (nonblocking) {
      break;
    }
  }
  return bytesWritten;
#endif
}

unsigned int Socket::receiveFrom(
  uint8* buffer,
  unsigned int size,
  InetAddress& address,
  unsigned short& port)
{
  Profiler::IOReadTask profile("Socket::receiveFrom()", buffer);

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else
  SocketAddress sa;
  socklen sl = sa.getAnySize();
  size = minimum<unsigned int>(size, PrimitiveTraits<int>::MAXIMUM); // silently reduce
  int result = (int)::recvfrom(
    (SOCKET)socket->getHandle(),
    (char*)buffer,
    size,
    0,
    sa.getValue(),
    &sl
  );
  if (result < 0) {
    internal::SocketImpl::raiseNetwork("Unable to receive from.");
  }
  address = sa.getAddress();
  port = sa.getPort();
  profile.onBytesRead(result);
  return result;
#endif
}

unsigned int Socket::sendTo(
  const uint8* buffer,
  unsigned int size,
  const InetAddress& address,
  unsigned short port)
{
  Profiler::IOWriteTask profile("Socket::sendTo()", buffer);

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else
  const SocketAddress sa(address, port, socket->getDomain());
  size = minimum<unsigned int>(size, PrimitiveTraits<int>::MAXIMUM); // silently reduce
  int result = (int)::sendto(
    (SOCKET)socket->getHandle(),
    (const char*)buffer,
    size,
    0,
    sa.getValue(),
    sa.getSize()
  );
  if (result < 0) {
    internal::SocketImpl::raiseNetwork("Unable to send to.");
  }
  profile.onBytesWritten(result);
  return result;
#endif
}

void Socket::asyncCancel()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::CancelIo(socket->getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation Socket::read(
  uint8* buffer,
  unsigned int bytesToRead,
  AsynchronousReadEventListener* listener)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(
    socket->getHandle(),
    buffer,
    bytesToRead,
    listener
  );
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

AsynchronousWriteOperation Socket::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  AsynchronousWriteEventListener* listener)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(
    socket->getHandle(),
    buffer,
    bytesToWrite,
    listener
  );
#else // unix
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif // flavor
}

void Socket::wait() const
{
  Profiler::WaitTask profile("Socket::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);
  
  int result = ::select((SOCKET)socket->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == SOCKET_ERROR) {
    internal::SocketImpl::raiseNetwork("Unable to wait for input.");
  }
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);
  
  int result = ::select((SOCKET)socket->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    internal::SocketImpl::raiseNetwork("Unable to wait for input.");
  }
#endif // flavor
}

bool Socket::wait(unsigned int microseconds) const
{
  Profiler::WaitTask profile("Socket::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);
  
  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;
  
  int result = ::select((SOCKET)socket->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == SOCKET_ERROR) {
    internal::SocketImpl::raiseNetwork("Unable to wait for input.");
  }
  return result != 0; // return true if data available
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((SOCKET)socket->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = microseconds/1000000;
  tv.tv_usec = microseconds % 1000000;

  int result = ::select((SOCKET)socket->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    internal::SocketImpl::raiseNetwork("Unable to wait for input.");
  }
  return result != 0; // return true if data available
#endif // flavor
}

Socket::~Socket()
{
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Socket) : public UnitTest {
public:

  TEST_PRIORITY(500);
  TEST_PROJECT("base/net");
  TEST_IMPACT(NORMAL);
  TEST_EXTERNAL();

  void run() override
  {
    String host = "www.google.com";
    InetAddress address;
    try {
      address = InetAddress::getAddressByName(host); // the address of the remote host
    } catch (HostNotFound&) {
      return;
    }

    Socket s1;
#if 0
    s1.connect(address, 80);
    InetAddress localAddress = s1.getLocalAddress();
    unsigned short localPort = s1.getLocalPort();
    // fout << localAddress << ":" << localPort << ENDL;
#endif
    s1.close();
  }
};

TEST_REGISTER(Socket);

#endif
  
_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
