/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
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
#include <base/ByteOrder.h>

// TAG: need method to traverse links for win32 platforms

#if 0
// TAG: need support for following a specified number of links
// String FileSystem::getFinalPath(const String& path, unsigned int maximumLinks = 8) throw(FileSystemException);

// ../../x/y/../z/..

// current folder "."
// previous folder ".."
// duplicate separators are redundant: "//"

// return true if the path is valid (syntax)
bool FileSystem::isValid(const String& name) throw() {
  // level "." or ".." or ? separated with "/" or "\\"
}

String FileSystem::urlToPath(Url url) throw() {
}

// returns true if the path is a ...
bool FileSystem::isForwardPath(const String& path) throw() {
  return false;
}

bool FileSystem::optimizePath(const String& name) throw() {
  String result;
  String::ReadIterator i = path.getBeginReadIterator();
  String::ReadIterator end = path.getEndReadIterator();
  
  while (i != end) {
    // read until separator and dump
    while ((i != end) && (*i != '/')) {
      ++i;
    }
    
    if (*i == '.') {
      ++i;
      if (i == end) {
        // ignore ending "."
        break;
      }
      if (*i == '.') {
        ++i;
        if ((i == end) || ()) {
          // remove last level
        }
      }
    }
    if (*i == SEPARATOR) {
      
    }
  }
}

bool FileSystem::isName(const String& name) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return (name.indexOf(SEPARATOR) < 0) && (name.indexOf('\\') < 0); // TAG: "C:" drive path
#else
  return name.indexOf(SEPARATOR) < 0;
#endif
}

// returns true if the specified path is a subpath of the root path
bool FileSystem::isSubPathOf(const String& root, const String& path) throw() {
  if (isAbsolutePath(path)) {
    return path.startsWith(root);
  } else {
    String subpath = toAbsolutePath(path);
    return subpath.startsWith(root);
  }
}
#endif





#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _WIN32_WINNT 0x500
#  include <windows.h>
#  include <winioctl.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h> // lstat
// TAG: need more file system support
// #  include <ustat.h>
// #  include <sys/statvfs.h>
#  include <fcntl.h>
#  include <unistd.h> // readlink, symlink
#  include <limits.h> // defines PATH_MAX
#  include <errno.h>

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
#  include <sys/quota.h>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
#  include <sys/fs/ufs_quota.h>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
#  include <sys/quota.h>
#endif

#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
// TAG: GLIBC: st_size is not 64bit aligned
struct packedStat64 { // temporary fix for unaligned st_size
  __dev_t st_dev;
  unsigned int __pad1;
  __ino_t __st_ino;
  __mode_t st_mode;
  __nlink_t st_nlink;
  __uid_t st_uid;
  __gid_t st_gid;
  __dev_t st_rdev;
  unsigned int __pad2;
  __off64_t st_size;
  __blksize_t st_blksize;
  __blkcnt64_t st_blocks;
  __time_t st_atime;
  unsigned long int __unused1;
  __time_t st_mtime;
  unsigned long int __unused2;
  __time_t st_ctime;
  unsigned long int __unused3;
  __ino64_t st_ino;
} _DK_SDU_MIP__BASE__PACKED;
#endif // GNU Linux

unsigned int FileSystem::counter = 0;
int FileSystem::cachedSupportsLinks = -1; // -1 not cached, 0 false, and 1 true

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
const unsigned int FileSystem::MAXIMUM_PATH_LENGTH = MAX_PATH;
#else // unix
const unsigned int FileSystem::MAXIMUM_PATH_LENGTH = PATH_MAX;
#endif // flavor

String FileSystem::getPath(const String& base, const String& relative) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  String result(base.getLength() + sizeof("\\") + relative.getLength());
  result.append(base).append("\\").append(relative);
#else // unix
  String result(base.getLength() + sizeof("/") + relative.getLength());
  result.append(base).append("/").append(relative);
#endif // flavor
  return result;
}

String FileSystem::getComponent(const String& path, Component component) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int forward = path.lastIndexOf('/');
  int backward = path.lastIndexOf('\\');
  int separator = maximum(forward, backward);
#else // unix
  int separator = path.lastIndexOf(SEPARATOR);
#endif // flavor
  switch (component) {
  case FileSystem::FOLDER_PATH:
    if (separator > 0) {
      return path.substring(0, separator);
    }
    break;
  case FileSystem::FILENAME:
    return path.substring(separator + 1);
  case FileSystem::NAME:
    {
      int dot = path.indexOf('.', separator + 1);
      return path.substring(separator + 1, dot);
    }
  case FileSystem::DOTEXTENSION:
    {
      int dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot);
      }
    }
    break;
  case FileSystem::EXTENSION:
    {
      int dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot + 1);
      }
    }
    break;
  }
  return String();
}

bool FileSystem::isAbsolutePath(const String& path) throw() {
  String::ReadIterator i = path.getBeginReadIterator();
  String::ReadIterator end = path.getEndReadIterator();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ((i < end) && ASCIITraits::isAlpha(*i++) && (i < end) && (*i++ == ':') && (i < end) && (*i == SEPARATOR));
#else // unix
  return (i < end) && (*i == SEPARATOR);
#endif // flavor
}

bool FileSystem::isFolderPath(const String& path) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int length = path.getLength();
  if (length == 0) {
    return false;
  }
  char lastChar = path[length - 1];
  return (lastChar == '/') || (lastChar == '\\');
#else // unix
  return path.endsWith(MESSAGE("/"));
#endif // flavor
}

String FileSystem::toAbsolutePath(const String& base, const String& path) throw(FileSystemException) {
  if (!base.isProper() || isAbsolutePath(path)) {
    return path;
  }
  String result = base;
  result.append(SEPARATOR);
  result.append(path);
  
  // remove duplicate separators
  String::Iterator i = result.getBeginIterator();
  bool previousIsSeparator = /*(i < end) &&*/ (*i == SEPARATOR); // content of comment not required
  ++i;
  String::Iterator j = i;
  String::Iterator end = result.getEndIterator();
  while (i < end) {
    if (*i == SEPARATOR) {
      if (previousIsSeparator) {
        ++i; // skip separator
        continue;
      }
      previousIsSeparator = true;
    } else {
      previousIsSeparator = false;
    }
    *j++ = *i++;
  }
  result.forceToLength(j - result.getBeginIterator());
  return result;
}

String FileSystem::findFile(const Array<String>& searchPaths, const String& relative, unsigned int index) throw(FileSystemException) {
  if (relative.isProper()) {
    if (isAbsolutePath(relative)) {
      return relative;
    } else {
      Array<String>::ReadIterator i = searchPaths.getBeginReadIterator();
      const Array<String>::ReadIterator end = searchPaths.getEndReadIterator();
      i += index;
      while (i < end) {
        String absolutePath = FileSystem::toAbsolutePath(*i, relative);
        if (FileSystem::fileExists(absolutePath)) {
          return absolutePath;
        }
        ++i;
      }
    }
  }
  return String();
}

