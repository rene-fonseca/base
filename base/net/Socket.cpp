/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ByteOrder.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/net/Socket.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <base/platforms/win32/AsyncReadStreamContext.h> // platform specific
  #include <base/platforms/win32/AsyncWriteStreamContext.h> // platform specific
  #include <winsock2.h>
#else // unix
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
  #include <limits.h> // defines SSIZE_MAX
  #include <arpa/inet.h>

  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
    #define BSD_COMP 1 // request BSD flags - don't known if this is ok to do
  #endif
  #include <sys/ioctl.h> // defines FIONREAD
#endif // flavour

// do we need to repair bad header file
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS) && defined(bind)
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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class SocketAddress { // Internet end point
private:

#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif
public:

  inline SocketAddress() throw() {}

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
    assert((addr.getFamily() == InetAddress::IP_VERSION_4) || addr.isIPv4Mapped(), NetworkException("Address not supported"));
    sa.sin_family = AF_INET;
    sa.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
    copy<byte>(getByteAddress(sa.sin_addr), addr.getIPv4Address(), sizeof(struct in_addr));
  #endif // _DK_SDU_MIP__BASE__INET_IPV6
  }

  /** Returns pointer to socket address. */
  inline struct sockaddr* getValue() throw() {return pointer_cast<struct sockaddr*>(&sa);}

  /** Returns pointer to socket address. */
  inline const struct sockaddr* getValue() const throw() {return pointer_cast<const struct sockaddr*>(&sa);}

  /** Returns the size of the socket address structure. */
  inline socklen_t getSize() const throw() {return sizeof(sa);}

  /** Returns the address. */
  inline InetAddress getAddress() const throw() {
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
    switch (sa.sin6_family) {
    case AF_INET:
      return InetAddress(getByteAddress(pointer_cast<const struct sockaddr_in*>(&sa)->sin_addr), InetAddress::IP_VERSION_4);
    case AF_INET6:
      return InetAddress(getByteAddress(pointer_cast<const struct sockaddr_in6*>(&sa)->sin6_addr), InetAddress::IP_VERSION_6);
    default:
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
  #else
    if (sa.sin_family == AF_INET) {
      return InetAddress(getByteAddress(pointer_cast<const struct sockaddr_in*>(&sa)->sin_addr), InetAddress::IP_VERSION_4);
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
      return ByteOrder::fromBigEndian<unsigned short>(pointer_cast<const struct sockaddr_in*>(&sa)->sin_port);
    case AF_INET6:
      return ByteOrder::fromBigEndian<unsigned short>(pointer_cast<const struct sockaddr_in6*>(&sa)->sin6_port);
    default:
      return 0; // TAG: or should I throw an exception
    }
  #else
    if (sa.sin_family == AF_INET) {
      return ByteOrder::fromBigEndian<unsigned short>(pointer_cast<const struct sockaddr_in*>(&sa)->sin_port);
    } else {
      return 0; // TAG: or should I throw an exception
    }
  #endif
  }

  /** Sets the socket name from the specified socket. */
  inline void setSocket(int handle) throw() {
    socklen_t length = getSize();
    ::getsockname(handle, getValue(), &length);
  }
};



void getSocketOption(int handle, int option, void* buffer, socklen_t* len) throw(IOException) {
  // getsockopt is MT-safe
  if (::getsockopt(handle, SOL_SOCKET, option, static_cast<char*>(buffer), len) != 0) {
    throw IOException("Unable to get socket option");
  }
}

void setSocketOption(int handle, int option, const void* buffer, socklen_t len) throw(IOException) {
  // setsockopt is MT-safe
  if (::setsockopt(handle, SOL_SOCKET, option, static_cast<const char*>(buffer), len) != 0) {
    throw IOException("Unable to set socket option");
  }
}



Socket::SocketImpl::SocketImpl(OperatingSystem::Handle _handle) throw() : Handle(_handle), remotePort(0), localPort(0), end(false) {
}

