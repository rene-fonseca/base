/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/io/FileDescriptor.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <errno.h>

  #if !defined(SSIZE_MAX)
    #define SSIZE_MAX (1024*1024)
  #endif
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

int FileDescriptor::Descriptor::getFlags() const throw(IOException) {
#if defined(__win32__)
  return 0;
#else // __unix__
  int result;
  if ((result = ::fcntl(handle, F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor");
  }
  return result;
#endif
}

void FileDescriptor::Descriptor::setFlags(int flags) throw(IOException) {
#if defined(__win32__)
#else // __unix__
  if (::fcntl(handle, F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor");
  }
#endif
}

void FileDescriptor::Descriptor::setNonBlocking(bool value) throw(IOException) {
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

FileDescriptor::Descriptor::~Descriptor() throw(IOException) {
  if (handle != -1) {
#if defined(__win32__)
    if (!CloseHandle((HANDLE)handle)) {
      throw IOException("Unable to close file descriptor");
    }
#else // __unix__
    if (::close(handle) != 0) {
      throw IOException("Unable to close file descriptor");
    }
#endif
  }
}



FileDescriptor::FileDescriptor() throw() : fd(new Descriptor()) {
}

FileDescriptor::FileDescriptor(int handle) throw() : fd(new Descriptor(handle)) {
}

FileDescriptor::FileDescriptor(const FileDescriptor& copy) throw() : fd(copy.fd) {
}

FileDescriptor& FileDescriptor::operator=(const FileDescriptor& eq) throw() {
  if (&eq != this) { // protect against self assignment
    fd = eq.fd;
  }
  return *this;
}

void FileDescriptor::close() throw(IOException) {
  fd = new Descriptor();
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

bool FileDescriptor::isValid() const throw() {
  return fd->getHandle() != -1;
}

void FileDescriptor::setHandle(int handle) throw() {
  if (handle != fd->getHandle()) {
    fd = new Descriptor(handle);
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException) {
  fd->setNonBlocking(value);
}

FileDescriptor::~FileDescriptor() throw(IOException) {
  TRACE_MEMBER();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value) {
  return stream << "class/FileDescriptor{"
                << "handle=" << value.fd->getHandle()
                << "}";
}

FileDescriptor FileDescriptor::getStandardInput() throw() {
#if defined(__win32__)
  return FileDescriptor((int)GetStdHandle(STD_INPUT_HANDLE));
#else // __unix__
  return FileDescriptor(0);
#endif
}

FileDescriptor FileDescriptor::getStandardOutput() throw() {
#if defined(__win32__)
  return FileDescriptor((int)GetStdHandle(STD_OUTPUT_HANDLE));
#else // __unix__
  return FileDescriptor(1);
#endif
}

FileDescriptor FileDescriptor::getStandardError() throw() {
#if defined(__win32__)
  return FileDescriptor((int)GetStdHandle(STD_ERROR_HANDLE));
#else // __unix__
  return FileDescriptor(2);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
