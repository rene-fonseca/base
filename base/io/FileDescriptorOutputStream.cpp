/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // Unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <errno.h>

  #ifndef SSIZE_MAX
    #define SSIZE_MAX (1024*1024)
  #endif
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptorOutputStream::FileDescriptorOutputStream() throw() :
  FileDescriptor() {
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptor& fd) throw() :
  FileDescriptor(fd) {
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptorOutputStream& copy) throw() :
  FileDescriptor(copy) {
}

FileDescriptorOutputStream& FileDescriptorOutputStream::operator=(const FileDescriptorOutputStream& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
  }
  return *this;
}

void FileDescriptorOutputStream::flush() throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // Handle to a console output cannot be flushed 'cause it isn't buffered, aarrgh
  if (!isValid()) {
    throw IOException("Unable to flush file descriptor");
  }
  ::FlushFileBuffers((HANDLE)fd->getHandle()); // yes ignore any error
#else // Unix
  // TAG: this is a bug
  ::fsync(fd->getHandle());
//  if (ioctl(fd->getHandle(), I_FLUSH, FLUSHRW) != 0) {
//    throw IOException("Unable to flush stream");
//  }
#endif
}

unsigned int FileDescriptorOutputStream::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesWritten < size) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::WriteFile((HANDLE)fd->getHandle(), buffer, size, &result, NULL);
    if (!success) {
      throw IOException("Unable to write to object");
    }
#else // Unix
    int result = ::write(fd->getHandle(), buffer, (size <= SSIZE_MAX) ? size : SSIZE_MAX);
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to object");
      }
    }
#endif
    bytesWritten += result;
  }
  return bytesWritten;
}

FileDescriptorOutputStream::~FileDescriptorOutputStream() {
  TRACE_MEMBER();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