Socket::SocketImpl::~SocketImpl() throw(IOException) {
  if (isValid()) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    if (::closesocket((int)getHandle())) {
      throw NetworkException("Unable to close socket", this);
    }
#else // unix
    if (::close((int)getHandle())) {
      throw NetworkException("Unable to close socket", this);
    }
#endif // flavour
  }
}



Socket::Socket() throw() : socket(SocketImpl::invalid) {
}

bool Socket::accept(Socket& socket) throw(IOException) {
  // ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);

  if (this->socket->isCreated()) {
    throw NetworkException("Attempt to overwrite socket", this);
  }

  SocketAddress sa;
  socklen_t sl = sa.getSize();
  OperatingSystem::Handle handle = (OperatingSystem::Handle)::accept((int)socket.getHandle(), sa.getValue(), &sl);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
  this->socket = new SocketImpl(handle);
  this->socket->setRemoteAddress(sa.getAddress());
  this->socket->setRemotePort(sa.getPort());
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  SocketAddress sa(addr, port);
  if (int rr = ::bind((int)getHandle(), sa.getValue(), sa.getSize())) {
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
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  socket = SocketImpl::invalid;
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  SocketAddress sa(addr, port);
  if (::connect((int)getHandle(), sa.getValue(), sa.getSize())) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
  }
//  sa.setSocket((int)getHandle());
//  socket->setLocalAddress(sa.getAddress());
//  socket->setLocalPort(sa.getPort());
  socket->setRemoteAddress(addr);
  socket->setRemotePort(port);
}

void Socket::create(bool stream) throw(IOException) {
  ExclusiveSynchronize<LOCK> exclusiveSynchronize(*this);
  OperatingSystem::Handle handle;
#if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  if (socket->isCreated() || ((handle = (OperatingSystem::Handle)::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == OperatingSystem::INVALID_HANDLE)) {
    throw NetworkException("Unable to create socket", this);
  }
#else
  if (socket->isCreated() || ((handle = (OperatingSystem::Handle)::socket(PF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == OperatingSystem::INVALID_HANDLE)) {
    throw NetworkException("Unable to create socket", this);
  }
#endif
  socket = new SocketImpl(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  backlog = minimum<int>(backlog, PrimitiveTraits<int>::MAXIMUM); // silently reduce the backlog argument
  if (::listen((int)getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections", this);
  }
}

void Socket::getName() throw() {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  SocketAddress sa;
  sa.setSocket((int)getHandle());
  socket->setLocalAddress(sa.getAddress());
  socket->setLocalPort(sa.getPort());
}

const InetAddress& Socket::getAddress() const throw() {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  return socket->getRemoteAddress();
}

unsigned short Socket::getPort() const throw() {
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const throw() {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  return socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const throw() {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)getHandle(), SD_RECEIVE)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading", this);
  }
#else // unix
  if (::shutdown((int)getHandle(), 0)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading", this);
  }
#endif // flavour
}

void Socket::shutdownOutputStream() throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown((int)getHandle(), SD_SEND)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing", this);
  }
#else // unix
  if (::shutdown((int)getHandle(), 1)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing", this);
  }
#endif // flavour
}

bool Socket::getBooleanOption(int option) const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption((int)getHandle(), option, &buffer, &len);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer = value;
  setSocketOption((int)getHandle(), option, &buffer, sizeof(buffer));
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
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  struct linger buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption((int)getHandle(), SO_LINGER, &buffer, &len);
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
  setSocketOption((int)getHandle(), SO_LINGER, &buffer, sizeof(buffer));
}

int Socket::getReceiveBufferSize() const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption((int)getHandle(), SO_RCVBUF, &buffer, &len);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer = size;
  setSocketOption((int)getHandle(), SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption((int)getHandle(), SO_SNDBUF, &buffer, &len);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int buffer = size;
  setSocketOption((int)getHandle(), SO_SNDBUF, &buffer, sizeof(buffer));
}

void Socket::setNonBlocking(bool value) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket((int)getHandle(), FIONBIO, pointer_cast<u_long*>(&buffer))) {
    throw IOException("Unable to set blocking mode", this);
  }
