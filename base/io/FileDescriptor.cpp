/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileDescriptor.h>
#include <base/io/EndOfFile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#ifndef SSIZE_MAX
#define SSIZE_MAX (1024*1024)
#endif

FileDescriptor FileDescriptor::bin(0);
FileDescriptor FileDescriptor::bout(1);
FileDescriptor FileDescriptor::berr(2);

FileDescriptor::Descriptor::Descriptor() throw() {
  handle = -1; // invalid handle
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

int FileDescriptor::Descriptor::getFlags() const throw(IOException) {
  int result;
  if ((result = ::fcntl(handle, F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor");
  }
  return result;
}

void FileDescriptor::Descriptor::setFlags(int flags) throw(IOException) {
  if (::fcntl(handle, F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor");
  }
}

void FileDescriptor::Descriptor::setNonBlocking(bool value) throw(IOException) {
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

FileDescriptor::Descriptor::~Descriptor() throw(IOException) {
  close();
}



FileDescriptor::FileDescriptor(int handle) throw() : fd(new Descriptor(handle)) {
}

FileDescriptor::FileDescriptor(const FileDescriptor& copy) throw() : fd(copy.fd) {
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor& eq) throw() {
  if (&eq == this) { // protect against self assignment
    fd = eq.fd;
  }
  return *this;
}

void FileDescriptor::close() throw(IOException) {
  fd->close();
}

int FileDescriptor::getFlags() const throw(IOException) {
  return fd->getFlags();
}

void FileDescriptor::setFlags(int flags) throw(IOException) {
  fd->setFlags(flags);
}

int FileDescriptor::getHandle() const throw() {
  return fd->getHandle();
}

void FileDescriptor::setHandle(int handle) throw() {
  if (handle != fd->getHandle()) {
    fd = new Descriptor(handle);
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException) {
  fd->setNonBlocking(value);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value) {
  return stream << "class/FileDescriptor{"
                << "handle=" << value.fd->getHandle()
                << "}";
}