String FileSystem::toUrl(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const Literal PREFIX = MESSAGE("file:///");
#else // unix
  static const Literal PREFIX = MESSAGE("file://");
#endif // flavor
  if (isAbsolutePath(path)) {
    throw FileSystemException(Type::getType<FileSystem>());
  }
  String result = PREFIX + path; // e.g. "file:///C:/"
  if (SEPARATOR != '/') {
    // replace all separators with "/" and replace duplicate separators (but leave prefix)
    String::Iterator i = result.getBeginIterator();
    i += PREFIX.getLength(); // skip prefix
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    *i++ = ASCIITraits::toUpper(*i); // make device uppercase
    ++i; // skip semicolon
#endif // flavor
    String::Iterator j = i;
    String::Iterator end = result.getEndIterator();
    bool previousIsSeparator = false;
    while (i < end) {
      if (*i == SEPARATOR) {
        ++i;
        if (!previousIsSeparator) {
          previousIsSeparator = true;
          *j++ = '/';
        }
      } else {
        previousIsSeparator = false;
        *j++ = *i++;
      }
    }
  }
  return result;
}

String FileSystem::getCurrentFolder() throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Allocator<char>* buffer = Thread::getLocalStorage();
  DWORD length = ::GetCurrentDirectory(buffer->getSize(), buffer->getElements());
  if (length == 0) {
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
#endif // flavor
}

void FileSystem::setCurrentFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetCurrentDirectory(path.getElements())) {
   throw FileSystemException("Unable to set current folder", Type::getType<FileSystem>());
  }
#else // unix
  if (::chdir(path.getElements())) {
    throw FileSystemException("Unable to set current folder", Type::getType<FileSystem>());
  }
#endif // flavor
}

