/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/Pipe.h>
#include <base/Functor.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__)
  #include <windows.h>
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

Pair<Pipe, Pipe> Pipe::make() throw(PipeException) {
#if defined(__win32__)
  // create two anonymous pipes for duplex
#else // __unix__
  int handles[2];
  if (::pipe(handles)) {
    throw PipeException("Unable to create pipe");
  }
  Pipe p;
  Pipe q;
  p.fd = new PipeImpl(handles[0]);
  q.fd = new PipeImpl(handles[1]);
  return makePair(p, q);
#endif
}



Pipe::PipeImpl::~PipeImpl() throw(PipeException) {
#if defined(__win32__)
  if (getHandle() != -1) {
    if (::CloseHandle((HANDLE)getHandle())) {
      throw PipeException("Unable to close pipe");
    }
  }
#else // __unix__
  if (getHandle() != -1) {
    if (::close(getHandle())) {
      throw PipeException("Unable to close pipe");
    }
  }
#endif
}



Pipe::Pipe() throw() : fd(&invalidPipe), end(false) {
}

void Pipe::close() throw(PipeException) {
  fd = &invalidPipe;
  end = true;
}

unsigned int Pipe::getBufferSize() const throw() {
#if defined(__win32__)
  DWORD result;
  GetNamedPipeInfo((HANDLE)fd->getHandle(), NULL, &result, NULL, NULL);
  return result;
#else // __unix__
  return PIPE_BUF;
#endif
}

bool Pipe::atEnd() const throw(PipeException) {
  return end;
}

unsigned int Pipe::available() const throw(PipeException) {
#if defined(__win32__)
  DWORD bytesAvailable;
  if (!::PeekNamedPipe((HANDLE)fd->getHandle(), NULL, 0, NULL, &bytesAvailable, NULL)) {
    throw PipeException("Unable to get available bytes");
  }
  return bytesAvailable;
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::fstat64(fd->getHandle(), &status) != 0) {
      throw PipeException("Unable to get available bytes");
    }
    return status.st_size;
  #else
    struct stat status;
    if (::fstat(fd->getHandle(), &status) != 0) {
      throw PipeException("Unable to get available bytes");
    }
    return status.st_size;
  #endif
#endif
}

unsigned int Pipe::skip(unsigned int count) throw(PipeException) {
  Allocator<char>* buffer = Thread::getLocalStorage();
  unsigned int bytesSkipped = 0;
  while (bytesSkipped < count) {
    unsigned int bytesToRead = minimum(count - bytesSkipped, buffer->getSize());
    bytesSkipped += read(buffer->getElements(), bytesToRead);
  }
  return bytesSkipped;
}

void Pipe::flush() throw(PipeException) {
#if defined(__win32__)
  if (!FlushFileBuffers((HANDLE)fd->getHandle())) {
    throw PipeException("Unable to flush pipe");
  }
#else // __unix__
  int command = FLUSHW;
  if (::ioctl(fd->getHandle(), I_FLUSH, &command)) {
    throw PipeException("Unable to flush pipe");
  }
#endif
}

unsigned int Pipe::read(char* buffer, unsigned int size, bool nonblocking) throw(PipeException) {
  // TAG: currently always blocks
  assert(!end, EndOfFile());
  unsigned int bytesRead = 0;
  while (bytesRead < size) {
#if defined(__win32__)
    DWORD result;
    BOOL success = ::ReadFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_BROKEN_PIPE) {
        result = 0;
      } else {
        throw PipeException("Unable to read from pipe");
      }
    }
#else // __unix__
    int result = ::read(fd->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw PipeException("Unable to read from pipe");
      }
    }
#endif
    if (result == 0) { // has end been reached
      end = true;
      if (bytesRead < size) {
        throw EndOfFile(); // attempt to read beyond end of stream
      }
    }
    bytesRead += result;
  }
  return bytesRead;
}

unsigned int Pipe::write(const char* buffer, unsigned int size, bool nonblocking) throw(PipeException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
#if defined(__win32__)
    DWORD result;
    BOOL success = ::WriteFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL);
    if (!success) {
      throw PipeException("Unable to write to pipe");
    }
#else // __unix__
    int result = ::write(fd->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw PipeException("Unable to write to pipe");
      }
    }
#endif
    bytesWritten += result;
  }
  return bytesWritten;
}

void Pipe::wait() const throw(PipeException) {
#if defined(__win32__)
  // TAG: not implemented
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, NULL);
  if (result == -1) {
    throw PipeException("Unable to wait for input");
  }
#endif
}

bool Pipe::wait(unsigned int timeout) const throw(PipeException) {
#if defined(__win32__)
  // TAG: not implemented
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
    throw PipeException("Unable to wait for input");
  }
  return result; // return true if data available
#endif
}

Pipe::~Pipe() throw(IOException) {
  flush();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
