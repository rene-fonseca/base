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
#include <base/filesystem/FileInfo.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // Unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileInfo::FileInfo(const String& path) throw(FileSystemException) : path(path) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  static const SYSTEMTIME systemTimeOffset = {1970, 1, 0, 0, 0, 0, 0, 0}; // TAG: day starts with 0 or 1
//  FILETIME fileTimeOffset;
//  SystemTimeToFileTime(&systemTimeOffset, &fileTimeOffset);
  static const long long fileTimeOffset = 0x0000001c1a021060LL; // TAG: validate this

  WIN32_FIND_DATA buffer;
  HANDLE handle = FindFirstFile(path.getElements(), &buffer);
  if ((handle == INVALID_HANDLE_VALUE) || (buffer.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
    throw FileSystemException("Not a file");
  }

  ULARGE_INTEGER temp;
  temp.LowPart = buffer.nFileSizeLow;
  temp.HighPart = buffer.nFileSizeHigh;
  size = temp.QuadPart;

  access = *(long long*)(&buffer.ftLastAccessTime) - fileTimeOffset; // TAG: overflow problem
  modification = *(long long*)(&buffer.ftLastWriteTime) - fileTimeOffset; // TAG: overflow problem
  change = *(long long*)(&buffer.ftCreationTime) - fileTimeOffset; // TAG: overflow problem
  FindClose(handle);
#else // Unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (stat64(path.getElements(), &buffer) || (!S_ISREG(buffer.st_mode))) {
      throw FileSystemException("Not a file");
    }
  #else
    struct stat buffer;
    if (stat(path.getElements(), &buffer) || (!S_ISREG(buffer.st_mode))) {
      throw FileSystemException("Not a file");
    }
  #endif
  size = buffer.st_size;
  access = buffer.st_atime;
  modification = buffer.st_mtime;
  change = buffer.st_ctime;
#endif
}

long long FileInfo::getSize() throw() {
  return size;
}

Date FileInfo::getLastModification() throw() {
  return modification;
}

Date FileInfo::getLastAccess() throw() {
  return access;
}

Date FileInfo::getLastChange() throw() {
  return change;
}

String FileInfo::getPath() throw() {
  return path;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
