/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/FileSystem.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <limits.h> // defines PATH_MAX
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String FileSystem::getPath(const String& base, const String& relative) throw() {
#if defined(__win32__)
  String result(base.getLength() + sizeof("\\") + relative.getLength());
  result.append(base).append("\\").append(relative);
#else // __unix__
  String result(base.getLength() + sizeof("/") + relative.getLength());
  result.append(base).append("/").append(relative);
#endif
  return result;
}

String FileSystem::getCurrentFolder() throw(FileSystemException) {
#if defined(__win32__)
  Allocator<char>* buffer = Thread::getLocalStorage();
  if (GetCurrentDirectory(buffer->getSize(), buffer->getElements())) {
    throw FileSystemException("Unable to get current folder");
  }
  return String(buffer->getElements());
#else // __unix__
  Allocator<char>* buffer = Thread::getLocalStorage();
  ASSERT(buffer->getSize() > PATH_MAX);
  if (::getcwd(buffer->getElements(), buffer->getSize())) {
    throw FileSystemException("Unable to get current folder");
  }
  return String(buffer->getElements());
#endif
}

void FileSystem::setCurrentFolder(const String& path) throw(FileSystemException) {
#if defined(__win32__)
  if (!SetCurrentDirectory(path.getElements())) {
    throw FileSystemException("Unable to set current folder");
  }
#else // __unix__
  if (::chdir(path.getElements())) {
    throw FileSystemException("Unable to set current folder");
  }
#endif
}

bool FileSystem::fileExists(const String& path) throw(FileSystemException) {
#if defined(__win32__)
  DWORD result = GetFileAttributes(path.getElements());
  return (result != (DWORD)(-1)) && (result & FILE_ATTRIBUTE_DIRECTORY == 0);
#else
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
#endif
}

bool FileSystem::folderExists(const String& path) throw(FileSystemException) {
#if defined(__win32__)
  DWORD result = GetFileAttributes(path.getElements());
  return (result != (DWORD)(-1)) && (result & FILE_ATTRIBUTE_DIRECTORY != 0);
#else
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
#endif
}

void FileSystem::removeFile(const String& path) throw(FileSystemException) {
#if defined(__win32__)
//  SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
  if (!DeleteFile(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#else // __unix__
  if (unlink(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#endif
}

void FileSystem::removeFolder(const String& path) throw(FileSystemException) {
#if defined(__win32__)
  if (!RemoveDirectory(path.getElements())) {
    throw FileSystemException("Unable to remove folder");
  }
#else // __unix__
  if (rmdir(path.getElements())) {
    throw FileSystemException("Unable to remove file");
  }
#endif
}

void FileSystem::makeFolder(const String& path) throw(FileSystemException) {
#if defined(__win32__)
  if (!CreateDirectory(path.getElements(), NULL)) { // use default security descriptor
    throw FileSystemException("Unable to make folder");
  }
#else // __unix__
  if (mkdir(path.getElements(), 0)) {
    throw FileSystemException("Unable to make folder");
  }
#endif
}

/*
String FileSystem::getTempFolder() throw(FileSystemException) {
//#if defined(__win32__)
//  // TAG:  check buffer with ASSERT
//  Allocator<char> buffer = Thread::getLocalStorage();
//  unsigned int length = GetTempPath(buffer->getSize(), buffer->getBytes());
//  return String(buffer->getBytes(), length);
//#else // __unix__
//  // get environment variable: TMP
//  return String("/tmp");
//#endif
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

#if defined(__win32__)
  GetTempFileName(getTempFolder()->getBytes(), , 0, buffer->getBytes());
#else
#endif
}
*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
