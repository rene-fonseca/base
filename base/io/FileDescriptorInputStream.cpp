/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileDescriptorInputStream.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <string.h> // required by FD_SET on solaris

  #ifndef SSIZE_MAX
    #define SSIZE_MAX (1024*1024)
  #endif
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorInputStream::FileDescriptorInputStream() throw() :
  FileDescriptor(), end(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(const FileDescriptor& fd) throw() :
  FileDescriptor(fd), end(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(const FileDescriptorInputStream& copy) throw() :
  FileDescriptor(copy), end(copy.end) {
}

FileDescriptorInputStream& FileDescriptorInputStream::operator=(const FileDescriptorInputStream& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
    end = eq.end;
  }
  return *this;
}

unsigned int FileDescriptorInputStream::available() const throw(IOException) {
#if defined(__win32__)
  // use GetFileSizeEx instead
  DWORD highWord;
  return GetFileSize((HANDLE)fd->getHandle(), &highWord);
#else // __unix__
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::fstat64(fd->getHandle(), &status) != 0) {
      throw IOException("Unable to get available bytes");
    }
    return status.st_size;
  #else
    struct stat status;
    if (::fstat(fd->getHandle(), &status) != 0) {
      throw IOException("Unable to get available bytes");
    }
    return status.st_size;
  #endif
#endif
}

unsigned int FileDescriptorInputStream::read(char* buffer, unsigned int size, bool nonblocking = false) throw(IOException) {
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
        throw IOException("Unable to read from object");
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
        throw IOException("Unable to read from object");
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

unsigned int FileDescriptorInputStream::skip(unsigned int count) throw(IOException) {
  Allocator<char>* buffer = Thread::getLocalStorage();
  unsigned int bytesSkipped = 0;
  while (bytesSkipped < count) {
    unsigned int bytesToRead = minimum(count - bytesSkipped, buffer->getSize());
    bytesSkipped += read(buffer->getElements(), bytesToRead);
  }
  return bytesSkipped;
}

void FileDescriptorInputStream::setNonBlocking(bool value) throw(IOException) {
#if defined(__win32__)
#else // __unix__
  int flags = getFlags();
  if (value) {
    if (flags & O_NONBLOCK == 0) { // do we need to set flag
      setFlags(flags | O_NONBLOCK);
    }
  } else {
    if (flags & O_NONBLOCK != 0) { // do we need to clear flag
      setFlags(flags & ~O_NONBLOCK);
    }
  }
#endif
}

void FileDescriptorInputStream::wait() const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, NULL);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
#endif
}

bool FileDescriptorInputStream::wait(unsigned int timeout) const throw(IOException) {
#if defined(__win32__)
#else // __unix__
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(fd->getHandle() + 1, &rfds, NULL, NULL, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input");
  }
  return result; // return true if data available
#endif
}

FileDescriptorInputStream::~FileDescriptorInputStream() {
  TRACE_MEMBER();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
