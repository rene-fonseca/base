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
#include <base/io/FileDescriptor.h>
#include <base/io/EndOfFile.h>
#include <base/Trace.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <errno.h>

  #if !defined(SSIZE_MAX)
    #define SSIZE_MAX (1024*1024)
  #endif
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

int FileDescriptor::Descriptor::getFlags() const throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return 0;
#else // unix
  int result;
  if ((result = ::fcntl(handle, F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor");
  }
  return result;
#endif // flavour
}

void FileDescriptor::Descriptor::setFlags(int flags) throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  if (::fcntl(handle, F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor");
  }
#endif // flavour
}

void FileDescriptor::Descriptor::setNonBlocking(bool value) throw(IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
}

FileDescriptor::Descriptor::~Descriptor() throw(IOException) {
  Trace::message(__PRETTY_FUNCTION__);
  if (handle != OperatingSystem::INVALID_HANDLE) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    if (!::CloseHandle((HANDLE)handle)) {
      Trace::message(__PRETTY_FUNCTION__);
      throw IOException("Unable to close file descriptor");
    }
#else // unix
    if (::close(handle) != 0) {
      throw IOException("Unable to close file descriptor");
    }
#endif // flavour
  }
}



FileDescriptor::Descriptor FileDescriptor::invalid;

FileDescriptor::FileDescriptor() throw() : fd(&invalid) {
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
  fd = &invalid;
}

int FileDescriptor::getFlags() const throw(IOException) {
  return fd->getFlags();
}

void FileDescriptor::setFlags(int flags) throw(IOException) {
  fd->setFlags(flags);
}

OperatingSystem::Handle FileDescriptor::getHandle() const throw() {
  return fd->getHandle();
}

bool FileDescriptor::isValid() const throw() {
  return fd->getHandle() != OperatingSystem::INVALID_HANDLE;
}

void FileDescriptor::setHandle(OperatingSystem::Handle handle) throw() {
  if (handle != fd->getHandle()) {
    fd = new Descriptor(handle);
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException) {
  fd->setNonBlocking(value);
}

FileDescriptor::~FileDescriptor() throw(IOException) {
  TRACE_MEMBER();
  Trace::message(__PRETTY_FUNCTION__);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value) {
  return stream << "class/FileDescriptor{"
                << "handle=" << value.fd->getHandle()
                << "}";
}

FileDescriptor FileDescriptor::getStandardInput() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
}

FileDescriptor FileDescriptor::getStandardOutput() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
}

FileDescriptor FileDescriptor::getStandardError() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
