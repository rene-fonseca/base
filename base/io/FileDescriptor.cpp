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
#include <base/io/FileDescriptor.h>
#include <base/io/EndOfFile.h>
#include <base/Application.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <io.h>
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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// Handle* FileDescriptor::Descriptor::invalid = nullptr;

FileDescriptor::Descriptor::~Descriptor()
{
  if (isValid()) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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



FileDescriptor::FileDescriptor() throw()
  : fd(FileDescriptor::Descriptor::invalid)
{
}

FileDescriptor::FileDescriptor(OperatingSystem::Handle handle) throw()
  : fd(new Descriptor(handle))
{
}

FileDescriptor::FileDescriptor(const FileDescriptor& copy) throw()
  : fd(copy.fd)
{
}

FileDescriptor& FileDescriptor::operator=(const FileDescriptor& copy) throw()
{
  fd = copy.fd; // no need to protect against self assignment
  return *this;
}

void FileDescriptor::close() throw(IOException)
{
  fd = Descriptor::invalid;
}

bool FileDescriptor::isANSITerminal() const noexcept
{
  // https://bixense.com/clicolors/
  const auto& env = Application::getApplication()->getEnvironment();
  if (auto value = env.find("CLICOLOR_FORCE")) { // defaults to disabled
    if (*value != "0") {
      return true;
    }
  }

  bool atty = FileDescriptor::getStandardOutput().isTerminal();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  {
    DWORD mode = 0;
    BOOL status = GetConsoleMode(fd->getHandle(), &mode);
    if (!status || ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == 0)) { // VT100 compatibility mode
      atty = false;
    }
  }
#endif
  
  if (!atty) {
    return false;
  }
  
  if (auto value = env.find("CLICOLOR")) { // defaults to enabled
    if (*value == "0") {
      return false;
    }
  }

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  if (!env.hasKey("TERM")) { // handle Xcode
    return false;
  }
#endif
  
  return true;
}

bool FileDescriptor::isTerminal() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD type = ::GetFileType(fd->getHandle());
  if (type == FILE_TYPE_CHAR) { // matches: Visual Studio Debug Console, Windows Terminal, Command Promt
    return true;
  }
  return false;
#else // unix
  const int status = isatty(fd->getHandle());
  return (status == 1);
#endif
}

bool FileDescriptor::isPipe() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD type = ::GetFileType(fd->getHandle());
  if (type == FILE_TYPE_PIPE) {
    return true;
  }
  return false;
#else // unix
  // get type directly instead
  // TAG: also not if file
  const int status = lseek(fd->getHandle(), 0, SEEK_CUR);
  return (status < 0) && (errno == ESPIPE);
#endif
}

int FileDescriptor::getFlags() const throw(IOException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
#else // unix
  int result = 0;
  if ((result = ::fcntl(fd->getHandle(), F_GETFL)) < 0) {
    throw IOException("Unable to get flags of file descriptor", this);
  }
  return result;
#endif // flavor
}

void FileDescriptor::setFlags(int flags) throw(IOException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#else // unix
  if (::fcntl(fd->getHandle(), F_SETFL, flags) != 0) {
    throw IOException("Unable to set flags of file descriptor", this);
  }
#endif // flavor
}

OperatingSystem::Handle FileDescriptor::getHandle() const throw()
{
  return fd->getHandle();
}

bool FileDescriptor::isValid() const throw()
{
  return fd->isValid();
}

void FileDescriptor::setHandle(OperatingSystem::Handle handle) throw()
{
  if (handle != fd->getHandle()) {
    fd = new Descriptor(handle);
  }
}

void FileDescriptor::setNonBlocking(bool value) throw(IOException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#else // unix
  int flags = getFlags();
  if (value) {
    if ((flags & O_NONBLOCK) == 0) { // do we need to set flag
      setFlags(flags | O_NONBLOCK);
    }
  } else {
    if ((flags & O_NONBLOCK) != 0) { // do we need to clear flag
      setFlags(flags & ~O_NONBLOCK);
    }
  }
#endif // flavor
}

FileDescriptor FileDescriptor::getStandardInput() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_INPUT_HANDLE); // should never fail

  DWORD dontCare = 0;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  static FileDescriptor fd(handle);
  return fd;
#else // unix
  static FileDescriptor fd(0); // we do not want this to be closed implicitly
  return fd;
#endif // flavor
}

FileDescriptor FileDescriptor::getStandardOutput() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_OUTPUT_HANDLE); // should never fail
  
  DWORD dontCare = 0;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  static FileDescriptor fd(handle);
  return fd;
#else // unix
  static FileDescriptor fd(1); // we do not want this to be closed implicitly
  return fd;
#endif // flavor
}

FileDescriptor FileDescriptor::getStandardError() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  OperatingSystem::Handle handle = ::GetStdHandle(STD_ERROR_HANDLE); // should never fail

/*
  bool isServiceContext = false;
  HANDLE consoleOutput = ::CreateFile("CONOUT$", 0, 0, 0, OPEN_EXISTING, 0, 0);
  if (consoleOutput != OperatingSystem::INVALID_HANDLE) {
    isServiceContext = true;
    ::CloseHandle(consoleOutput);
  }
*/
  
  DWORD dontCare = 0;
  if (::GetNumberOfConsoleMouseButtons(&dontCare) == 0) { // services start without a console
    handle = OperatingSystem::INVALID_HANDLE; // prevent use of fin, fout, and ferr
  } else if (!::GetHandleInformation(handle, &dontCare)) {
    handle = OperatingSystem::INVALID_HANDLE;
  } else if (::GetFileType(handle) == FILE_TYPE_UNKNOWN) {
    handle = OperatingSystem::INVALID_HANDLE;
  }
  static FileDescriptor fd(handle);
  return fd;
#else // unix
  static FileDescriptor fd(2); // we do not want this to be closed implicitly
  return fd;
#endif // flavor
}

FileDescriptor::~FileDescriptor()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
