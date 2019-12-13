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
#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <errno.h>

#  ifndef SSIZE_MAX
#    define SSIZE_MAX (1024*1024)
#  endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FileDescriptorOutputStream::FileDescriptorOutputStream() throw()
  : FileDescriptor()
{
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptor& fd) throw()
  : FileDescriptor(fd)
{
}

FileDescriptorOutputStream::FileDescriptorOutputStream(const FileDescriptorOutputStream& copy) throw()
  : FileDescriptor(copy)
{
}

FileDescriptorOutputStream& FileDescriptorOutputStream::operator=(const FileDescriptorOutputStream& assign) throw()
{
  if (&assign != this) { // protect against self assignment
    fd = assign.fd;
  }
  return *this;
}

void FileDescriptorOutputStream::flush() throw(IOException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: handle may or may not be flushable
  // handle to a console output cannot be flushed 'cause it isn't buffered, aarrgh
//  if (!isValid()) {
//    throw IOException("Unable to flush file descriptor.");
//  }
//  ::FlushFileBuffers((HANDLE)fd->getHandle()); // yes ignore any error
// #elif defined(_COM_AZURE_DEV__BASE__USE_FLUSH)
#else // unix
//  ::fsync(fd->getHandle()); // TAG: this is a bug
//  if (ioctl(fd->getHandle(), I_FLUSH, FLUSHRW) != 0) {
//    throw IOException("Unable to flush stream.");
//  }
#endif // flavor
}

unsigned int FileDescriptorOutputStream::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking) throw(IOException) {
  // TAG: currently always blocks
  unsigned int bytesWritten = 0;
  while (bytesToWrite) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::WriteFile(
      fd->getHandle(),
      buffer,
      bytesToWrite,
      &result,
      0
    );
    if (!success) {
      throw IOException("Unable to write to object.");
    }
#else // unix
    int result = (int)::write(fd->getHandle(), buffer, minimum<size_t>(bytesToWrite, SSIZE_MAX));
    if (result < 0) { // has an error occured
      switch (errno) {
      case EINTR: // interrupted by signal before any data was written
        continue; // try again
      case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
//      return 0; // try later
      default:
        throw IOException("Unable to write to object.");
      }
    }
#endif // flavor
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

FileDescriptorOutputStream::~FileDescriptorOutputStream()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
