/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/io/FileDescriptor.h>
#include <base/io/EndOfFile.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <errno.h>

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileDescriptor::Descriptor::~Descriptor() throw(IOException) {
  if (isValid()) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!::CloseHandle(getHandle())) {
      throw IOException("Unable to close file descriptor", this);
    }
#else // unix
    if (::close(getHandle()) != 0) {
      throw IOException("Unable to close file descriptor", this);
    }
#endif // flavor
  }
}



FileDescriptor::FileDescriptor() throw() : fd(FileDescriptor::Descriptor::invalid) {
}

FileDescriptor::FileDescriptor(OperatingSystem::Handle handle) throw() : fd(new Descriptor(handle)) {
}

FileDescriptor::FileDescriptor(const FileDescriptor& copy) throw() : fd(copy.fd) {
}

FileDescriptor& FileDescriptor::operator=(const FileDescriptor& eq) throw() {
  fd = eq.fd; // no need to protect against self assignment
  return *this;
}

void FileDescriptor::close() throw(IOException) {
  fd = Descriptor::invalid;
}

int FileDescriptor::getFlags() const throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else // unix
  int result;
  if ((result = ::fcntl(fd->getHandle(), F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor", this);
  }
  return result;
#endif // flavor
}

void FileDescriptor::setFlags(int flags) throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  if (::fcntl(fd->getHandle(), F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor", this);
  }
#endif // flavor
}

OperatingSystem::Handle FileDescriptor::getHandle() const throw() {
  return fd->getHandle();
}

bool FileDescriptor::isValid() const throw() {
  return fd->isValid();
}

void FileDescriptor::setHandle(OperatingSystem::Handle handle) throw() {
  if (handle != fd->getHandle()) {
    fd = new Descriptor(handle);
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException) {
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

FileDescriptor FileDescriptor::getStandardInput() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_INPUT_HANDLE); // should never fail

  DWORD dontCare;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  return FileDescriptor(handle);
#else // unix
  return FileDescriptor(0);
#endif // flavor
}

FileDescriptor FileDescriptor::getStandardOutput() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_OUTPUT_HANDLE); // should never fail
  
  DWORD dontCare;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  return FileDescriptor(handle);
#else // unix
  return FileDescriptor(1);
#endif // flavor
}

FileDescriptor FileDescriptor::getStandardError() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_ERROR_HANDLE); // should never fail

/*
  bool isServiceContext = false;
  HANDLE consoleOutput = ::CreateFile("CONOUT$", 0, 0, 0, OPEN_EXISTING, 0, 0);
  if (consoleOutput != OperatingSystem::INVALID_HANDLE) {
    isServiceContext = true;
    ::CloseHandle(consoleOutput);
  }
*/
  
  DWORD dontCare;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  return FileDescriptor(handle);
#else // unix
  return FileDescriptor(2);
#endif // flavor
}

FileDescriptor::~FileDescriptor() throw(IOException) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
