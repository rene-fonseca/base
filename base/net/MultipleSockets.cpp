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
#include <base/net/MultipleSockets.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SingleExclusiveSynchronize.h>
#include <base/Profiler.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <winsock2.h>
#  undef ERROR
#else // unix
#  include <errno.h>
#if (defined(_COM_AZURE_DEV__BASE__HAVE_POLL))
#  include <poll.h>
#else
#  include <sys/select.h>
#  include <sys/time.h>
#  include <string.h> // memset for solaris
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class MultipleSockets {
  public:

#if (!defined(_COM_AZURE_DEV__BASE__HAVE_POLL))
    struct pollfd {
      OperatingSystem::Handle fd;
      unsigned int events;
      unsigned int revents;
    };
#endif    
  };
};

MultipleSockets::MultipleSockets()
{
  numberOfSelected = 0;
}

void MultipleSockets::add(StreamSocket socket, unsigned int events)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;

  pollfd entry;
  entry.fd = socket.getHandle();
  entry.events = events;
  entry.revents = 0;
  
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    {
      const pollfd* fd = Cast::pointer<const pollfd*>(context.getElements());
      const pollfd* const end = fd + streamSockets.getSize();
      while (fd != end) {
        if (fd->fd == entry.fd) {
          _throw AlreadyKeyException(this);
        }
        ++fd;
      };
    }
    const unsigned int desiredCapacity =
      (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
    if (sizeof(pollfd) * desiredCapacity != context.getSize()) {
      context.setSize(sizeof(pollfd) * desiredCapacity);
    }
    streamSockets.append(socket);
    pollfd* fds = Cast::pointer<pollfd*>(context.getElements());
    fds[streamSockets.getSize() - 1] = entry;
  }
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL))
  struct pollfd fd;
  clear(fd);
  fd.fd = socket.getHandle();
  fd.events |= (events & MultipleSockets::INPUT) ? POLLRDNORM : 0;
  fd.events |= (events & MultipleSockets::PRIORITY_INPUT) ? POLLRDBAND : 0;
  fd.events |= (events & MultipleSockets::HIGH_PRIORITY_INPUT) ? POLLPRI : 0;
  fd.events |= (events & MultipleSockets::OUTPUT) ? POLLWRNORM : 0;
  fd.events |= (events & MultipleSockets::PRIORITY_OUTPUT) ? POLLWRBAND : 0;
  fd.revents = 0;
  
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    {
      const struct pollfd* fd =
        Cast::pointer<const struct pollfd*>(context.getElements());
      const struct pollfd* const end = fd + streamSockets.getSize();
      while (fd != end) {
        if (fd->fd == socket.getHandle()) {
          _throw AlreadyKeyException(this);
        }
        ++fd;
      };
    }
    const unsigned int desiredCapacity =
      (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
    if (sizeof(struct pollfd) * desiredCapacity != context.getSize()) {
      context.setSize(sizeof(struct pollfd) * desiredCapacity);
    }
    streamSockets.append(socket);
    struct pollfd* fds = Cast::pointer<struct pollfd*>(context.getElements());
    fds[streamSockets.getSize() - 1] = fd;
  }
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  pollfd entry;
  entry.fd = socket.getHandle();
  entry.events = events;
  entry.revents = 0;
  
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    {
      const pollfd* fd = Cast::pointer<const pollfd*>(context.getElements());
      const pollfd* const end = fd + streamSockets.getSize();
      while (fd != end) {
        if (fd->fd == entry.fd) {
          _throw AlreadyKeyException(this);
        }
        ++fd;
      };
    }
    const MemorySize desiredCapacity =
      (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
    if (sizeof(pollfd) * desiredCapacity != context.getSize()) {
      context.setSize(sizeof(pollfd) * desiredCapacity);
    }
    streamSockets.append(socket);
    pollfd* fds = Cast::pointer<pollfd*>(context.getElements());
    fds[streamSockets.getSize() - 1] = entry;
  }
#endif
}

