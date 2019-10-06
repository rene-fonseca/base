/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/time.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <string.h> // required by FD_SET on solaris

#  ifndef SSIZE_MAX
#    define SSIZE_MAX (1024*1024)
#  endif
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
// TAG: GLIBC: st_size is not 64bit aligned
struct packedStat64 { // temporary fix for unaligned st_size
  __dev_t st_dev;
  unsigned int __pad1;
  __ino_t __st_ino;
  __mode_t st_mode;
  __nlink_t st_nlink;
  __uid_t st_uid;
  __gid_t st_gid;
  __dev_t st_rdev;
  unsigned int __pad2;
  __off64_t st_size;
  __blksize_t st_blksize;
  __blkcnt64_t st_blocks;
  __time_t st_atime;
  unsigned long int __unused1;
  __time_t st_mtime;
  unsigned long int __unused2;
  __time_t st_ctime;
  unsigned long int __unused3;
  __ino64_t st_ino;
} _DK_SDU_MIP__BASE__PACKED;
#endif // GNU Linux

FileDescriptorInputStream::FileDescriptorInputStream() throw() :
  FileDescriptor(), end(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(
  const FileDescriptor& fd) throw()
  : FileDescriptor(fd), end(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(
  const FileDescriptorInputStream& copy) throw() :
  FileDescriptor(copy), end(copy.end) {
}

FileDescriptorInputStream& FileDescriptorInputStream::operator=(
  const FileDescriptorInputStream& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
    end = eq.end;
  }
  return *this;
}

unsigned int FileDescriptorInputStream::available() const throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD bytesAvailable;
  switch (::GetFileType(fd->getHandle())) {
  case FILE_TYPE_PIPE:
    if (!::PeekNamedPipe((HANDLE)fd->getHandle(), 0, 0, 0, &bytesAvailable, 0)) {
      throw IOException(this);
    }
    break;
  case FILE_TYPE_DISK:
    {
      DWORD highWord;
      long long size = ::GetFileSize((HANDLE)fd->getHandle(), &highWord);
      LARGE_INTEGER position;
      position.QuadPart = 0;
      position.LowPart = ::SetFilePointer((HANDLE)fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
      if ((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
        throw IOException(this);
      }
      bytesAvailable = minimum<long long>(size - position.QuadPart, PrimitiveTraits<unsigned int>::MAXIMUM);
    }
    break;
  case FILE_TYPE_CHAR:
    bytesAvailable = 1; // TAG: fixme
    break;
  default:
    throw IOException(this);
  }
  return bytesAvailable;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
    int result = fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    assert(result == 0, IOException("Unable to get available bytes", this));
    return status.st_size;
#endif // flavor
}

unsigned int FileDescriptorInputStream::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  assert(!end, EndOfFile(this));
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::ReadFile(
      fd->getHandle(),
      buffer,
      bytesToRead,
      &result,
      0
    );
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_BROKEN_PIPE) {
        result = 0;
      } else {
        throw IOException("Unable to read from object", this);
      }
    }
#else // unix
    int result = ::read(
      fd->getHandle(),
      buffer,
      minimum<unsigned int>(bytesToRead, SSIZE_MAX)
    );
    if (result < 0) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        continue; // try again
      case EAGAIN: // no data available (only in non-blocking mode)
//        return bytesRead; // try later
      default:
        throw IOException("Unable to read from object", this);
      }
    }
#endif // flavor
    if (result == 0) { // has end been reached
      end = true;
      if (bytesToRead > 0) {
        throw EndOfFile(this); // attempt to read beyond end of stream
      }
    }
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
  }
  return bytesRead;
}

unsigned int FileDescriptorInputStream::skip(
  unsigned int count) throw(IOException) {
  Allocator<uint8>* buffer = Thread::getLocalStorage();
  unsigned int bytesSkipped = 0;
  while (bytesSkipped < count) {
    unsigned int bytesToRead = minimum(count - bytesSkipped, buffer->getSize());
    bytesSkipped += read(buffer->getElements(), bytesToRead);
  }
  return bytesSkipped;
}

void FileDescriptorInputStream::setNonBlocking(bool value) throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
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
#endif // flavor
}

void FileDescriptorInputStream::wait() const throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(fd->getHandle(), INFINITE);
  ASSERT(result == WAIT_OBJECT_0);
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  int result = ::select(fd->getHandle() + 1, &rfds, 0, 0, 0);
  if (result == -1) {
    throw IOException("Unable to wait for input", this);
  }
#endif // flavor
}

bool FileDescriptorInputStream::wait(
  unsigned int timeout) const throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::WaitForSingleObject(
    fd->getHandle(),
    minimum(timeout, 999999999U)
  );
  return result == WAIT_OBJECT_0;
#else // unix
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd->getHandle(), &rfds);

  struct timeval tv;
  tv.tv_sec = timeout/1000000;
  tv.tv_usec = timeout % 1000000;

  int result = ::select(fd->getHandle() + 1, &rfds, 0, 0, &tv);
  if (result == -1) {
    throw IOException("Unable to wait for input", this);
  }
  return result; // return true if data available
#endif // flavor
}

FileDescriptorInputStream::~FileDescriptorInputStream() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
