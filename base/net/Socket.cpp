/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/net/Socket.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>
#include <base/ByteOrder.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <winsock2.h>
#else // Unix
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
#endif

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

/*
  WIN32 specific:
    SOCKET is compatible with int (it is really an unsigned int).
    INVALID_SOCKET is equal to int(-1) (it is defined as unsigned int(~0)).

  I have decided not to use these definitions to avoid a platform specific Socket
  interface. Is this going to be a problem?
*/

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
    fill<char>((char*)&sa, sizeof(sa), 0);
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
  #if defined(SIN6_LEN)
    sa.sin6_len = sizeof(sa);
  #endif // SIN6_LEN
    sa.sin6_family = AF_INET6;
    sa.sin6_port = ByteOrder::toBigEndian<unsigned short>(port);
    if (addr.getFamily() == InetAddress::IP_VERSION_4) {
      InetAddress temp(addr);
      temp.convertToIPv6();
      copy<byte>((byte*)&sa.sin6_addr, temp.getAddress(), sizeof(struct in6_addr));
    } else {
      copy<byte>((byte*)&sa.sin6_addr, addr.getAddress(), sizeof(struct in6_addr));
    }
  #else // only IPv4 support
    assert((addr.getFamily() == InetAddress::IP_VERSION_4) || addr.isIPv4Mapped(), NetworkException("Address not supported"));
    sa.sin_family = AF_INET;
    sa.sin_port = ByteOrder::toBigEndian<unsigned short>(port);
    copy<byte>((byte*)&(sa.sin_addr), addr.getIPv4Address(), sizeof(struct in_addr));
  #endif // _DK_SDU_MIP__BASE__INET_IPV6
  }

  /** Returns pointer to socket address. */
  inline struct sockaddr* getValue() throw() {return (struct sockaddr*)&sa;}

  /** Returns pointer to socket address. */
  inline const struct sockaddr* getValue() const throw() {return (const struct sockaddr*)&sa;}

  /** Returns the size of the socket address structure. */
  inline socklen_t getSize() const throw() {return sizeof(sa);}

  /** Returns the address. */
  inline InetAddress getAddress() const throw() {
  #if defined(_DK_SDU_MIP__BASE__INET_IPV6)
    switch (sa.sin6_family) {
    case AF_INET:
      return InetAddress((const byte*)&(((const struct sockaddr_in*)&sa)->sin_addr), InetAddress::IP_VERSION_4);
    case AF_INET6:
      return InetAddress((const byte*)&(((const struct sockaddr_in6*)&sa)->sin6_addr), InetAddress::IP_VERSION_6);
    default:
      return InetAddress(); // TAG: or should I throw an exception or just ignore
    }
  #else
    if (sa.sin_family == AF_INET) {
      return InetAddress((const byte*)&(((const struct sockaddr_in*)&sa)->sin_addr), InetAddress::IP_VERSION_4);
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
      return ByteOrder::fromBigEndian<unsigned short>(((const struct sockaddr_in*)&sa)->sin_port);
    case AF_INET6:
      return ByteOrder::fromBigEndian<unsigned short>(((const struct sockaddr_in6*)&sa)->sin6_port);
    default:
      return 0; // TAG: or should I throw an exception
    }
  #else
    if (sa.sin_family == AF_INET) {
      return ByteOrder::fromBigEndian<unsigned short>(((const struct sockaddr_in*)&sa)->sin_port);
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    if (::closesocket(getHandle())) {
      throw NetworkException("Unable to close socket");
    }
#else // Unix
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

  SocketAddress sa;
  socklen_t sl = sa.getSize();
  int handle = ::accept(socket.getHandle(), sa.getValue(), &sl);
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (handle == -1) {
    switch (WSAGetLastError()) {
    case WSAEWOULDBLOCK:
      return false;
    default:
      throw NetworkException("Unable to accept connection");
    }
  }
#else // Unix
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
  this->socket->setRemoteAddress(sa.getAddress());
  this->socket->setRemotePort(sa.getPort());
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  SocketAddress sa(addr, port);
  if (int rr = ::bind(getHandle(), sa.getValue(), sa.getSize())) {
    ferr << "bind result: " << rr << "  errno:" << errno << ENDL;
    throw NetworkException("Unable to assign name to socket");
  }
//  if ((addr.isUnspecified()) || (port == 0)) { // do we need to determine assigned name
//    sa.setSocket(getHandle());
//    socket->setLocalAddress(sa.getAddress());
//    socket->setLocalPort(sa.getPort());
//  } else {
    socket->setLocalAddress(addr);
    socket->setLocalPort(port);
//  }
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();
  socket = new SocketImpl(); // invalidate socket
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  SocketAddress sa(addr, port);
  if (::connect(getHandle(), sa.getValue(), sa.getSize())) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    switch (WSAGetLastError()) {
    case WSAECONNREFUSED:
      throw AccessDenied();
    case WSAETIMEDOUT:
      throw TimedOut();
    default:
      throw NetworkException("Unable to connect to socket");
    }
#else // Unix
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
//  sa.setSocket(getHandle());
//  socket->setLocalAddress(sa.getAddress());
//  socket->setLocalPort(sa.getPort());
  socket->setRemoteAddress(addr);
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
  backlog = minimum<int>(backlog, Int::MAXIMUM); // silently reduce the backlog argument
  if (::listen(getHandle(), backlog)) { // may also silently limit backlog
    throw NetworkException("Unable to set queue limit for incomming connections");
  }
}

void Socket::getName() throw() {
  SynchronizeShared();
  SocketAddress sa;
  sa.setSocket(getHandle());
  socket->setLocalAddress(sa.getAddress());
  socket->setLocalPort(sa.getPort());
}

const InetAddress& Socket::getAddress() const throw() {
  SynchronizeShared();
  return socket->getRemoteAddress();
}

unsigned short Socket::getPort() const throw() {
  return socket->getRemotePort();
}

const InetAddress& Socket::getLocalAddress() const throw() {
  SynchronizeShared();
  return socket->getLocalAddress();
}

unsigned short Socket::getLocalPort() const throw() {
  SynchronizeShared();
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(IOException) {
  SynchronizeShared();
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown(getHandle(), SD_RECEIVE)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading");
  }
#else // Unix
  if (::shutdown(getHandle(), 0)) { // disallow further receives
    throw IOException("Unable to shutdown socket for reading");
  }
#endif
}

void Socket::shutdownOutputStream() throw(IOException) {
  SynchronizeShared();
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::shutdown(getHandle(), SD_SEND)) { // disallow further sends
    throw IOException("Unable to shutdown socket for writing");
  }
#else // Unix
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket(getHandle(), FIONBIO, (u_long*)&buffer)) {
    throw IOException("Unable to set blocking mode");
  }
#else // Unix
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int result;
  if (ioctlsocket(getHandle(), FIONREAD, (u_long*)&result)) {
    throw IOException("Unable to determine the amount of data pending in the input buffer");
  }
  return result;
#else // Unix
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::recv(socket->getHandle(), buffer, minimum<int>(size, Int::MAXIMUM), 0);
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
#else // Unix
    unsigned int bytesToRead = (size <= SSIZE_MAX) ? size : SSIZE_MAX;
    int result = ::recv(socket->getHandle(), buffer, bytesToRead, 0);
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
    bytesRead += result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      socket->onEnd(); // remember end of file
      throw EndOfFile(); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int Socket::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    int result = ::send(socket->getHandle(), buffer, minimum<int>(size, Int::MAXIMUM), 0);
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
#else // Unix
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
  SocketAddress sa;
  socklen_t sl = sa.getSize();
  if ((result = ::recvfrom(getHandle(),  buffer, size, 0, sa.getValue(), &sl)) == -1) {
    throw IOException("Unable to receive from");
  }
  address = sa.getAddress();
  port = sa.getPort();
  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, const InetAddress& address, unsigned short port) throw(IOException) {
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