void MultipleSockets::remove(
  StreamSocket socket) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fds = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fds[i].fd == socket.getHandle()) {
      streamSockets.remove(i);
      if (fds[i].revents) {
        --numberOfSelected;
      }
      fds[i] = fds[streamSockets.getSize() + 1 - 1];
      const unsigned int desiredCapacity =
        (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
      if (sizeof(pollfd) * desiredCapacity != context.getSize()) {
        context.setSize(sizeof(pollfd) * desiredCapacity);
      }
      return;
    }
  };
  _throw InvalidKey(this);
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  SingleExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fds = Cast::pointer<struct pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fds[i].fd == socket.getHandle()) {
      streamSockets.remove(i);
      if (fds[i].revents) {
        --numberOfSelected;
      }
      fds[i] = fds[streamSockets.getSize() + 1 - 1];
      const unsigned int desiredCapacity =
        (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
      if (sizeof(struct pollfd) * desiredCapacity != context.getSize()) {
        context.setSize(sizeof(struct pollfd) * desiredCapacity);
      }
      return;
    }
  };
  _throw InvalidKey(this);
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fds = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fds[i].fd == socket.getHandle()) {
      streamSockets.remove(i);
      if (fds[i].revents) {
        --numberOfSelected;
      }
      fds[i] = fds[streamSockets.getSize() + 1 - 1];
      const MemorySize desiredCapacity =
        (streamSockets.getSize() + GRANULARITY - 1)/GRANULARITY * GRANULARITY;
      if (sizeof(pollfd) * desiredCapacity != context.getSize()) {
        context.setSize(sizeof(pollfd) * desiredCapacity);
      }
      return;
    }
  };
  _throw InvalidKey(this);
#endif
}

unsigned int MultipleSockets::getEvents(
  StreamSocket socket) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  bassert(socket.isValid(), InvalidKey(this));
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      unsigned int result = fd->revents;
      if (fd->revents) {
        fd->revents = 0;
        --numberOfSelected;
      }
      return result;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  bassert(socket.isValid(), InvalidKey(this));
  SingleExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fd = Cast::pointer<struct pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      unsigned int events = 0;
      events |=
        (fd->revents & POLLRDNORM) ? MultipleSockets::INPUT : 0;
      events |=
        (fd->revents & POLLRDBAND) ? MultipleSockets::PRIORITY_INPUT : 0;
      events |=
        (fd->revents & POLLPRI) ? MultipleSockets::HIGH_PRIORITY_INPUT : 0;
      events |=
        (fd->revents & POLLWRNORM) ? MultipleSockets::OUTPUT : 0;
      events |=
        (fd->revents & POLLWRBAND) ? MultipleSockets::PRIORITY_OUTPUT : 0;
      events |=
        (fd->revents & POLLERR) ? MultipleSockets::ERROR : 0;
      events |=
        (fd->revents & POLLHUP) ? MultipleSockets::DISCONNECTED : 0;
      if (fd->revents) {
        fd->revents = 0;
        --numberOfSelected;
      }
      return events;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  bassert(socket.isValid(), InvalidKey(this));
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      unsigned int result = fd->revents;
      if (fd->revents) {
        fd->revents = 0;
        --numberOfSelected;
      }
      return result;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#endif
}

unsigned int MultipleSockets::getFilter(
  StreamSocket socket) const {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  ExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      return fd->events;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  ExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fd = Cast::pointer<struct pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      unsigned int events = 0;
      events |=
        (fd->events & POLLRDNORM) ? MultipleSockets::INPUT : 0;
      events |=
        (fd->events & POLLRDBAND) ? MultipleSockets::PRIORITY_INPUT : 0;
      events |=
        (fd->events & POLLPRI) ? MultipleSockets::HIGH_PRIORITY_INPUT : 0;
      events |=
        (fd->events & POLLWRNORM) ? MultipleSockets::OUTPUT : 0;
      events |=
        (fd->events & POLLWRBAND) ? MultipleSockets::PRIORITY_OUTPUT : 0;
      return events;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  ExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      return fd->events;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#endif
}

void MultipleSockets::setFilter(
  StreamSocket socket,
  unsigned int events) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      fd->events = events;
      return;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  SingleExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fd = Cast::pointer<struct pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      fd->events = 0;
      fd->events |=
        (events & MultipleSockets::INPUT) ? POLLRDNORM : 0;
      fd->events |=
        (events & MultipleSockets::PRIORITY_INPUT) ? POLLRDBAND : 0;
      fd->events |=
        (events & MultipleSockets::HIGH_PRIORITY_INPUT) ? POLLPRI : 0;
      fd->events |=
        (events & MultipleSockets::OUTPUT) ? POLLWRNORM : 0;
      fd->events |=
        (events & MultipleSockets::PRIORITY_OUTPUT) ? POLLWRBAND : 0;
      return;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    if (fd->fd == socket.getHandle()) {
      fd->events = events;
      return;
    }
    ++fd;
  }
  _throw InvalidKey(this);
