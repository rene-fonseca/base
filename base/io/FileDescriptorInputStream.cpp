/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileDescriptorInputStream.h"
#include "EndOfFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#ifndef SSIZE_MAX
#define SSIZE_MAX (1024*1024)
#endif

FileDescriptorInputStream::FileDescriptorInputStream() throw() :
  FileDescriptor() {
}

FileDescriptorInputStream::FileDescriptorInputStream(const FileDescriptor& fd) throw() :
  FileDescriptor(fd),
  eof(false) {
}

FileDescriptorInputStream::FileDescriptorInputStream(FileDescriptorInputStream& copy) throw() :
  FileDescriptor(copy),
  eof(copy.eof) {
}

FileDescriptorInputStream& FileDescriptorInputStream::operator=(FileDescriptorInputStream& eq) throw() {
  if (&eq == this) { // protect against self assignment
    eof = eq.eof;
  }
  return *this;
}

unsigned int FileDescriptorInputStream::available() const throw(IOException) {
  // pipes: returns current bytes in buffer
  // files: returns the total size in bytes
  struct stat status;
  if (::fstat(getHandle(), &status) != 0) {
    throw IOException("Unable to get status of file descriptor");
  }
  return status.st_size;
}

unsigned int FileDescriptorInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesRead = 0;

  while (totalBytesRead < size) {
    int result = ::read(getHandle(), &buffer[totalBytesRead], (size - totalBytesRead) % SSIZE_MAX);

    if (result == 0) { // has end of file been reached
      if (eof) {
        throw EndOfFile();
      }
      eof = true; // remember end of file
      return totalBytesRead; // early return
    } else if (result == -1) { // has an error occured
      switch (errno) { // remember that errno is local to the thread - this simplifies things a lot
      case EINTR: // interrupted by signal before any data was read
        // try again
        break;
      case EAGAIN: // no data available (only in non-blocking mode)
        return totalBytesRead; // early return
      default:
        throw IOException("Unable to read from file decriptor");
      }
    } else {
      totalBytesRead += result;
    }
  }

  return totalBytesRead;
}

unsigned int FileDescriptorInputStream::skip(unsigned int count) throw(IOException) {
  unsigned int temp = count;
  char buffer[1024];
  while (temp) {
    temp -= read((char*)&buffer, sizeof(buffer) <? temp);
  }
  return count - temp;
}

void FileDescriptorInputStream::setNonBlocking(bool value) throw(IOException) {
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
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptorInputStream& value) {
  return stream << "class/FileDescriptorInputStream{"
                << "eof=" << value.eof << ";"
                << "handle=" << value.fd->getHandle()
                << "}";
}
