/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/EndOfFile.h>
#include <sys/types.h>
#include <stropts.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#ifndef SSIZE_MAX
#define SSIZE_MAX (1024*1024)
#endif

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
//#if defined(__CYGWIN__)
//#else
//  if (::fdatasync(fd->getHandle()) != 0) {
//    throw IOException("Unable to flush file descriptor");
//  }
//#endif

  fsync(fd->getHandle());
//  if (ioctl(fd->getHandle(), I_FLUSH, FLUSHRW) != 0) {
//    throw IOException("Unable to flush stream");
//  }
}

unsigned int FileDescriptorOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesWritten = 0;

  while (totalBytesWritten < size) {
    int result = ::write(fd->getHandle(), &buffer[totalBytesWritten], (size - totalBytesWritten) % SSIZE_MAX);

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
        throw IOException("Unable to write to file decriptor");
      }
    } else {
      totalBytesWritten += result;
    }
  }

  return totalBytesWritten;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorOutputStream& value) {
  return stream << "class/FileDescriptorOutputStream{"
                << "handle=" << value.fd->getHandle()
                << "}";
}
