/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/filesystem/FileSystem.h>
#include <base/concurrency/Thread.h>
#include <base/NotImplemented.h>
#include <base/Type.h>
#include <base/collection/Map.h>
#include <base/Application.h>
#include <base/string/StringOutputStream.h>
#include <base/security/User.h>
#include <base/concurrency/Process.h>
#include <base/net/InetAddress.h>
#include <base/mathematics/Random.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <limits.h> // defines PATH_MAX
  #include <errno.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

unsigned int FileSystem::counter = 0;

String FileSystem::getPath(const String& base, const String& relative) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  String result(base.getLength() + sizeof("\\") + relative.getLength());
  result.append(base).append("\\").append(relative);
#else // unix
  String result(base.getLength() + sizeof("/") + relative.getLength());
  result.append(base).append("/").append(relative);
#endif // flavour
  return result;
}

String FileSystem::getCurrentFolder() throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  Allocator<char>* buffer = Thread::getLocalStorage();
  if (::GetCurrentDirectory(buffer->getSize(), buffer->getElements())) {
    throw FileSystemException("Unable to get current folder", Type::getType<FileSystem>());
  }
  return String(buffer->getElements());
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  ASSERT(buffer->getSize() > PATH_MAX);
  if (::getcwd(buffer->getElements(), buffer->getSize())) {
    throw FileSystemException("Unable to get current folder", Type::getType<FileSystem>());
  }
  return String(buffer->getElements());
#endif // flavour
}

void FileSystem::setCurrentFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetCurrentDirectory(path.getElements())) {
    throw FileSystemException("Unable to set current folder", Type::getType<FileSystem>());
  }
#else // unix
  if (::chdir(path.getElements())) {
    throw FileSystemException("Unable to set current folder", Type::getType<FileSystem>());
  }
#endif // flavour
}

bool FileSystem::fileExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  return (result != (DWORD)(-1)) && (result & FILE_ATTRIBUTE_DIRECTORY == 0);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = stat64(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISREG(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a regular file", Type::getType<FileSystem>());
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists", Type::getType<FileSystem>());
    }
  #else
    struct stat status;
    int result = stat(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISREG(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a regular file", Type::getType<FileSystem>());
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists", Type::getType<FileSystem>());
    }
  #endif
#endif // flavour
}

bool FileSystem::folderExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  return (result != (DWORD)(-1)) && (result & FILE_ATTRIBUTE_DIRECTORY != 0);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = stat64(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISDIR(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a folder", Type::getType<FileSystem>());
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists", Type::getType<FileSystem>());
    }
  #else
    struct stat status;
    int result = stat(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISDIR(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a folder", Type::getType<FileSystem>());
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists", Type::getType<FileSystem>());
    }
  #endif
#endif // flavour
}

void FileSystem::removeFile(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  ::SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
  if (!::DeleteFile(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
  }
#else // unix
  if (unlink(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
  }
#endif // flavour
}

void FileSystem::removeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::RemoveDirectory(path.getElements())) {
    throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
  }
#else // unix
  if (rmdir(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
  }
#endif // flavour
}

void FileSystem::makeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::CreateDirectory(path.getElements(), 0)) { // use default security descriptor
    throw FileSystemException("Unable to make folder", Type::getType<FileSystem>());
  }
#else // unix
  if (mkdir(path.getElements(), 0)) {
    throw FileSystemException("Unable to make folder", Type::getType<FileSystem>());
  }
#endif // flavour
}

String FileSystem::getTempFolder(TemporaryFolder folder) throw() {
  // TAG: need to expand variables (win32) (e.g. set TMP=%HOME%\temp)
  switch (folder) {
  case USER_SPECIFIED:
    {
      const Map<String, String> environment = Application::getApplication()->getEnvironment();
      if (environment.isKey(MESSAGE("TMP"))) {
        return environment[MESSAGE("TMP")];
      } else if (environment.isKey(MESSAGE("TEMP"))) {
        return environment[MESSAGE("TEMP")];
      }
    }
  case MACHINE_NONPERSISTENT:
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    return MESSAGE("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return MESSAGE("/tmp");
#endif // flavour
  case MACHINE_PERSISTENT:
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
    return MESSAGE("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return MESSAGE("/var/tmp");
#endif // flavour
  }
}

String FileSystem::getTempFileName(unsigned int options) throw() {
  bool first = true;
  StringOutputStream stream;
  if (options & PREFIX) {
    stream << '~';
  }
  if (options & USER) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << User::getCurrentUser().getName();
  }
  if (options & APPLICATION) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << Application::getApplication()->getFormalName();
  }
  if (options & PROCESS) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << Process::getProcess().getId();
  }
  if (options & HOST) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << InetAddress::getLocalHost();
  }
  if (options & COUNTER) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << HEX << ZEROPAD << counter++;
  }
  if (options & TIME) {
    if (!first) {
      stream << '-';
    }
    first = false;
    stream << HEX << ZEROPAD << Date();
  }
  if (options & RANDOM) {
    if (!first) {
      stream << '-';
    }
    first = false;
    unsigned int random = Random::getInteger();
    stream << HEX << ZEROPAD << random;
  }
  if (options & SUFFIX) {
    stream << MESSAGE(".tmp");
  }
  stream << FLUSH;
  return stream.getString();
}

File FileSystem::getTempFile(unsigned int options) throw(IOException) {
  throw NotImplemented(Type::getType<FileSystem>());
/*
  unsigned int attempts = 16;
  while (attempts--) {
    // change name policy if to many attempts
    String path = FileSystem::combine(getTempFolder(), getTempFileName());
    File file;
    try {
      file = File(path, opts);
      return file;
    } catch (FileNotFound& e) {
    }
  }
*/
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