unsigned int FileSystem::getType(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: follow link
  unsigned int flags = 0;
  HANDLE file = ::CreateFile(
    path.getElements(), // file name
    FILE_READ_ATTRIBUTES | FILE_READ_EA /*| READ_CONTROL*/, // access mode
    FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
    0, // security descriptor
    OPEN_EXISTING, // how to create
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, // file attributes
    0
  ); // handle to template file
  if (file == INVALID_HANDLE_VALUE) {
//     switch (::GetLastError()) {
//     case ERROR_ACCESS_DENIED:
//     case ERROR_SHARING_VIOLATION: // possible with page file
//     case ERROR_LOCK_VIOLATION: // TAG: is this ok
//       throw FileSystemException(Type::getType<FileSystem>());
//     }
    
    WIN32_FIND_DATA information;
    HANDLE find = ::FindFirstFileExA(
      path.getElements(),
      FindExInfoStandard,
      &information,
      FindExSearchNameMatch,
      0,
      0
    );
    assert(find != INVALID_HANDLE_VALUE, FileSystemException(Type::getType<FileSystem>()));
    ::FindClose(find);
    if (information.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) {
      flags |= FileSystem::DEVICE;
    }
    if (information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      flags |= FileSystem::FOLDER;
    }
    if (information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
      flags |= FileSystem::LINK;
    }
    if (information.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
      // flags |= FileSystem::SYSTEM;
    }
    return flags;
  }
  
  BY_HANDLE_FILE_INFORMATION information;
  bool error = ::GetFileInformationByHandle(file, &information) == 0;
  if (error) {
    ::CloseHandle(file);
  }
  assert(!error, FileSystemException(Type::getType<FileSystem>()));
  
  if (information.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) {
    flags |= FileSystem::DEVICE;
  }
  if (information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    flags |= FileSystem::FOLDER;
  }
  if (information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
    flags |= FileSystem::LINK;
  }
  if (information.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
    //  flags |= FileSystem::SYSTEM;
  }
  
  switch (::GetFileType(file)) {
  case FILE_TYPE_UNKNOWN:
    flags |= 0;
    break;
  case FILE_TYPE_DISK:
    if (flags == 0) {
      flags |= FileSystem::REGULAR;
    }
    break;
  case FILE_TYPE_CHAR:
    flags |= FileSystem::CHARACTER;
    break;
  case FILE_TYPE_PIPE:
    flags |= FileSystem::SOCKET; // TAG: what flag should we use
    break;
  }
  ::CloseHandle(file);
  return flags;
#else // unix
#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
    int result = stat64(path.getElements(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = stat64(path.getElements(), &status);
  #endif // GNU Linux
#else
  struct stat status;
  int result = stat(path.getElements(), &status);
#endif
  assert(result == 0, FileSystemException("Unable to query entry", Type::getType<FileSystem>()));
  unsigned int flags = 0;
  if (S_ISBLK(status.st_mode)) {
    flags |= FileSystem::BLOCK;
  }
  if (S_ISCHR(status.st_mode)) {
    flags |= FileSystem::CHARACTER;
  }
  if (S_ISDIR(status.st_mode)) {
    flags |= FileSystem::FOLDER;
  }
  if (S_ISFIFO(status.st_mode)) {
    flags |= FileSystem::FIFO;
  }
  if (S_ISREG(status.st_mode)) {
    flags |= FileSystem::REGULAR;
  }
  if (S_ISLNK(status.st_mode)) {
    flags |= FileSystem::LINK;
  }
  if (S_ISSOCK(status.st_mode)) {
    flags |= FileSystem::SOCKET;
  }

  #if defined(S_TYPEISMQ)
    if (S_TYPEISMQ(&status)) {
      flags |= FileSystem::MESSAGE_QUEUE;
    }
  #endif // S_TYPEISMQ
  #if defined(S_TYPEISSEM)
    if (S_TYPEISSEM(&status)) {
      flags |= FileSystem::SEMPAHORE;
    }
  #endif // S_TYPEISEM
  #if defined(S_TYPEISSHM)
    if (S_TYPEISSHM(&status)) {
      flags |= FileSystem::SHARED_MEMORY;
    }
  #endif // S_TYPEISSHM
  return flags;
#endif // flavor
}

uint64 FileSystem::getSize(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WIN32_FIND_DATA information;
  HANDLE handle = ::FindFirstFile(path.getElements(), &information);
  assert(
    handle != INVALID_HANDLE_VALUE,
    FileSystemException("Unable to get size of file", Type::getType<FileSystem>())
  );
  ::FindClose(handle);
  return static_cast<uint64>(information.nFileSizeHigh) | information.nFileSizeLow;
#else // unix
#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
    int result = stat64(path.getElements(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = stat64(path.getElements(), &status);
  #endif // GNU Linux
#else
  struct stat status;
  int result = stat(path.getElements(), &status);
#endif
  assert(result == 0, FileSystemException("Unable to get size of file", Type::getType<FileSystem>()));
  return status.st_size;
#endif // flavor
}

bool FileSystem::entryExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  WIN32_FIND_DATA information;
  HANDLE handle = ::FindFirstFile(path.getElements(), &information);
  if (handle == INVALID_HANDLE_VALUE) {
    assert(
      ::GetLastError() == ERROR_FILE_NOT_FOUND,
      FileSystemException("Unable to examine if entry exists", Type::getType<FileSystem>())
    );
    return false;
  }
  ::FindClose(handle);
  return true;
#else // unix
#  if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
#    if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
  int result = stat64(path.getElements(), (struct stat64*)&status);
#    else
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
#    endif // GNU Linux
  if (result == 0) {
    return true;
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if entry exists", Type::getType<FileSystem>());
    }
  }
#  else
  struct stat status;
  int result = stat(path.getElements(), &status);
  if (result == 0) {
    return true;
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if entry exists", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

bool FileSystem::fileExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  if (result == INVALID_FILE_ATTRIBUTES) {
    // TAG: need support for no access
    return false;
  }
  // TAG: could check if (GetLastError() == ERROR_FILE_NOT_FOUND)
  return ((result & FILE_ATTRIBUTE_DIRECTORY) == 0) && ((result & FILE_ATTRIBUTE_DEVICE) == 0);
  // we ignore FILE_ATTRIBUTE_REPARSE_POINT
#else // unix
#  if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
#    if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
  int result = stat64(path.getElements(), (struct stat64*)&status);
#    else
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
#    endif // GNU Linux
  if (result == 0) {
    return S_ISREG(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists", Type::getType<FileSystem>());
    }
  }
#  else
  struct stat status;
  int result = stat(path.getElements(), &status);
  if (result == 0) {
    return S_ISREG(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if file exists", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

bool FileSystem::folderExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  if (result == INVALID_FILE_ATTRIBUTES) {
    // TAG: need support for no access
    return false;
  }
  // TAG: could check if (GetLastError() == ERROR_FILE_NOT_FOUND)
  return ((result & FILE_ATTRIBUTE_DIRECTORY) != 0); // we ignore FILE_ATTRIBUTE_REPARSE_POINT
#else // unix
#  if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
#    if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
  int result = stat64(path.getElements(), (struct stat64*)&status);
#    else
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
#    endif // GNU Linux
  if (result == 0) {
    return S_ISDIR(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists", Type::getType<FileSystem>());
    }
  }
#  else
  struct stat status;
  int result = stat(path.getElements(), &status);
  if (result == 0) {
    return S_ISDIR(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException("Unable to examine if folder exists", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

void FileSystem::removeFile(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//  ::SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
  if (!::DeleteFile(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
  }
#else // unix
  if (unlink(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
  }
#endif // flavor
}

void FileSystem::removeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD attributes = ::GetFileAttributes(path.getElements());
  if ((attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
    HANDLE link = ::CreateFile(
      path.getElements(),
      FILE_ALL_ACCESS,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,
      0
    );
    if (link == INVALID_HANDLE_VALUE) {
      throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
    }
    uint8 buffer[REPARSE_GUID_DATA_BUFFER_HEADER_SIZE + 256]; // TAG: doc missing for min. size
    REPARSE_GUID_DATA_BUFFER* reparseHeader = (REPARSE_GUID_DATA_BUFFER*)buffer;
    DWORD bytesWritten;
    if (::DeviceIoControl(
          link,
          FSCTL_GET_REPARSE_POINT,
          0,
          0, // input
          reparseHeader,
          sizeof(buffer), // output
          &bytesWritten,
          0
        ) == 0) {
      if (::GetLastError() != ERROR_MORE_DATA) {
        ::CloseHandle(link);
        throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
      }
    }
    if (reparseHeader->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
      reparseHeader->ReparseDataLength = 0;
      reparseHeader->Reserved = 0;
      if (::DeviceIoControl(
            link,
            FSCTL_DELETE_REPARSE_POINT,
            reparseHeader,
            REPARSE_GUID_DATA_BUFFER_HEADER_SIZE, // input
            0,
            0, // output
            &bytesWritten,
            0
          ) == 0) {
        fout << "12345: " << ::GetLastError() << ENDL;
        ::CloseHandle(link);
        throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
      }
      ::CloseHandle(link);
      if (!::RemoveDirectory(path.getElements())) {
        throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
      }
      // } else if (reparseHeader->ReparseTag == 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK) {
      // TAG: need support for symbolic link to folder
    } else {
      ::CloseHandle(link);
      throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
    }
  } else {
    if (!::RemoveDirectory(path.getElements())) {
      throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
    }
  }
#else // unix
  if (rmdir(path.getElements())) {
    throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
  }
#endif // flavor
}

void FileSystem::makeFolder(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::CreateDirectory(path.getElements(), 0)) { // use default security descriptor
    throw FileSystemException("Unable to make folder", Type::getType<FileSystem>());
  }
#else // unix
  if (mkdir(path.getElements(), 0)) {
    throw FileSystemException("Unable to make folder", Type::getType<FileSystem>());
  }
#endif // flavor
}

bool FileSystem::supportsLinks() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    OSVERSIONINFO versionInfo;
    versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
    ::GetVersionEx(&versionInfo); // never fails // TAG: need static variable for windows version
    cachedSupportsLinks = ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5)) ? 1 : 0;
  }
  return cachedSupportsLinks == 1;
#else // unix
  return true;
#endif
}

// TAG: also for platform in general
// String getFileSystem(const String& path) throw() {...}
// return infomation about the file system capabilities
// unsigned int FileSystem::getCapabilities(const String& path) throw() {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   DWORD flags;
//   BOOL result = ::GetVolumeInformation("C:\\", // root directory
//                                        0, // volume name buffer
//                                        0, // length of name buffer
//                                        0, // volume serial number
//                                        0, // maximum file name length
//                                        &flags, // file system options
//                                        0, // file system name buffer
//                                        0 // length of file system name buffer
//   );
//   unsigned int flags = 0;
//   return FILE_SUPPORTS_REPARSE_POINTS;
// #else // unix
//   flags |= MOUNT_POINT;
//   flags |= HARD_LINK;
//   flags |= LINK;
// #endif // flavor
//   return flags;
// }

bool FileSystem::isLink(const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    supportsLinks();
  }
  assert(cachedSupportsLinks == 1, NotSupported(Type::getType<FileSystem>()));

  const char* elements = path.getElements();
  if ((elements[0] == '.') && ((elements[1] == 0) || ((elements[1] == '.') && (elements[2] == 0)))) { // "." or ".."
    return false;
  }
  
  DWORD attributes = ::GetFileAttributes(path.getElements());
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return false;
  }
  if ((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
    HANDLE link = ::CreateFile(path.getElements(),
                               0,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               0,
                               OPEN_EXISTING,
                               FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,
                               //(attributes & FILE_ATTRIBUTE_DIRECTORY) ? (FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT) : (FILE_FLAG_OPEN_REPARSE_POINT),
                               0);
    // TAG: error code 4390 - ERROR_NOT_A_REPARSE_POINT?
    if (link == INVALID_HANDLE_VALUE) {
      switch (::GetLastError()) {
      case ERROR_ACCESS_DENIED:
      case ERROR_SHARING_VIOLATION: // possible with page file
      case ERROR_LOCK_VIOLATION: // TAG: is this ok
        return false;
      default:
        throw FileSystemException(Type::getType<FileSystem>());
      }
    }
    
    // TAG: test if partial support works - ERROR_MORE_DATA
    uint8 buffer[17000]; // TAG: fixme
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)buffer;
    DWORD bytesWritten;
    bool error = ::DeviceIoControl(
      link,
      FSCTL_GET_REPARSE_POINT,
      0,
      0, // input
      reparseHeader,
      sizeof(buffer), // output
      &bytesWritten,
      0
    ) == 0;
    bool isLink = false;
    if (!error) {
      if (IsReparseTagMicrosoft(reparseHeader->ReparseTag)) {
        switch (reparseHeader->ReparseTag) {
        case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
        case IO_REPARSE_TAG_MOUNT_POINT:
          isLink = true;
        }
      }
    } else {
      if (::GetLastError() == ERROR_MORE_DATA) {
        ASSERT(bytesWritten >= REPARSE_DATA_BUFFER_HEADER_SIZE);
        if (IsReparseTagMicrosoft(reparseHeader->ReparseTag)) {
          switch (reparseHeader->ReparseTag) {
          case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
          case IO_REPARSE_TAG_MOUNT_POINT:
            isLink = true;
          }
        }
        error = false;
      }
    }
    ::CloseHandle(link);
    return isLink;
  } else if ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
    // check if shell symbolic link
    static const unsigned char GUID[16] = {
      0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46
    };
    
    struct ShortcutHeader {
      LittleEndian<uint32> identifier; // 'L'
      unsigned char guid[16];
      LittleEndian<uint32> flags;
      LittleEndian<uint32> attributes;
      LittleEndian<uint64> time1;
      LittleEndian<uint64> time2;
      LittleEndian<uint64> time3;
      LittleEndian<uint32> length;
      LittleEndian<uint32> icon;
      LittleEndian<uint32> windowMode;
      LittleEndian<uint32> hotKey;
      LittleEndian<uint32> reserved[2];
    } _DK_SDU_MIP__BASE__PACKED;
    
    enum Flags {
      SHELL_ITEM_ID_PRESENT = 1 << 0,
      POINTS_TO_FILE_OR_FOLDER = 1 << 1,
      HAS_DESCRIPTION = 1 << 2,
      HAS_RELATIVE_PATH = 1 << 3,
      HAS_WORKING_FOLDER = 1 << 4,
      HAS_COMMAND_LINE_ARGUMENTS = 1 << 5,
      HAS_ICON = 1 << 6
    };
    HANDLE link = ::CreateFile(
      path.getElements(),
      GENERIC_READ,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      0,
      OPEN_EXISTING,
      0,
      0
    );
    if (link == INVALID_HANDLE_VALUE) {
      return false; // we may not have read permission
    }
    ShortcutHeader header;
    DWORD result;
    bool error = ::ReadFile(link, &header, sizeof(header), &result, 0) == 0;
    ::CloseHandle(link);
    
    return !error &&
      (header.identifier == static_cast<unsigned int>('L')) &&
      (result == sizeof(header)) &&
      ((header.flags & POINTS_TO_FILE_OR_FOLDER|HAS_RELATIVE_PATH) == POINTS_TO_FILE_OR_FOLDER|HAS_RELATIVE_PATH) &&
      (compare(header.guid, GUID, sizeof(GUID)) == 0);
  } else {
    return false;
  }
#else
  struct stat status;
  int result = ::lstat(path.getElements(), &status);
  if (result == 0) {
    return S_ISLNK(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      throw FileSystemException(Type::getType<FileSystem>());
    }
  }
#endif
}

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
class FileSystemImpl {
public:

  // TAG: the link target may not exist
  
  static String getLinkTarget(const String& path) throw(FileSystemException) {
//     if (cachedSupportsLinks == -1) {
//       supportsLinks();
//     }
//     assert(
//       cachedSupportsLinks == 1,
//       NotSupported("Symbolic link", Type::getType<FileSystem>())
//     );
    
    HANDLE link = ::CreateFile(
      path.getElements(),
      0,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      0,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
      0
    );
    if (link == INVALID_HANDLE_VALUE) {
      switch (::GetLastError()) {
      case ERROR_ACCESS_DENIED:
      case ERROR_SHARING_VIOLATION: // possible with page file
      case ERROR_LOCK_VIOLATION:
        throw FileSystemException("Not a link", Type::getType<FileSystem>());
      }
    }
    while (link != INVALID_HANDLE_VALUE) {
      // TAG: fix buffer size
      uint8 buffer[17000]; // need alternative - first attempt to get length first failed
      REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)buffer;
      DWORD bytesWritten;
      bool error = ::DeviceIoControl(
        link,
        FSCTL_GET_REPARSE_POINT,
        0,
        0,
        reparseHeader,
        sizeof(buffer),
        &bytesWritten,
        0
      ) == 0;
      if (error) {
        // bool reparse = ::GetLastError() != 4390; // ERROR_NOT_A_REPARSE_POINT
        ::CloseHandle(link);
        // if (reparse) { // no need to check for shell link
        //   throw FileSystemException("Not a link", Type::getType<FileSystem>());
        // }
        break;
      }
      
      switch (reparseHeader->ReparseTag) {
      case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
        {
          ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0) &&
                 (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/sizeof(wchar) > 4));
          wchar* substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer +
            reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset + 4;
          // skip prefix "\??\"
          unsigned int substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2 - 4;
          substPath[substLength] = 0; // add terminator
          return WideString::getMultibyteString(substPath /*, substLength*/);
        }
      case IO_REPARSE_TAG_MOUNT_POINT:
        {
          ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0) &&
                 (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/sizeof(wchar) > 4));
          wchar* substPath = reparseHeader->MountPointReparseBuffer.PathBuffer +
            reparseHeader->MountPointReparseBuffer.SubstituteNameOffset + 4;
          // skip prefix "\??\"
          unsigned int substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 - 4;
          substPath[substLength] = 0; // add terminator
          return WideString::getMultibyteString(substPath /*, substLength*/);
        }
      default:
        error = true;
      }
      if (error) {
        break;
      }
    }
  }
  
  static inline bool enablePrivileges() throw() {
    HANDLE token;
    char buffer[sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES)]; // make room for backup and restore privileges
    TOKEN_PRIVILEGES& privileges = *((TOKEN_PRIVILEGES*)buffer);
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
      return false;
    }
    
    privileges.PrivilegeCount = 2;
    
    // enable SeBackupPrivilege
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // ask for privilege
    if (!::LookupPrivilegeValue(0, SE_BACKUP_NAME, &privileges.Privileges[0].Luid)) {
      ::CloseHandle(token);
      return false;
    }
    
    // enable SeRestorePrivilege
    privileges.Privileges[1].Attributes = SE_PRIVILEGE_ENABLED; // ask for privilege
    if (!::LookupPrivilegeValue(0, SE_RESTORE_NAME, &privileges.Privileges[1].Luid)) {
      ::CloseHandle(token);
      return false;
    }
    
    if (!::AdjustTokenPrivileges(token, FALSE, &privileges, 0, 0, 0)) {
      ::CloseHandle(token);
      return false;
    }
    ::CloseHandle(token);
    return true;
  }
  
  static inline bool makeHardLink(const String& target, const String& path) throw() {
    static bool elevatedPrivileges = false;
    if (!elevatedPrivileges) {
      enablePrivileges();
      elevatedPrivileges = true;
    }
    
    HANDLE handle = ::CreateFile(path.getElements(), 0, 0, 0, OPEN_EXISTING, 0, 0);
    if (handle != INVALID_HANDLE_VALUE) { // make path does not exist
      ::CloseHandle(handle);
      return false;
    }
    
    handle = ::CreateFile(
      target.getElements(),
      GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_POSIX_SEMANTICS,
      0
    );    
    if (handle == INVALID_HANDLE_VALUE) {
      return false;
    }
    
    char fullPath[MAX_PATH];
    char* filename;
    ::GetFullPathName(path.getElements(), MAX_PATH, fullPath, &filename);
    WideString wideFullPath(fullPath);
    
    WIN32_STREAM_ID stream; // setup hard link
    stream.dwStreamId = BACKUP_LINK;
    stream.dwStreamAttributes = 0;
    stream.dwStreamNameSize = 0;
    stream.Size.QuadPart = (wideFullPath.getLength() + 1) * sizeof(wchar); // including terminator
    
    bool success = false;
    DWORD bytesWritten;
    void* context = 0;
    if (::BackupWrite(
          handle,
          (BYTE*)&stream,
          (char*)&stream.cStreamName-(char*)&stream,
          &bytesWritten,
          FALSE,
          FALSE,
          &context
        ) != 0) {
      if (bytesWritten == static_cast<MemorySize>((char*)&stream.cStreamName-(char*)&stream)) {
        if (::BackupWrite(
              handle,
              (BYTE*)wideFullPath.getElements(),
              stream.Size.LowPart,
              &bytesWritten,
              FALSE,
              FALSE,
              &context) != 0) {
          if (bytesWritten == stream.Size.LowPart) {
            success = true;
          }
        }
      }
      
      // release the backup context
      ::BackupWrite(handle, (BYTE*)fullPath, 0, &bytesWritten, TRUE, FALSE, &context); // abort
    }
    ::CloseHandle(handle);
    return success;
  }
  
};
#endif // flavor

