/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/Pipe.h>
#include <base/Functor.h>

#if defined(__win32__)
#else // __unix__
  #include <sys/types.h>
  #include <sys/time.h> // defines timeval on Linux systems
  #include <sys/stat.h>
  #include <limits.h> // defines PIPE_BUF...
  #include <unistd.h>
  #include <errno.h>
  #include <stropts.h> // defines FLUSH macros
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Pipe::PipeImpl invalidPipe;

Pair<Pipe, Pipe> Pipe::make() throw(IOException) {
#if defined(__win32__)
#else // __unix__
  int handles[2];
  if (::pipe(handles)) {
    throw IOException("Unable to create pipe");
  }
  Pipe p;
  Pipe q;
  p.pipe = new PipeImpl(handles[0]);
  q.pipe = new PipeImpl(handles[1]);
  return makePair(p, q);
#endif
}



Pipe::PipeImpl::~PipeImpl() throw(IOException) {
  if (getHandle() != -1) {
    if (::close(getHandle())) {
      throw IOException("Unable to close pipe");
    }
  }
}



Pipe::Pipe() throw() : pipe(&invalidPipe) {
}

void Pipe::close() throw(IOException) {
  SynchronizeExclusively();
  pipe = &invalidPipe;
}

unsigned int Pipe::getBufferSize() const throw() {
#if defined(__win32__)
#else // __unix__
  return PIPE_BUF;
#endif
}

bool Pipe::atEnd() const throw(IOException) {
  return false;
}

unsigned int Pipe::available() const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  struct stat status;
  if (fstat(pipe->getHandle(), &status) != 0) {
    throw IOException("Unable to get available bytes of pipe");
  }
  return status.st_size;
#endif
}

unsigned int Pipe::skip(unsigned int count) throw(IOException) {
  return 0; // not implemented
}

void Pipe::flush() throw(IOException) {
#if defined(__win32__)
#else // __unix__
  int command = FLUSHW;
  if (ioctl(pipe->getHandle(), I_FLUSH, &command)) {
    throw IOException("Unable to flush pipe");
  }
#endif
}

unsigned int Pipe::read(char* buffer, unsigned int size) throw(IOException) {
#if defined(__win32__)
  DWORD bytesRead;
  BOOL success = ReadFile((HANDLE)pipe->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX, &bytesRead, NULL);
  if (success) {
    if (bytesRead == 0) { // has end of file been reached
      eof = true; // remember end of file
      return 0;
    }
    return bytesRead;
  } else { // error occured
    throw IOException("Unable to read from file descriptor");
  }
#else // __unix__
  int result = ::read(pipe->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
  if (result == 0) { // has end of file been reached
    return 0; // return
  } else if (result < 0) { // has an error occured
    switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
    case EINTR: // interrupted by signal before any data was read
      return 0; // try later
    case EAGAIN: // no data available (only in non-blocking mode)
      return 0; // try later
    default:
      throw IOException("Unable to read from pipe");
    }
  }
  return result;
#endif
}

unsigned int Pipe::write(const char* buffer, unsigned int size) throw(IOException) {
#if defined(__win32__)
  DWORD bytesWritten;
  BOOL success = WriteFile((HANDLE)pipe->getHandle(), buffer, (size <= INT_MAX) ? size : INT_MAX, &bytesWritten, 0);
  if (success) {
    return bytesWritten;
  } else {
    throw IOException("Unable to write to pipe");
  }
#else // __unix__
  int result = ::write(pipe->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
  if (result == 0) {
    // do not know if it is possible to end up here
    return 0;
  } else if (result == -1) { // has an error occured
    switch (errno) {
    case EINTR: // interrupted by signal before any data was written
      return 0; // try later
    case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
      return 0; // try later
    default:
      throw IOException("Unable to write to pipe");
    }
  }
  return result;
#endif
}

void Pipe::wait() const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(pipe->getHandle(), &rfds);

  int result = ::select(pipe->getHandle() + 1, &rfds, NULL, NULL, NULL);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
#endif
}

bool Pipe::wait(unsigned int timeout) const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(pipe->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(pipe->getHandle() + 1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
  return result; // return true if data available
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
