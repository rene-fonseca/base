/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/FileSystem.h>
#include <base/concurrency/Thread.h>

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
  if (GetCurrentDirectory(buffer->getSize(), buffer->getElements())) {
    throw FileSystemException("Unable to get current folder");
  }
  return String(buffer->getElements());
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();
  ASSERT(buffer->getSize() > PATH_MAX);
  if (::getcwd(buffer->getElements(), buffer->getSize())) {
    throw FileSystemException("Unable to get current folder");
  }
  return String(buffer->getElements());
#endif // flavour
}

void FileSystem::setCurrentFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!SetCurrentDirectory(path.getElements())) {
    throw FileSystemException("Unable to set current folder");
  }
#else // unix
  if (::chdir(path.getElements())) {
    throw FileSystemException("Unable to set current folder");
  }
#endif // flavour
}

bool FileSystem::fileExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = GetFileAttributes(path.getElements());
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
      throw FileSystemException("Not a regular file");
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists");
    }
  #else
    struct stat status;
    int result = stat(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISREG(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a regular file");
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists");
    }
  #endif
#endif // flavour
}

bool FileSystem::folderExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = GetFileAttributes(path.getElements());
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
      throw FileSystemException("Not a folder");
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists");
    }
  #else
    struct stat status;
    int result = stat(path.getElements(), &status);
    switch (result) {
    case 0:
      if (S_ISDIR(status.st_mode)) {
        return true;
      }
      throw FileSystemException("Not a folder");
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists");
    }
  #endif
#endif // flavour
}

void FileSystem::removeFile(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
  if (!DeleteFile(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#else // unix
  if (unlink(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#endif // flavour
}

void FileSystem::removeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!RemoveDirectory(path.getElements())) {
    throw FileSystemException("Unable to remove folder");
  }
#else // unix
  if (rmdir(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#endif // flavour
}

void FileSystem::makeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!CreateDirectory(path.getElements(), NULL)) { // use default security descriptor
    throw FileSystemException("Unable to make folder");
  }
#else // unix
  if (mkdir(path.getElements(), 0)) {
    throw FileSystemException("Unable to make folder");
  }
#endif // flavour
}

/*
String FileSystem::getTempFolder() throw(FileSystemException) {
//#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  // TAG:  check buffer with ASSERT
//  Allocator<char> buffer = Thread::getLocalStorage();
//  unsigned int length = GetTempPath(buffer->getSize(), buffer->getBytes());
//  return String(buffer->getBytes(), length);
//#else // unix
//  // get environment variable: TMP
//  return String("/tmp");
//#endif // flavour
}

String FileSystem::getTempFileName() throw(FileException) {
  // TAG: prefix ~ name of application
  // TAG: options: use application name, use host name, use user name, use process id, use time?
  // TAG: options: overwrite file on creation, exclusive?
  // TAG: options: remove file on object destruction, overwrite file on destruction?
  // TAG: format of name: app-host-id-time.tmp
  // TAG: should return file object created with the appropriate options

// String tempFile;
// StringOutputStream stream(tempFile);
// stream << User::getUser() << "- " << InetAddress::getHostname() << "-" << Process::getProcess().getId() << "-" << ".tmp";

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  GetTempFileName(getTempFolder()->getBytes(), , 0, buffer->getBytes());
#else // unix
#endif // flavour
}
*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