#endif
}

unsigned int MultipleSockets::poll()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  
  {
    const pollfd* fd =
      Cast::pointer<const pollfd*>(context.getElements());
    for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
      if (fd->events & MultipleSockets::INPUT) {
        FD_SET((SOCKET)fd->fd, &readfds);
      }
      if (fd->events & MultipleSockets::OUTPUT) {
        FD_SET((SOCKET)fd->fd, &writefds);
      }
      if (fd->events & MultipleSockets::ERROR) {
        FD_SET((SOCKET)fd->fd, &exceptfds);
      }
      ++fd;
    }
  }
  
  int result = ::select(0, &readfds, &writefds, &exceptfds, 0);
  if (result < 0) {
    if (::WSAGetLastError() == WSAEINTR) {
      return 0;
    }
    _throw IOException(this);
  }
  
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    fd->revents = 0;
    if (FD_ISSET((SOCKET)fd->fd, &readfds)) {
      fd->revents |= MultipleSockets::INPUT;
    }
    if (FD_ISSET((SOCKET)fd->fd, &writefds)) {
      fd->revents |= MultipleSockets::OUTPUT;
    }
    if (FD_ISSET((SOCKET)fd->fd, &exceptfds)) {
      fd->revents |= MultipleSockets::ERROR;
    }
    ++fd;
  }
  return result;
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  SingleExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fds = Cast::pointer<struct pollfd*>(context.getElements());
  int result = ::poll(fds, streamSockets.getSize(), -1);
  if (result < 0) {
    if ((errno == EINTR) || (errno == EAGAIN)) {
      return 0;
    }
    _throw IOException(this);
  }
  return result;
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  
  int nfds = 0;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  
  {
    const pollfd* fd =
      Cast::pointer<const pollfd*>(context.getElements());
    for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
      nfds = maximum(nfds, fd->fd);
      if (fd->events & MultipleSockets::INPUT) {
        FD_SET(fd->fd, &readfds);
      }
      if (fd->events & MultipleSockets::OUTPUT) {
        FD_SET(fd->fd, &writefds);
      }
      if (fd->events & MultipleSockets::ERROR) {
        FD_SET(fd->fd, &exceptfds);
      }
      ++fd;
    }
  }
  
  int result = select(nfds + 1, &readfds, &writefds, &exceptfds, 0);
  if (result < 0) {
    if (errno == EINTR) {
      return 0;
    }
    _throw IOException(this);
  }
  
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    fd->revents = 0;
    if (FD_ISSET(fd->fd, &readfds)) {
      fd->revents |= MultipleSockets::INPUT;
    }
    if (FD_ISSET(fd->fd, &writefds)) {
      fd->revents |= MultipleSockets::OUTPUT;
    }
    if (FD_ISSET(fd->fd, &exceptfds)) {
      fd->revents |= MultipleSockets::ERROR;
    }
    ++fd;
  }
  return result;
#endif // flavor
}

