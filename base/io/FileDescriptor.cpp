/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileDescriptor.h"
#include "EndOfFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#ifndef SSIZE_MAX
#define SSIZE_MAX (1024*1024)
#endif

FileDescriptor::Descriptor::Descriptor() throw() {
  handle = -1;
}

FileDescriptor::Descriptor::Descriptor(int handle) throw() {
  this->handle = handle;
}

void FileDescriptor::Descriptor::close() throw(IOException) {
  if (handle != -1) {
    if (::close(handle) != 0) {
      throw IOException("Unable to close file descriptor");
    }
    handle = -1;
  }
}


/*
FileDescriptor::in(0);
FileDescriptor::out(1);
FileDescriptor::err(2);
*/

int FileDescriptor::getFlags() const throw(IOException) {
  int result;
  if ((result = ::fcntl(handle, F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor");
  }
  return result;
}

void FileDescriptor::setFlags(int flags) throw(IOException) {
  if (::fcntl(handle, F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor");
  }
}

FileDescriptor::FileDescriptor(int handle) throw() {
  this->handle = handle;
  eof = false;
}

FileDescriptor::FileDescriptor(FileDescriptor& copy) throw() {
  handle = copy.handle;
  eof = copy.eof;
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor& eq) throw() {
  if (&eq == this) { // protect against self assignment
    handle = eq.handle;
    eof = eq.eof;
  }
  return *this;
}

FileDescriptor& FileDescriptor::operator=(int handle) throw() {
  this->handle = handle;
  eof = false;
  return *this;
}

unsigned int FileDescriptor::available() throw(IOException) {
  // does not work for files: returns the total size in bytes
  struct stat status;
  if (::fstat(handle, &status) != 0) {
    throw IOException("Unable to get status of file descriptor");
  }
  return status.st_size;
}

void FileDescriptor::close() throw(IOException) {
  if (::close(handle) != 0) {
    throw IOException("Unable to close file descriptor");
  }
  handle = -1;
}

void FileDescriptor::flush() throw(IOException) {
  if (::fdatasync(handle) != 0) {
    throw IOException("Unable to flush file descriptor");
  }
}

int FileDescriptor::getHandle() const throw() {
  return handle;
}

unsigned int FileDescriptor::read(char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesRead = 0;

  while (totalBytesRead < size) {
    int result = ::read(handle, &buffer[totalBytesRead], (size - totalBytesRead) % SSIZE_MAX);

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

void FileDescriptor::setHandle(int handle) throw() {
  if (handle != this->handle) {
    this->handle = handle;
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException) {
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

unsigned int FileDescriptor::write(const char* buffer, unsigned int size) throw(IOException) {
  unsigned int totalBytesWritten = 0;

  while (totalBytesWritten < size) {
    int result = ::write(handle, &buffer[totalBytesWritten], (size - totalBytesWritten) % SSIZE_MAX);

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

FileDescriptor::~FileDescriptor() {
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value) {
  return stream << "{handle=" << value.handle << "}";
}
