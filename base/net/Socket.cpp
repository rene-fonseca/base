/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/Socket.h>
#include <base/Functor.h>

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
//  #include <string.h>
#endif

#if !defined(HAVE_TYPE_SOCKLEN_T)
  typedef int socklen_t;
#endif

/*
  __win32__ specific:
    SOCKET is compatible with int (it is really an unsigned int).
    INVALID_SOCKET is equal to int(-1) (it is defined as unsigned int(~0)).

  I have decided not to use these definitions to avoid a platform specific Socket
  interface. Is this going to be a problem?
*/

class SocketAddress {
private:

#if defined(HAVE_INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // HAVE_INET_IPV6
public:

  /**
    Initializes socket address.
  */
  SocketAddress(InetAddress addr, unsigned short port) throw() {
    fill<char>((char*)&sa, sizeof(sa), 0);
#if defined(HAVE_INET_IPV6)
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
#endif // HAVE_INET_IPV6
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



Socket::SocketImpl::SocketImpl() throw() :  handle(-1), remotePort(0), localPort(0) {
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
  SynchronizeExclusively();

  if (this->socket->isCreated()) {
    SynchronizeRelease();
    throw NetworkException("Attempt to overwrite socket");
  }

  // don't know if accept() fills 'sa' with something different from sockaddr_in6 - do you know this
#if defined(HAVE_INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // HAVE_INET_IPV6
  socklen_t sl = sizeof(sa);

  int handle = ::accept(socket.getHandle(), (struct sockaddr*)&sa, &sl);
  if (handle == -1) {
#if defined(__win32__)
    switch (WSAGetLastError()) {
    case WSAEWOULDBLOCK:
      return false;
    default:
      SynchronizeRelease();
      throw NetworkException("Unable to accept connection");
    }
#else // __unix__
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      SynchronizeRelease();
      throw NetworkException("Unable to accept connection");
    }
#endif
  }

  setHandle(handle);
#if defined(HAVE_INET_IPV6)
  this->socket->getRemoteAddress()->setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  this->socket->setRemotePort(ntohs(sa.sin6_port));
#else
  this->socket->getRemoteAddress()->setAddress((char*)&(sa.sin_addr), InetAddress::IPv4);
  this->socket->setRemotePort(ntohs(sa.sin_port));
#endif // HAVE_INET_IPV6
  return true;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  SocketAddress sa(addr, port);
  if (::bind(getHandle(), sa.getValue(), sa.getSize())) {
    SynchronizeRelease();
    throw NetworkException("Unable to associate name with socket");
  }
  *socket->getLocalAddress() = addr;
  socket->setLocalPort(port);
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();
#if defined(__win32__)
  if (::closesocket(getHandle())) {
    SynchronizeRelease();
    throw NetworkException("Unable to close socket");
  }
#else // __unix__
  if (::close(getHandle())) {
    SynchronizeRelease();
    throw NetworkException("Unable to close socket");
  }
#endif
  socket->setHandle(-1); // invalidate socket handle
  socket->setRemotePort(0); // make unconnected
  socket->setLocalPort(0); // make unbound
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();

  SocketAddress sa(addr, port);

  if (::connect(getHandle(), sa.getValue(), sa.getSize())) {
    SynchronizeRelease();
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
#if defined(HAVE_INET_IPV6)
  if (socket->isCreated() || ((handle = ::socket(PF_INET6, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)) {
    SynchronizeRelease();
    throw NetworkException("Unable to create socket");
  }
#else
  if (socket->isCreated() || ((handle = ::socket(PF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)) {
    SynchronizeRelease();
    throw NetworkException("Unable to create socket");
  }
#endif
  setHandle(handle);
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  // synchronization not required
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
  return socket->getLocalPort();
}

void Socket::shutdownInputStream() throw(IOException) {
  SynchronizeExclusively();
#if defined(__win32__)
  if (::shutdown(getHandle(), SD_RECEIVE)) { // disallow further receives
    SynchronizeRelease();
    throw IOException("Unable to shutdown socket for reading");
  }
#else // __unix__
  if (::shutdown(getHandle(), 0)) { // disallow further receives
    SynchronizeRelease();
    throw IOException("Unable to shutdown socket for reading");
  }
#endif
}

void Socket::shutdownOutputStream() throw(IOException) {
  // synchronization not required
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
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), option, &buffer, &len);
  return buffer != 0;
}

void Socket::setBooleanOption(int option, bool value) throw(IOException) {
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
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), SO_RCVBUF, &buffer, &len);
  return buffer;
}

void Socket::setReceiveBufferSize(int size) throw(IOException) {
  int buffer = size;
  setSocketOption(getHandle(), SO_RCVBUF, &buffer, sizeof(buffer));
}

int Socket::getSendBufferSize() const throw(IOException) {
  int buffer;
  socklen_t len = sizeof(buffer);
  getSocketOption(getHandle(), SO_SNDBUF, &buffer, &len);
  return buffer;
}

void Socket::setSendBufferSize(int size) throw(IOException) {
  int buffer = size;
  setSocketOption(getHandle(), SO_SNDBUF, &buffer, sizeof(buffer));
}

void Socket::setNonBlocking(bool value) throw(IOException) {
#if defined(__win32__)
  unsigned int buffer = value; // set to zero to disable nonblocking
  if (ioctlsocket(getHandle(), FIONBIO, &buffer)) {
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
#if defined(__win32__)
  unsigned int result;
  if (ioctlsocket(getHandle(), FIONREAD, &result)) {
    throw IOException("Unable to determine the amount of data pending in the input buffer");
  }
  return result;
#else // __unix__
  struct stat status;
  if (fstat(getHandle(), &status) != 0) {
    throw IOException("Unable to get status of socket");
  }
  return status.st_size;
#endif
}

unsigned int Socket::read(char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesRead = 0;

  while (totalBytesRead < size) {
    int result = ::read(getHandle(), &buffer[totalBytesRead], (size - totalBytesRead) % SSIZE_MAX);

    if (result == 0) { // has end of file been reached
//      if (eof) {
//        throw EndOfFile();
//      }
//      eof = true; // remember end of file
      return totalBytesRead; // early return
    } else if (result == -1) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        // try again
        break;
      case EAGAIN: // no data available (only in non-blocking mode)
        return totalBytesRead; // early return
      default:
        throw IOException("Unable to read from socket");
      }
    } else {
      totalBytesRead += result;
    }
  }

  return totalBytesRead;
}

unsigned int Socket::write(const char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesWritten = 0;

  while (totalBytesWritten < size) {
    int result = ::write(getHandle(), &buffer[totalBytesWritten], (size - totalBytesWritten) % SSIZE_MAX);

    if (result == 0) {
      // do not know if it is possible to end up here
      return totalBytesWritten;
    } else if (result == -1) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        // try again
        break;
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
        return totalBytesWritten; // early return
        break;
      default:
        throw IOException("Unable to write to socket");
      }
    } else {
      totalBytesWritten += result;
    }
  }

  return totalBytesWritten;
}