void FileSystem::makeHardLink(const String& target, const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__W2K)
  assert(::CreateHardLink(path.getElements(), target.getElements(), 0) != 0,
         FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#else
  typedef BOOL (*PCreateHardLink)(LPCSTR, LPCSTR, LPSECURITY_ATTRIBUTES);
  static PCreateHardLink CreateHardLink = 0;
  static bool cachedCreateHardLink = false;
  if (!cachedCreateHardLink) {
    CreateHardLink = (PCreateHardLink)::GetProcAddress(::GetModuleHandle("kernel32.dll"), "CreateHardLinkA");
    cachedCreateHardLink = true;
  }
  bool error;
  if (CreateHardLink) {
    error = CreateHardLink(path.getElements(), target.getElements(), 0) == 0;
  } else {
    error = !FileSystemImpl::makeHardLink(target, path);
  }
  assert(!error, FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#endif // w2k or later
#else // unix
  assert(::link(target.getElements(), path.getElements()) == 0, FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#endif // flavor
}

void FileSystem::makeLink(const String& target, const String& path)
  throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    supportsLinks();
  }
  assert(cachedSupportsLinks == 1, NotSupported(Type::getType<FileSystem>()));
  
  char* fileNameComponent;
  
	char fullTargetPath[MAX_PATH];
  assert(
    ::GetFullPathName(
      target.getElements(),
      MAX_PATH,
      fullTargetPath,
      &fileNameComponent
    ),
    FileSystemException(Type::getType<FileSystem>())
  );
  
	// make the native target name
	String nativePath(MESSAGE("\\??\\"));
  nativePath += NativeString(fullTargetPath);
  
  DWORD attributes = ::GetFileAttributes(target.getElements());
  assert(
    attributes != INVALID_FILE_ATTRIBUTES,
    FileSystemException(Type::getType<FileSystem>())
  );
  bool isDirectory = attributes & FILE_ATTRIBUTE_DIRECTORY;
  bool directoryCreated = false;
  
  HANDLE link;
  if (isDirectory) {
    if (!nativePath.endsWith(MESSAGE(":\\"))) {
      nativePath -= MESSAGE("\\");
    }
    assert(
      nativePath.getLength() <= MAX_PATH,
      FileSystemException(Type::getType<FileSystem>())
    ); // watch out for buffer overflow
    // we do not care whether or not it already exists
    directoryCreated = ::CreateDirectory(path.getElements(), 0) != 0;
    assert(directoryCreated, FileSystemException(Type::getType<FileSystem>()));
    link = ::CreateFile(
      path.getElements(),
      GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS,
      0
    );
  } else {
    assert(
      !nativePath.endsWith(MESSAGE("\\")),
      FileSystemException(Type::getType<FileSystem>())
    );
    assert(
      nativePath.getLength() <= MAX_PATH,
      FileSystemException(Type::getType<FileSystem>())
    ); // watch out for buffer overflow
    link = ::CreateFile(
      path.getElements(),
      GENERIC_WRITE,
      0,
      0,
      CREATE_NEW,
      FILE_FLAG_OPEN_REPARSE_POINT,
      0
    );
  }
  assert(link != INVALID_HANDLE_VALUE, FileSystemException(Type::getType<FileSystem>()));
  
  uint8 reparseBuffer[sizeof(REPARSE_DATA_BUFFER) + MAX_PATH * sizeof(WCHAR)];
  REPARSE_DATA_BUFFER* reparseInfo = (REPARSE_DATA_BUFFER*)reparseBuffer;
  clear(*reparseInfo);
  if (isDirectory) {
    reparseInfo->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
  } else {
    reparseInfo->ReparseTag = 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK;
  }
  
  reparseInfo->ReparseDataLength = nativePath.getLength() * sizeof(WCHAR) + 12;
  reparseInfo->MountPointReparseBuffer.SubstituteNameLength = nativePath.getLength() * sizeof(WCHAR);
  reparseInfo->MountPointReparseBuffer.PrintNameOffset =
    reparseInfo->MountPointReparseBuffer.SubstituteNameLength + sizeof(WCHAR);
  
  WideString::UTF8ToUCS2(
    Cast::pointer<ucs2*>(reparseInfo->MountPointReparseBuffer.PathBuffer),
    Cast::pointer<const uint8*>(nativePath.getElements()),
    nativePath.getLength(),
    0
  );
  
	// set the link
  DWORD returnedLength;
  bool error = ::DeviceIoControl(
    link,
    FSCTL_SET_REPARSE_POINT,
    reparseInfo,
    reparseInfo->ReparseDataLength + REPARSE_DATA_BUFFER_HEADER_SIZE,
    0,
    0,
    &returnedLength,
    0) == 0;
  ::CloseHandle(link);
  if (error) {
    if (isDirectory) {
      if (directoryCreated) {
        ::RemoveDirectory(path.getElements()); // clean up
      }
    } else {
      ::DeleteFile(path.getElements()); // clean up // TAG: can we use FILE_FLAG_DELETE_ON_CLOSE
    }
  }
  assert(!error, FileSystemException("Unable to make link", Type::getType<FileSystem>()));
#else // unix
  assert(
    !::symlink(target.getElements(), path.getElements()),
    FileSystemException("Unable to make link", Type::getType<FileSystem>())
  );
#endif // flavor
}

