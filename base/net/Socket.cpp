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

Socket::Socket() throw() {
  handle = -1;
  remotePort = 0;
  localPort = 0;
  inputStream = NULL;
  outputStream = NULL;
}

bool Socket::accept(Socket& socket) throw(IOException) {
  SynchronizeExclusively();

  if (isCreated()) {
    throw NetworkException("Attempt to overwrite socket");
  }

  // don't know if accept() fills sa with something different from sockaddr_in6 - do you know this
  int handle;
  struct sockaddr_in6 sa;
  socklen_t sl = sizeof(sa);

  if ((handle = ::accept(socket.getHandle(), (struct sockaddr*)&sa, &sl)) != 0) {
    switch (errno) {
    case EAGAIN: // EWOULDBLOCK
      return false;
    default:
      throw NetworkException("Unable to accept connection");
    }
  }

  this->handle = handle;
  remoteAddress = InetAddress((char*)&(sa.sin6_addr), InetAddress::IPv6); // *(byte[16]*)&
  remotePort = ntohs(sa.sin6_port);
  return true;
}

unsigned int Socket::available() const throw(IOException) {
  // NOT IMPLEMENTED
  return 0;
}

void Socket::bind(const InetAddress& addr, unsigned short port) throw(IOException) {
  struct sockaddr_in6 socketAddress;
  memset(&socketAddress, 0, sizeof(socketAddress)); // clear structure
#ifdef SIN6_LEN
  socketAddress.sin6_len = sizeof(socketAddress);
#endif
  socketAddress.sin6_family = AF_INET6;
  socketAddress.sin6_port = htons(port);
  memcpy(&socketAddress.sin6_addr, addr.getBytes(), sizeof(struct in6_addr)); // copy address

  if (::bind(handle, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) != 0) {
    throw NetworkException("Unable to assign name to socket");
  }

  localAddress = addr;
  localPort = port;
}

void Socket::close() throw(IOException) {
  SynchronizeExclusively();
  if (::close(handle) != 0) {
    throw NetworkException("Unable to close socket");
  }
  handle = -1; // invalidate socket handle
  remotePort = 0; // make unconnected
  localPort = 0; // make unbound
}

void Socket::connect(const InetAddress& addr, unsigned short port) throw(IOException) {
  SynchronizeExclusively();
  struct sockaddr_in6 socketAddress;
  memset(&socketAddress, 0, sizeof(socketAddress)); // clear structure
#ifdef SIN6_LEN
  socketAddress.sin6_len = sizeof(socketAddress);
#endif
  socketAddress.sin6_family = AF_INET6;
  socketAddress.sin6_port = htons(port);
  memcpy(&socketAddress.sin6_addr, addr.getBytes(), sizeof(struct in6_addr)); // copy address
  if (::connect(handle, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) != 0) {
    switch (errno) {
    case ECONNREFUSED:
//      throw ConnectionRefused("Connection refused");
    case ETIMEDOUT:
//      throw TimeOut("Connection request timed out");
    default:
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
    throw NetworkException("Unable to create socket");
  }
  this->handle = handle;
}

void Socket::listen(unsigned int backlog) throw(IOException) {
  // synchronization not required
  if (backlog > INT_MAX) { // does backlog fit in 'int' type
    backlog = INT_MAX; // silently reduce the backlog argument
  }
  if (::listen(handle, backlog) != 0) { // may also silently limit backlog
    throw NetworkException("Unable to listen for connections");
  }
}

const InetAddress& Socket::getAddress() const throw() {
  return remoteAddress;
}

unsigned short Socket::getPort() const throw() {
  return remotePort;
}

const InetAddress& Socket::getLocalAddress() const throw() {
  return localAddress;
}

unsigned short Socket::getLocalPort() const throw() {
  return localPort;
}

InputStream* Socket::getInputStream() {
  SynchronizeExclusively();
  if (inputStream != NULL) {
    shutdownInputStream();
  }
//  inputStream = new FileInputStream(handle);
  return inputStream;
}

void Socket::shutdownInputStream() throw(IOException) {
/*
  if (shutdown(handle, 0)) {
    throw IOException()
  }
*/
  SynchronizeExclusively();
  delete inputStream;
  inputStream = NULL;
}

OutputStream* Socket::getOutputStream() {
  SynchronizeExclusively();
  if (outputStream != NULL) {
    shutdownOutputStream();
  }
//  outputStream = new FileInputStream(handle);
  return outputStream;
}

void Socket::shutdownOutputStream() throw(IOException) {
/*
  if (shutdown(handle, 1)) {
    throw IOException()
  }
*/
  SynchronizeExclusively();
  delete outputStream;
  outputStream = NULL;
}

FormatOutputStream& Socket::operator<<(FormatOutputStream& stream) const {
  SynchronizeShared();
  stream << "{";
  stream << "connected to=";
  if (isConnected()) {
    stream << "{address=" << remoteAddress << ",port="<< remotePort << "}";
  } else {
    stream << "UNCONNECTED";
  }
  stream << "bound to=";
  if (isBound()) {
    stream << "{address=" << localAddress << ",port="<< localPort << "}";
  } else {
    stream << "UNBOUND";
  }
  stream << "}";
  return stream;
}

Socket::~Socket() throw(IOException) {
  // clean up
}