#else // unix
  int flags;
  if ((flags = fcntl((int)getHandle(), F_GETFL)) == -1) {
    throw IOException("Unable to get flags for socket", this);
  }
  if (value) {
    if (flags & O_NONBLOCK == 0) { // do we need to set flag
      flags |= O_NONBLOCK;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  } else {
    if (flags & O_NONBLOCK != 0) { // do we need to clear flag
      flags &= ~O_NONBLOCK;
      if (fcntl((int)getHandle(), F_SETFL, flags) != 0) {
        throw IOException("Unable to set flags of socket", this);
      }
    }
  }
#endif // flavour
}

unsigned int Socket::available() const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result;
  if (ioctlsocket((int)getHandle(), FIONREAD, pointer_cast<u_long*>(&result))) {
    throw IOException("Unable to determine the amount of data pending in the input buffer", this);
  }
  return result;
#else // unix
  // this implementation is not very portable?
  int result;
  if (ioctl((int)getHandle(), FIONREAD, &result)) {
    throw IOException("Unable to determine the amount of data pending in the input buffer", this);
  }
  return result;
#endif // flavour
}

bool Socket::atEnd() const throw() {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  return socket->atEnd();
}

void Socket::flush() throw(IOException) {
}

unsigned int Socket::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);

  // TAG: currently always blocks
  assert(!socket->atEnd(), EndOfFile());
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::recv((int)socket->getHandle(), buffer, minimum<int>(bytesToRead, PrimitiveTraits<int>::MAXIMUM), 0);
    if (result < 0) { // has an error occured
      switch (::WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was read
        continue; // try again
      case WSAEWOULDBLOCK: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from socket", this);
      }
    }
#else // unix
    int result = ::recv((int)socket->getHandle(), buffer, minimum<unsigned int>(bytesToRead, SSIZE_MAX), 0);
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from socket", this);
      }
    }
#endif // flavour
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      socket->onEnd(); // remember end of file
      throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int Socket::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::send((int)socket->getHandle(), buffer, minimum<int>(bytesToWrite, PrimitiveTraits<int>::MAXIMUM), 0);
    if (result < 0) { // has an error occured
      switch (::WSAGetLastError()) {
      case WSAEINTR: // interrupted by signal before any data was written
        continue; // try again
      case WSAEWOULDBLOCK: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to socket", this);
      }
    }
#else // unix
    int result = ::send((int)socket->getHandle(), buffer, minimum<unsigned int>(bytesToWrite, SSIZE_MAX), 0);
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to socket", this);
      }
    }
#endif // flavour
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

unsigned int Socket::receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException) {
  int result = 0;
  SocketAddress sa;
  socklen_t sl = sa.getSize();
  if ((result = ::recvfrom((int)getHandle(),  buffer, size, 0, sa.getValue(), &sl)) == -1) {
    throw IOException("Unable to receive from", this);
  }
  address = sa.getAddress();
  port = sa.getPort();
  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, const InetAddress& address, unsigned short port) throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  int result = 0;
  const SocketAddress sa(address, port);
  if ((result = ::sendto((int)getHandle(), buffer, size, 0, sa.getValue(), sa.getSize())) == -1) {
    throw IOException("Unable to send to", this);
  }
  return result;
}

void Socket::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(getHandle());
#else // unix
#endif // flavour
}

AsynchronousReadOperation Socket::read(char* buffer, unsigned int bytesToRead, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncReadStreamContext(getHandle(), buffer, bytesToRead, listener);
#endif // flavour
}

AsynchronousWriteOperation Socket::write(const char* buffer, unsigned int bytesToWrite, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException()); // FIXME
  return new win32::AsyncWriteStreamContext(getHandle(), buffer, bytesToWrite, listener);
#endif // flavour
}

void Socket::wait() const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET((int)getHandle(), &rfds);

  int result = ::select((int)getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    throw IOException("Unable to wait for input", this);
  }
}

bool Socket::wait(unsigned int microseconds) const throw(IOException) {
  SharedSynchronize<LOCK> sharedSynchronize(*this);
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