unsigned int Socket::receiveFrom(char* buffer, unsigned int size, InetAddress& address, unsigned short& port) throw(IOException) {
  unsigned int result = 0;

#if defined(HAVE_INET_IPV6)
  struct sockaddr_in6 sa;
#else
  struct sockaddr_in sa;
#endif // HAVE_INET_IPV6
  socklen_t sl = sizeof(sa);

  if ((result = ::recvfrom(getHandle(),  buffer, size, 0, (struct sockaddr*)&sa, &sl)) == -1) {
    throw IOException("Unable to receive from");
  }

#if defined(HAVE_INET_IPV6)
  // check if really an IPv4 address
  address.setAddress((char*)&(sa.sin6_addr), InetAddress::IPv6);
  port = ntohs(sa.sin6_port);
#else
  address.setAddress((char*)&(sa.sin_addr), InetAddress::IPv4);
  port = ntohs(sa.sin_port);
#endif // HAVE_INET_IPV6

  return result;
}

unsigned int Socket::sendTo(const char* buffer, unsigned int size, InetAddress& address, unsigned short port) throw(IOException) {
  unsigned int result = 0;
  const SocketAddress sa(address, port);
  if ((result = ::sendto(getHandle(), buffer, size, 0, sa.getValue(), sa.getSize())) == -1) {
    throw IOException("Unable to send to");
  }
  return result;
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