unsigned int MultipleSockets::poll(
  unsigned int milliseconds) {
  milliseconds = minimum(milliseconds, 999999999U);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  
  struct timeval timeout;
  timeout.tv_sec = milliseconds/1000;
  timeout.tv_usec = (milliseconds % 1000) * 1000;
  
  {
    const pollfd* fd =
      Cast::pointer<const pollfd*>(context.getElements());
    for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
      if (fd->events & MultipleSockets::INPUT) {
        FD_SET((SOCKET)fd->fd, &readfds);
      }
      if (fd->events & MultipleSockets::OUTPUT) {
        FD_SET((SOCKET)fd->fd, &writefds);
      }
      if (fd->events & MultipleSockets::ERROR) {
        FD_SET((SOCKET)fd->fd, &exceptfds);
      }
      ++fd;
    }
  }
  
  int result = ::select(0, &readfds, &writefds, &exceptfds, &timeout);
  if (result < 0) {
    if (::WSAGetLastError() == WSAEINTR) {
      return 0;
    }
    _throw IOException(this);
  }
  
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    fd->revents = 0;
    if (FD_ISSET((SOCKET)fd->fd, &readfds)) {
      fd->revents |= MultipleSockets::INPUT;
    }
    if (FD_ISSET((SOCKET)fd->fd, &writefds)) {
      fd->revents |= MultipleSockets::OUTPUT;
    }
    if (FD_ISSET((SOCKET)fd->fd, &exceptfds)) {
      fd->revents |= MultipleSockets::ERROR;
    }
    ++fd;
  }
  return result;
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fds = Cast::pointer<pollfd*>(context.getElements());
  int result = ::poll(fds, streamSockets.getSize(), milliseconds);
  if (result < 0) {
    if ((errno == EINTR) || (errno == EAGAIN)) {
      return 0;
    }
    _throw IOException(this);
  }
  return result;
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  
  int nfds = 0;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  
  struct timeval timeout;
  timeout.tv_sec = milliseconds/1000;
  timeout.tv_usec = (milliseconds % 1000) * 1000;
  
  {
    const pollfd* fd =
      Cast::pointer<const pollfd*>(context.getElements());
    for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
      nfds = maximum(nfds, fd->fd);
      if (fd->events & MultipleSockets::INPUT) {
        FD_SET(fd->fd, &readfds);
      }
      if (fd->events & MultipleSockets::OUTPUT) {
        FD_SET(fd->fd, &writefds);
      }
      if (fd->events & MultipleSockets::ERROR) {
        FD_SET(fd->fd, &exceptfds);
      }
      ++fd;
    }
  }
  
  int result = select(nfds + 1, &readfds, &writefds, &exceptfds, &timeout);
  if (result < 0) {
    if (errno == EINTR) {
      return 0;
    }
    _throw IOException(this);
  }
  
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = 0; i < streamSockets.getSize(); ++i) {
    fd->revents = 0;
    if (FD_ISSET(fd->fd, &readfds)) {
      fd->revents |= MultipleSockets::INPUT;
    }
    if (FD_ISSET(fd->fd, &writefds)) {
      fd->revents |= MultipleSockets::OUTPUT;
    }
    if (FD_ISSET(fd->fd, &exceptfds)) {
      fd->revents |= MultipleSockets::ERROR;
    }
    ++fd;
  }
  return result;
#endif // flavor
}

void MultipleSockets::signal(
  SocketListener* listener)
{
  Profiler::pushSignal("MultipleSockets::signal()");

  if (!listener) {
    return;
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = numberOfSelected; i > 0; --i) {
    if (fd->revents) {
      unsigned int events = fd->revents;
      fd->revents = 0; // deselect
      --numberOfSelected;
      listener->onSocketEvent(streamSockets[i], events);
    }
    ++fd;
  }
#elif (defined(_COM_AZURE_DEV__BASE__HAVE_POLL)) // unix (poll)
  SingleExclusiveSynchronize<Guard> _guard(guard);
  struct pollfd* fd =
    Cast::pointer<struct pollfd*>(context.getElements());
  for (unsigned int i = numberOfSelected; i > 0; --i) {
    if ((fd->revents &
         (POLLRDNORM|POLLRDBAND|POLLPRI|POLLWRNORM|POLLWRBAND|POLLERR|POLLHUP))
        ) {
      unsigned int events = 0;
      events |=
        (fd->revents & POLLRDNORM) ? MultipleSockets::INPUT : 0;
      events |=
        (fd->revents & POLLRDBAND) ? MultipleSockets::PRIORITY_INPUT : 0;
      events |=
        (fd->revents & POLLPRI) ? MultipleSockets::HIGH_PRIORITY_INPUT : 0;
      events |=
        (fd->revents & POLLWRNORM) ? MultipleSockets::OUTPUT : 0;
      events |=
        (fd->revents & POLLWRBAND) ? MultipleSockets::PRIORITY_OUTPUT : 0;
      events |=
        (fd->revents & POLLERR) ? MultipleSockets::ERROR : 0;
      events |=
        (fd->revents & POLLHUP) ? MultipleSockets::DISCONNECTED : 0;
      fd->revents = 0; // deselect
      --numberOfSelected;
      listener->onSocketEvent(streamSockets[i], events);
    }
    ++fd;
  }
#else // unix (select)
  typedef internal::MultipleSockets::pollfd pollfd;
  
  SingleExclusiveSynchronize<Guard> _guard(guard);
  pollfd* fd = Cast::pointer<pollfd*>(context.getElements());
  for (unsigned int i = numberOfSelected; i > 0; --i) {
    if (fd->revents) {
      unsigned int events = fd->revents;
      fd->revents = 0; // deselect
      --numberOfSelected;
      listener->onSocketEvent(streamSockets[i], events);
    }
    ++fd;
  }
#endif
}

MultipleSockets::~MultipleSockets() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