// TAG: need option disallow transparent link/strict transparency
String FileSystem::getLink(const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    supportsLinks();
  }
  assert(cachedSupportsLinks == 1, NotSupported("Symbolic link", Type::getType<FileSystem>()));
  
  HANDLE link = ::CreateFile(
    path.getElements(),
    0,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    0,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
    0
  );
  if (link == INVALID_HANDLE_VALUE) {
    switch (::GetLastError()) {
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION: // possible with page file
    case ERROR_LOCK_VIOLATION:
      throw FileSystemException("Not a link", Type::getType<FileSystem>());
    }
  }
  while (link != INVALID_HANDLE_VALUE) {
    // TAG: fix buffer size
    uint8 buffer[17000]; // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)buffer;
    DWORD bytesWritten;
    bool error = ::DeviceIoControl(
      link,
      FSCTL_GET_REPARSE_POINT,
      0,
      0,
      reparseHeader,
      sizeof(buffer),
      &bytesWritten,
      0
    ) == 0;
    if (error) {
      // bool reparse = ::GetLastError() != 4390; // ERROR_NOT_A_REPARSE_POINT
      ::CloseHandle(link);
      // if (reparse) { // no need to check for shell link
      //   throw FileSystemException("Not a link", Type::getType<FileSystem>());
      // }
      break;
    }
    
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      {
        ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
          (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
        wchar* substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer +
          reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset + 4;
        // skip prefix "\??\"
        unsigned int substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2 - 4;
        substPath[substLength] = 0; // add terminator
        return WideString::getMultibyteString(substPath /*, substLength*/);
      }
    case IO_REPARSE_TAG_MOUNT_POINT:
      {
        ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
               (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
        wchar* substPath = reparseHeader->MountPointReparseBuffer.PathBuffer +
          reparseHeader->MountPointReparseBuffer.SubstituteNameOffset + 4;
        // skip prefix "\??\"
        unsigned int substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 - 4;
        substPath[substLength] = 0; // add terminator
        return WideString::getMultibyteString(substPath /*, substLength*/);
      }
    default:
      error = true;
    }
    
    if (error) {
      break;
    }
  }
  
  // TAG: need variable to disable/enable this symbolic link
  // TAG: need support for link to link for open file, cycle, ... (problem: reparse->shell link->reparse->shell link->shell link->...)
  // check if shell symbolic link
  static const unsigned char GUID[16] = {0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
  
  struct ShortcutHeader {
    LittleEndian<uint32> identifier; // 'L'
    unsigned char guid[16];
    LittleEndian<uint32> flags;
    LittleEndian<uint32> attributes;
    LittleEndian<uint64> time1;
    LittleEndian<uint64> time2;
    LittleEndian<uint64> time3;
    LittleEndian<uint32> length;
    LittleEndian<uint32> icon;
    LittleEndian<uint32> windowMode;
    LittleEndian<uint32> hotKey;
    LittleEndian<uint32> reserved[2];
  } _DK_SDU_MIP__BASE__PACKED;
  
  struct FileLocationInfo {
    LittleEndian<uint32> size; // size of structure and data
    LittleEndian<uint32> offset; // 0x1c
    LittleEndian<uint32> flags;
    LittleEndian<uint32> volumeOffset;
    LittleEndian<uint32> pathOffset;
    LittleEndian<uint32> networkVolumeOffset;
    LittleEndian<uint32> remainingPathOffset;
  } _DK_SDU_MIP__BASE__PACKED;
  
  struct LocalVolume {
    LittleEndian<uint32> size; // size of structure
    LittleEndian<uint32> type;
    LittleEndian<uint32> serialNumber;
    LittleEndian<uint32> labelOffset;
    char label;
  } _DK_SDU_MIP__BASE__PACKED;
  
  struct NetworkVolume {
    LittleEndian<uint32> size; // size of structure
    LittleEndian<uint32> reserved0;
    LittleEndian<uint32> shareOffset;
    LittleEndian<uint32> reserved1;
    LittleEndian<uint32> reserved2;
    char share;
  } _DK_SDU_MIP__BASE__PACKED;
  
  enum Flags {
    SHELL_ITEM_ID_PRESENT = 1 << 0,
    POINTS_TO_FILE_OR_FOLDER = 1 << 1,
    HAS_DESCRIPTION = 1 << 2,
    HAS_RELATIVE_PATH = 1 << 3,
    HAS_WORKING_FOLDER = 1 << 4,
    HAS_COMMAND_LINE_ARGUMENTS = 1 << 5,
    HAS_ICON = 1 << 6
  };
  
  while (true) {
    link = ::CreateFile(path.getElements(),
                        GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                        0, OPEN_EXISTING, 0, 0);
    if (link == INVALID_HANDLE_VALUE) { // fails if directory
      // we may not have read permission or file could be locked
      // so file could be link after all but we may never know for sure
      break;
    }
    
    char buffer[4096 * 2]; // maximum link size
    const ShortcutHeader* header = (const ShortcutHeader*)buffer;
    
    DWORD linkLength;
    bool error = ::ReadFile(link, buffer, sizeof(buffer), &linkLength, 0) == 0;
    ::CloseHandle(link);
    
    if (!(!error &&
          (linkLength > sizeof(ShortcutHeader)) &&
          (header->identifier == static_cast<unsigned int>('L')) &&
          (compare(header->guid, GUID, sizeof(GUID)) == 0))) {
      break;
    }
    unsigned long long offset = sizeof(ShortcutHeader);
    if (header->flags & SHELL_ITEM_ID_PRESENT) {
      if (offset + sizeof(LittleEndian<uint16>) > linkLength) {
        break;
      }
      const LittleEndian<uint16>* shellItemSize =
        (const LittleEndian<uint16>*)(buffer + offset);
      offset += *shellItemSize + sizeof(LittleEndian<uint16>);
    }
    
    if (header->flags & POINTS_TO_FILE_OR_FOLDER) {
      if (offset + sizeof(FileLocationInfo) > linkLength) {
        break;
      }
      const FileLocationInfo* fileLocationInfo =
        (const FileLocationInfo*)(buffer + offset);
//       ferr << "file location: " << fileLocationInfo->size << EOL
//            << "file location: " << fileLocationInfo->offset << EOL
//            << "file location: " << fileLocationInfo->flags << EOL
//            << "file location: " << fileLocationInfo->volumeOffset << EOL
//            << "file location: " << fileLocationInfo->pathOffset << EOL
//            << "file location: " << fileLocationInfo->networkVolumeOffset << EOL
//            << "file location: " << fileLocationInfo->remainingPathOffset << EOL
//            << ENDL;
      
      if (!((fileLocationInfo->size > sizeof(FileLocationInfo)) &&
            (fileLocationInfo->offset >= sizeof(FileLocationInfo)))) {
        break;
      }

      // TAG: prefer Unicode if available (but I do not know the format)
      if (fileLocationInfo->flags & 1) {
        if (offset + fileLocationInfo->volumeOffset + sizeof(LocalVolume) > linkLength) {
          break;
        }
        const LocalVolume* volume = (const LocalVolume*)(buffer + offset + fileLocationInfo->volumeOffset);
        if (offset + fileLocationInfo->volumeOffset + volume->labelOffset >= linkLength) {
          break;
        }
//         ferr << "local volume: " << volume->size << EOL
//              << "local volume: " << volume->type << EOL
//              << "local volume: " << volume->serialNumber << EOL
//              << "local volume: " << volume->labelOffset << EOL
//              << "local volume: " << buffer[offset + volume->labelOffset] << EOL
//              << "base path: " << String(buffer + offset + fileLocationInfo->pathOffset) << EOL
//              << "remaining path: " << String(buffer + offset + fileLocationInfo->remainingPathOffset) << EOL
//              << ENDL;

        if (!((offset + fileLocationInfo->pathOffset < linkLength) &&
              (offset + fileLocationInfo->remainingPathOffset < linkLength))) {
          break;
        }

        // TAG: check if path looks valid
        String path(buffer + offset + fileLocationInfo->pathOffset, linkLength - offset - fileLocationInfo->pathOffset);
        if (buffer[offset + fileLocationInfo->remainingPathOffset] != 0) { // if not empty
          path += MESSAGE("\\") + String(buffer + offset + fileLocationInfo->remainingPathOffset,
                                         linkLength - offset - fileLocationInfo->remainingPathOffset);
        }
        return path;
      } else if (fileLocationInfo->flags & 2) {
        if (offset + fileLocationInfo->networkVolumeOffset + sizeof(NetworkVolume) > linkLength) {
          break;
        }
        const NetworkVolume* volume = (const NetworkVolume*)(buffer + offset + fileLocationInfo->networkVolumeOffset);
        if (!((offset + fileLocationInfo->networkVolumeOffset + volume->shareOffset < linkLength) &&
              (offset + fileLocationInfo->remainingPathOffset < linkLength))) {
          break;
        }
        
//         ferr << "network volume: " << volume->size << EOL
//              << "network volume: " << volume->shareOffset << EOL
//              << "network volume: " << String(buffer + offset + fileLocationInfo->networkVolumeOffset + volume->shareOffset) << EOL
//              << ENDL;
        
        // TAG: check if path looks valid (e.g. \\xxxx\xxxx...????)
        String path(buffer + offset + fileLocationInfo->networkVolumeOffset + volume->shareOffset,
                    linkLength - offset + fileLocationInfo->networkVolumeOffset - volume->shareOffset);
        if (buffer[offset + fileLocationInfo->remainingPathOffset] != 0) { // if not empty
          path += MESSAGE("\\") + String(buffer + offset + fileLocationInfo->remainingPathOffset,
                                         linkLength - offset - fileLocationInfo->remainingPathOffset);
        }
        return path;
      }
    } else if (header->flags & HAS_RELATIVE_PATH) {
      if (header->flags & HAS_DESCRIPTION) { // skip description if present
        if (offset + sizeof(LittleEndian<uint16>) > linkLength) {
          break;
        }
        const LittleEndian<uint16>* length = (const LittleEndian<uint16>*)(buffer + offset);
        offset += *length + sizeof(LittleEndian<uint16>); // skip description
        if (offset > linkLength) {
          break;
        }
      }

      // read relative path
      if (offset + sizeof(LittleEndian<uint16>) > linkLength) {
        break;
      }
      const LittleEndian<uint16>* length = (const LittleEndian<uint16>*)(buffer + offset);
      if (offset + *length + sizeof(LittleEndian<uint16>) > linkLength) { // make sure path fits in file (with terminator)
        break;
      }
      
      // we have to do some basic validation 'cause anybody can create a shell link
      // TAG: check if valid chars
      // TAG: if path looks valid
//       if (linkLength + 1 > sizeof(buffer)) {
//         break; // no room for terminator
//       }
//       buffer[offset + sizeof(LittleEndian<uint16>) + *length] = 0; // add terminator
//       char fullPath[MAX_PATH];
//       char* fullFilename;
//       bool error = ::GetFullPathName(buffer + offset + sizeof(LittleEndian<uint16>),
//                                      sizeof(fullPath),
//                                      fullPath,
//                                      &fullFilename) == 0;
//       if (error) {
//         break;
//       }
      return String(buffer + offset + sizeof(LittleEndian<uint16>), *length); // return relative path
    }

    break; // exit while loop
  }
  throw FileSystemException("Not a link", Type::getType<FileSystem>());
#else // unix
  char buffer[PATH_MAX + 1];
  ssize_t length = ::readlink(path.getElements(), buffer, sizeof(buffer));
  assert(length >= 0, FileSystemException(Type::getType<FileSystem>()));
  return String(buffer, length);
#endif
}

String FileSystem::getTempFolder(TemporaryFolder folder) throw() {
  // TAG: need to expand variables (win32) (e.g. set TMP=%HOME%\temp)
  switch (folder) {
  case FileSystem::USER_SPECIFIED:
    {
      const Map<String, String> environment = Application::getApplication()->getEnvironment();
      if (environment.isKey(MESSAGE("TMP"))) {
        return environment[MESSAGE("TMP")];
      } else if (environment.isKey(MESSAGE("TEMP"))) {
        return environment[MESSAGE("TEMP")];
      }
    }
  case FileSystem::MACHINE_NONPERSISTENT:
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    return MESSAGE("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return MESSAGE("/tmp");
#endif // flavor
  case FileSystem::MACHINE_PERSISTENT:
  default:
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    return MESSAGE("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return MESSAGE("/var/tmp");
#endif // flavor
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
    stream << HEX << ZEROPAD << Date::getNow().getValue();
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
    stream << ".tmp";
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

unsigned long FileSystem::getVariable(const String& path, Variable variable) throw(NotSupported) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(Type::getType<FileSystem>());
#else // unix
#  if (!(defined(_PC_FILESIZEBITS)))
#    define _PC_FILESIZEBITS -1
#  endif
#  if (!(defined(_PC_LINK_MAX)))
#    define _PC_LINK_MAX -1
#  endif
#  if (!(defined(_PC_NAME_MAX)))
#    define _PC_NAME_MAX -1
#  endif
#  if (!(defined(_PC_PATH_MAX)))
#    define _PC_PATH_MAX -1
#  endif
#  if (!(defined(_PC_PIPE_BUF)))
#    define _PC_PIPE_BUF -1
#  endif
#  if (!(defined(_PC_SYMLINK_MAX)))
#    define _PC_SYMLINK_MAX -1
#  endif
  
  static int FILE_SYSTEM_VARIABLES[MAX_SIZE_OF_SYMLINK + 1] = {
    _PC_FILESIZEBITS, // MIN_FILE_SIZE_BITS
    _PC_LINK_MAX, // MAX_NUM_OF_LINKS
    _PC_NAME_MAX, // MAX_LEN_OF_NAME
    _PC_PATH_MAX, // MAX_LEN_OF_PATH
    _PC_PIPE_BUF, // MAX_SIZE_OF_PIPE_BUFFER
    _PC_SYMLINK_MAX // MAX_SIZE_OF_SYMLINK
  };
  
  int fileSystemVariable = FILE_SYSTEM_VARIABLES[variable];
  if (fileSystemVariable != -1) {
    errno = 0;
    long result = ::pathconf(path.getElements(), fileSystemVariable);
    if (!((result == -1) && (errno != 0))) {
      return result;
    }
  }

  // TAG: add POSIX values here?
  throw NotSupported(Type::getType<FileSystem>());
#endif // flavor
}

/*
  TAG: other folders:

  System configuration: "C:\windows\system32\?" or "/etc"
  System libraries: "C:\windows\system32" or "/lib"
  All users: GetAllUsersProfileDirectory(...) or ?
  Personal persistent data: "GetUserProfileDirectory()\Application Data" or %HOME%/etc
  ?: ? or "/net"
  ?: ? or "/opt"
  ?: ? or "/proc"
  Homes: GetProfilesDirectory(...) or "/home"
  ?: ? or "/var/tmp"

  Files:
    "/dev/null"
    "/dev/zero"
    "/dev/tty"
*/

String FileSystem::getFolder(Folder folder) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  switch (folder) {
  case FileSystem::ROOT:
    {
      char buffer[3]; // large enough for "C:\0"
      ::GetWindowsDirectory(buffer, sizeof(buffer));
      return String(buffer);
    }
  case FileSystem::DEVICES:
    return MESSAGE("\\\\.");
  case FileSystem::TEMP:
  default:
    {
      char buffer[MAX_PATH + 1];
      ::GetWindowsDirectory(buffer, sizeof(buffer));
      return String(buffer) + MESSAGE("\\temp");
    }
  }
#else // unix
  static const Literal FOLDERS[] = {
    MESSAGE("/"),
    MESSAGE("/dev"),
    MESSAGE("/tmp")
  };
  return String(FOLDERS[folder]);
#endif // flavor
}

FileSystem::Quota FileSystem::getQuota(
  const String& path, Trustee trustee) throw(FileSystemException) {
  Quota result;
#if ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65))
  int id;
  switch (trustee.getType()) {
  case Trustee::USER:
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  case Trustee::GROUP:
#endif
    id = trustee.getIntegralId();
    break;
  default:
    throw FileSystemException(Type::getType<FileSystem>());
  }
  
  struct dqblk temp;
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
  // TAG: casting away const (temporary fix)
  int res = ::quotactl(
    Q_GETQUOTA,
    (char*)path.getElements(),
    id,
    Cast::pointer<caddr_t>(&temp)
  );
#else
  int res = ::quotactl(
    Q_GETQUOTA,
    path.getElements(),
    id,
    Cast::pointer<caddr_t>(&temp)
  );
#endif
  if (res == -1) {
    if (errno == EINVAL) {
      result.hardLimit = 0;
      result.softLimit = 0;
      result.currentUsage = 0;
      return result;
    }
    throw FileSystemException(Type::getType<FileSystem>());
  }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
  result.hardLimit = temp.dqb_bhardlimit;
  result.softLimit = temp.dqb_bsoftlimit;
  result.currentUsage = temp.dqb_curblocks;
#else
  result.hardLimit = temp.dqb_ihardlimit;
  result.softLimit = temp.dqb_isoftlimit;
  result.currentUsage = temp.dqb_curinodes;
#endif
  return result;
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  struct dqblk temp;
  struct quotctl operation;
  operation.op = Q_GETQUOTA;
  operation.addr = Cast::pointer<caddr_t>(&temp);
  
  switch (trustee.getType()) {
  case Trustee::USER:
    operation.uid = trustee.getIntegralId();
    break;
  default:
    throw FileSystemException(Type::getType<FileSystem>());
  }
  
  int fd = ::open(path.getElements(), O_RDONLY, 0);
  if (fd != -1) {
    if (::ioctl(fd, Q_QUOTACTL, &operation) == 0) {
      ::close(fd);
      result.hardLimit = temp.dqb_bhardlimit;
      result.softLimit = temp.dqb_bsoftlimit;
      result.currentUsage = temp.dqb_curblocks;
      return result;
    }
    ::close(fd);
  }
  throw bindError(FileSystemException(Type::getType<FileSystem>()), errno);
#else
  result.hardLimit = 0;
  result.softLimit = 0;
  result.currentUsage = 0;
  return result;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
