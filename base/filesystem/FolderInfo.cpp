/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/filesystem/FolderInfo.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <errno.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FolderInfo::FolderInfo(const String& path) throw(FileSystemException) : path(path) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  static const SYSTEMTIME systemTimeOffset = {1970, 1, 0, 0, 0, 0, 0, 0}; // TAG: day starts with 0 or 1
//  FILETIME fileTimeOffset;
//  SystemTimeToFileTime(&systemTimeOffset, &fileTimeOffset);
  static const long long fileTimeOffset = 0x0000001c1a021060LL; // TAG: validate this

  WIN32_FIND_DATA buffer;
  HANDLE handle = FindFirstFile((path + "\\*").getElements(), &buffer);
  if ((handle == INVALID_HANDLE_VALUE) || !(buffer.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
    throw FileSystemException("Not a folder");
  }

  access = *pointer_cast<const long long*>(&buffer.ftLastAccessTime) - fileTimeOffset; // TAG: overflow problem
  modification = *pointer_cast<const long long*>(&buffer.ftLastWriteTime) - fileTimeOffset; // TAG: overflow problem
  change = *pointer_cast<const long long*>(&buffer.ftCreationTime) - fileTimeOffset; // TAG: overflow problem
  FindClose(handle);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (stat64(path.getElements(), &buffer) || (!S_ISDIR(buffer.st_mode))) {
      throw FileSystemException("Not a folder");
    }
  #else
    struct stat buffer;
    if (stat(path.getElements(), &buffer) || (!S_ISDIR(buffer.st_mode))) {
      throw FileSystemException("Not a folder");
    }
  #endif
  access = buffer.st_atime;
  modification = buffer.st_mtime;
  change = buffer.st_ctime;
#endif // flavor
}

FolderInfo FolderInfo::getParent() const throw(FileSystemException) {
  return *this; // not impl
}

Array<String> FolderInfo::getEntries() const throw(FileSystemException) {
  Array<String> result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE handle;
  WIN32_FIND_DATA entry;

  handle = FindFirstFile((path + "\\*").getElements(), &entry);
  if (handle == INVALID_HANDLE_VALUE) {
    if (GetLastError() != ERROR_NO_MORE_FILES) {
      throw FileSystemException("Unable to read entries of folder");
    }
  } else {
    while (true) {
      result.append(String(entry.cFileName));
      if (!FindNextFile(handle, &entry)) {
        if (GetLastError() == ERROR_NO_MORE_FILES) {
          break;
        }
        FindClose(handle); // avoid that resource leak
        throw FileSystemException("Unable to read entries of folder");
      }
    }

    if (!FindClose(handle)) {
      throw FileSystemException("Unable to close folder");
    }
  }
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();

  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    DIR* directory;

    if ((directory = ::opendir(path.getElements())) == 0) {
      throw FileSystemException("Unable to read entries of folder");
    }

    while (true) {
      int status;
      struct dirent64* entry;

      errno = 0;
      if ((status = ::readdir64_r(directory, pointer_cast<struct dirent64*>(buffer->getElements()), &entry)) != 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        throw FileSystemException("Unable to read entries of folder");
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      throw FileSystemException("Unable to close folder");
    }
  #else
    DIR* directory;

    if ((directory = ::opendir(path.getElements())) == 0) {
      throw FileSystemException("Unable to read entries of folder");
    }

    while (true) {
      int status;
      struct dirent* entry;

      errno = 0;
      if ((status = ::readdir_r(directory, pointer_cast<struct dirent*>(buffer->getElements()), &entry)) != 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        throw FileSystemException("Unable to read entries of folder");
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      throw FileSystemException("Unable to close folder");
    }
  #endif
#endif // flavor
  return result;
}

Date FolderInfo::getLastModification() const throw() {
  return modification;
}

Date FolderInfo::getLastAccess() const throw() {
  return access;
}

Date FolderInfo::getLastChange() const throw() {
  return change;
}

String FolderInfo::getPath() const throw() {
  return path;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
