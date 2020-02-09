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
#include <base/filesystem/FileSystem.h>
#include <base/concurrency/AtomicCounter.h>
#include <base/concurrency/Thread.h>
#include <base/Type.h>
#include <base/collection/Map.h>
#include <base/Application.h>
#include <base/string/StringOutputStream.h>
#include <base/security/User.h>
#include <base/concurrency/Process.h>
#include <base/net/InetAddress.h>
#include <base/Random.h>
#include <base/ByteOrder.h>
#include <base/UnitTest.h>
#include <base/build.h>

// TAG: add method to normalize . and .. for path and multiple //
// TAG: need method to traverse links for win32 platforms

#if 0
// TAG: need support for following a specified number of links
// String FileSystem::getFinalPath(const String& path, unsigned int maximumLinks = 8);

// ../../x/y/../z/..

// current folder "."
// previous folder ".."
// duplicate separators are redundant: "//"

// return true if the path is valid (syntax)
bool FileSystem::isValid(const String& name) noexcept
{
  // level "." or ".." or ? separated with "/" or "\\"
  return false;
}

String FileSystem::urlToPath(Url url) noexcept
{
  return String();
}

// returns true if the path is a ...
bool FileSystem::isForwardPath(const String& path) noexcept
{
  return false;
}

bool FileSystem::optimizePath(const String& name) noexcept
{
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

bool FileSystem::isName(const String& name) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return (name.indexOf(SEPARATOR) < 0) && (name.indexOf('\\') < 0); // TAG: "C:" drive path
#else
  return name.indexOf(SEPARATOR) < 0;
#endif
}

// returns true if the specified path is a subpath of the root path
bool FileSystem::isSubPathOf(const String& root, const String& path) noexcept
{
  if (isAbsolutePath(path)) {
    return path.startsWith(root);
  } else {
    String subpath = toAbsolutePath(path);
    return subpath.startsWith(root);
  }
}
#endif





#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
// #  define _WIN32_WINNT _WIN32_WINNT_WINXP
#  define _WIN32_WINNT _WIN32_WINNT_VISTA
#  include <windows.h>
#  include <winioctl.h>

// #  include <ntifs.h>
typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT ReparseDataLength;
  USHORT Reserved;
  union {
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      ULONG  Flags;
      WCHAR  PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      WCHAR  PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      UCHAR DataBuffer[1];
    } GenericReparseBuffer;
  } DUMMYUNIONNAME;
} REPARSE_DATA_BUFFER, * PREPARSE_DATA_BUFFER;

#if !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
#  define REPARSE_DATA_BUFFER_HEADER_SIZE FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif

#if !defined(IO_REPARSE_TAG_SYMBOLIC_LINK)
#  define IO_REPARSE_TAG_SYMBOLIC_LINK 0
#endif

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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#  include <sys/quota.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#  include <sys/fs/ufs_quota.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
#  include <sys/quota.h>
#endif

#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace {

  const String currentFolder(".");
  const String parentFolder("..");

  /** Internal attribute specifying whether or not file system links are supported by the platform. */
  int cachedSupportsLinks = -1; // -1 not cached, 0 false, and 1 true
    /** Counter used for generating temporary file names. */
  PreferredAtomicCounter tempfileCounter;

  /** Specifies the folder level separator. */
  constexpr char SEPARATOR = '/';

  void normalizeSplit(Array<String>& paths)
  {
    // TAG: handle device item for win32
    paths.ensureCapacity(paths.getSize());
    MemorySize i = 0;
    while (i < paths.getSize()) {
      const String& p = paths[i];
      if (p == currentFolder) {
        paths.remove(i);
      } else if ((i > 0) && (p == parentFolder) && (paths[i] != parentFolder)) {
        paths.remove(i);
        paths.remove(i - 1);
        --i;
      } else {
        ++i;
      }
    }
  }
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
const unsigned int FileSystem::MAXIMUM_PATH_LENGTH = MAX_PATH;
#else // unix
const unsigned int FileSystem::MAXIMUM_PATH_LENGTH = PATH_MAX;
#endif // flavor

String FileSystem::getPath(const String& base, const String& relative) noexcept
{
  if (isAbsolutePath(relative)) {
    return relative;
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  String result(base.getLength() + sizeof("\\") + relative.getLength());
  result.append(base);
  while (result && isSeparator(result.getAt(result.getLength() - 1))) {
    result.removeAt(result.getLength() - 1);
  }
  result.append(SEPARATOR);
  result.append(relative); // cant start with separator
#else // unix
  String result(base.getLength() + sizeof("/") + relative.getLength());
  result.append(base);
  while (result && isSeparator(result.getAt(result.getLength() - 1))) {
    result.removeAt(result.getLength() - 1);
  }
  result.append(SEPARATOR);
  result.append(relative); // cant start with separator
#endif // flavor
  return result;
}

String FileSystem::getComponent(const String& path, Component component)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  int forward = path.lastIndexOf('/');
  int backward = path.lastIndexOf('\\');
  int separator = maximum(forward, backward);
#else // unix
  MemoryDiff separator = path.lastIndexOf(SEPARATOR);
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
      MemoryDiff dot = path.indexOf('.', separator + 1);
      return path.substring(separator + 1, dot);
    }
  case FileSystem::DOTEXTENSION:
    {
      MemoryDiff dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot);
      }
    }
    break;
  case FileSystem::EXTENSION:
    {
      MemoryDiff dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot + 1);
      }
    }
    break;
  }
  return String();
}

bool FileSystem::isAbsolutePath(const String& path) noexcept
{
  String::ReadIterator i = path.getBeginReadIterator();
  String::ReadIterator end = path.getEndReadIterator();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if ((end - i) >= 2) {
    if ((i[0] == '\\') && (i[1] == '\\')) { // UNC path
      ++i;
      ++i;
      return true;
    } else if (ASCIITraits::isAlpha(i[0]) && (i[1] == ':')) { // check for device like C:
      ++i;
      ++i;
    }
  }
#endif
  return (i < end) && isSeparator(*i);
}

bool FileSystem::isFolderPath(const String& path) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MemorySize length = path.getLength();
  if (length == 0) {
    return false;
  }
  char lastChar = path[length - 1];
  return isSeparator(lastChar);
#else // unix
  return path.endsWith("/");
#endif // flavor
}

String FileSystem::getParent(const String& path) 
{
  if (!path) {
    return String();
  }
  MemoryDiff index = findLastSeparator(path);
  if (index < 0) {
    return String();
  }
  return path.substring(0, index);
}

String FileSystem::toAbsolutePath(const String& base, const String& path)
{
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

String FileSystem::findFile(const Array<String>& searchPaths, const String& relative, unsigned int index)
{
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

String FileSystem::toUrl(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static const Literal PREFIX = "file:///";
#else // unix
  static const Literal PREFIX = "file://";
#endif // flavor
  if (isAbsolutePath(path)) {
    _throw FileSystemException(Type::getType<FileSystem>());
  }
  String result = PREFIX + path; // e.g. "file:///C:/"
  if (SEPARATOR != '/') {
    // replace all separators with "/" and replace duplicate separators (but leave prefix)
    String::Iterator i = result.getBeginIterator();
    i += PREFIX.getLength(); // skip prefix
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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

String FileSystem::getCurrentFolder()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD length = ::GetCurrentDirectory(0, NULL);
  if (length == 0) {
    _throw FileSystemException("Unable to get current folder.", Type::getType<FileSystem>());
  }
  PrimitiveStackArray<wchar> buffer(length);
  length = ::GetCurrentDirectory(length, buffer);
  if (length == 0) {
    _throw FileSystemException("Unable to get current folder.", Type::getType<FileSystem>());
  }
  return String(buffer, length);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
  return String();
#else // unix
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;
  BASSERT(buffer.getSize() > PATH_MAX);
  char* path = ::getcwd((char*)buffer.getElements(), buffer.getSize());
  if (!INLINE_ASSERT(path)) {
    _throw FileSystemException(
      "Unable to get current folder.",
      Type::getType<FileSystem>()
    );
  }
  return String(path);
#endif // flavor
}

void FileSystem::setCurrentFolder(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  if (!::SetCurrentDirectory(nativePath)) {
   _throw FileSystemException("Unable to set current folder.", Type::getType<FileSystem>());
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  if (::chdir(path.getElements())) {
    _throw FileSystemException("Unable to set current folder.", Type::getType<FileSystem>());
  }
#endif // flavor
}

unsigned int FileSystem::getType(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  unsigned int flags = 0;
  HANDLE file = ::CreateFile(
    nativePath, // file name
    FILE_READ_ATTRIBUTES | FILE_READ_EA /*| READ_CONTROL*/, // access mode
    FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
    0, // security descriptor
    OPEN_EXISTING, // how to create
    FILE_FLAG_BACKUP_SEMANTICS /*| FILE_FLAG_OPEN_REPARSE_POINT*/, // file attributes // follow link
    0
  ); // handle to template file
  if (file == INVALID_HANDLE_VALUE) {
//     switch (::GetLastError()) {
//     case ERROR_ACCESS_DENIED:
//     case ERROR_SHARING_VIOLATION: // possible with page file
//     case ERROR_LOCK_VIOLATION: // TAG: is this ok
//       _throw FileSystemException(Type::getType<FileSystem>());
//     }
    
    WIN32_FIND_DATA information;
    HANDLE find = ::FindFirstFileEx(
      nativePath,
      FindExInfoStandard,
      &information,
      FindExSearchNameMatch,
      0,
      0
    );
    if (find == INVALID_HANDLE_VALUE) {
      return 0;
    }
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
  bassert(!error, FileSystemException(Type::getType<FileSystem>()));
  
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
#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
#else
  struct stat status;
  int result = stat(path.getElements(), &status);
#endif
  if (result != 0) {
    return 0; // symbolic link might point to file that no longer exists
  }
  // bassert(result == 0, FileSystemException("Unable to query entry.", Type::getType<FileSystem>()));
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

#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS)
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
#endif
  return flags;
#endif // flavor
}

uint64 FileSystem::getSize(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  WIN32_FIND_DATA information;
  HANDLE handle = ::FindFirstFile(nativePath, &information);
  bassert(
    handle != INVALID_HANDLE_VALUE,
    FileSystemException("Unable to get size of file.", Type::getType<FileSystem>())
  );
  ::FindClose(handle);
  return static_cast<uint64>(information.nFileSizeHigh) | information.nFileSizeLow;
#else // unix
#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
#else
  struct stat status;
  int result = stat(path.getElements(), &status);
#endif
  bassert(result == 0, FileSystemException("Unable to get size of file.", Type::getType<FileSystem>()));
  return status.st_size;
#endif // flavor
}

bool FileSystem::entryExists(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  WIN32_FIND_DATA information;
  HANDLE handle = ::FindFirstFile(nativePath, &information);
  if (handle == INVALID_HANDLE_VALUE) {
    bassert(
      ::GetLastError() == ERROR_FILE_NOT_FOUND,
      FileSystemException("Unable to examine if entry exists.", Type::getType<FileSystem>())
    );
    return false;
  }
  ::FindClose(handle);
  return true;
#else // unix
#  if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
  if (result == 0) {
    return true;
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      _throw FileSystemException("Unable to examine if entry exists.", Type::getType<FileSystem>());
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
      _throw FileSystemException("Unable to examine if entry exists.", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

bool FileSystem::fileExists(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  DWORD result = ::GetFileAttributes(nativePath);
  if (result == INVALID_FILE_ATTRIBUTES) {
    // TAG: need support for no access
    return false;
  }
  // TAG: could check if (GetLastError() == ERROR_FILE_NOT_FOUND)
  return ((result & FILE_ATTRIBUTE_DIRECTORY) == 0) && ((result & FILE_ATTRIBUTE_DEVICE) == 0);
  // we ignore FILE_ATTRIBUTE_REPARSE_POINT
#else // unix
#  if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
  if (result == 0) {
    return S_ISREG(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      _throw FileSystemException("Unable to examine if file exists.", Type::getType<FileSystem>());
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
      _throw FileSystemException("Unable to examine if file exists.", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

bool FileSystem::folderExists(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  DWORD result = ::GetFileAttributes(nativePath);
  if (result == INVALID_FILE_ATTRIBUTES) {
    // TAG: need support for no access
    return false;
  }
  // TAG: could check if (GetLastError() == ERROR_FILE_NOT_FOUND)
  return ((result & FILE_ATTRIBUTE_DIRECTORY) != 0); // we ignore FILE_ATTRIBUTE_REPARSE_POINT
#else // unix
#  if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
  struct stat64 status;
  int result = stat64(path.getElements(), &status);
  if (result == 0) {
    return S_ISDIR(status.st_mode);
  } else {
    switch (errno) {
    case ENOENT:
      return false;
    default:
      _throw FileSystemException("Unable to examine if folder exists.", Type::getType<FileSystem>());
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
      _throw FileSystemException("Unable to examine if folder exists.", Type::getType<FileSystem>());
    }
  }
#  endif
#endif // flavor
}

void FileSystem::removeFile(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//  ::SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
  ToWCharString nativePath(path);
  if (!::DeleteFile(nativePath)) {
    _throw FileSystemException("Unable to remove file.", Type::getType<FileSystem>());
  }
#else // unix
  if (unlink(path.getElements())) {
    _throw FileSystemException("Unable to remove file.", Type::getType<FileSystem>());
  }
#endif // flavor
}

void FileSystem::removeFolder(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString _path(path);
  DWORD attributes = ::GetFileAttributes(_path);
  if ((attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
    HANDLE link = ::CreateFile(
      _path,
      FILE_ALL_ACCESS,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,
      0
    );
    if (link == INVALID_HANDLE_VALUE) {
      _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
    }
    uint8 buffer[REPARSE_GUID_DATA_BUFFER_HEADER_SIZE + 256]; // TAG: doc missing for min. size
    REPARSE_GUID_DATA_BUFFER* reparseHeader = (REPARSE_GUID_DATA_BUFFER*)buffer;
    DWORD bytesWritten = 0;
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
        _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
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
        ::CloseHandle(link);
        _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
      }
      ::CloseHandle(link);
      if (!::RemoveDirectory(_path)) {
        _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
      }
      // } else if (reparseHeader->ReparseTag == 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK) {
      // TAG: need support for symbolic link to folder
    } else {
      ::CloseHandle(link);
      _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
    }
  } else {
    if (!::RemoveDirectory(_path)) {
      _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
    }
  }
#else // unix
  if (rmdir(path.getElements())) {
    _throw FileSystemException("Unable to remove folder.", Type::getType<FileSystem>());
  }
#endif // flavor
}

void FileSystem::makeFolder(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  if (!::CreateDirectory(nativePath, NULL)) { // use default security descriptor
    _throw FileSystemException("Unable to make folder.", Type::getType<FileSystem>());
  }
#else // unix
  if (mkdir(path.getElements(), 0770)) { // TAG: need a way to control access
    _throw FileSystemException("Unable to make folder.", Type::getType<FileSystem>());
  }
#endif // flavor
}

MemoryDiff FileSystem::findSeparator(const String& path, MemorySize start)
{
  if (start >= path.getLength()) {
    return -1;
  }
  auto i = path.getBeginReadIterator() + start;
  const auto end = path.getEndReadIterator();
  while (i != end) {
    if (FileSystem::isSeparator(*i)) {
      return i - path.getBeginReadIterator();
    }
    ++i;
  }
  return -1;
}

MemoryDiff FileSystem::findLastSeparator(const String& path)
{
  auto begin = path.getBeginReadIterator();
  auto i = path.getEndReadIterator();
  while (i != begin) {
    --i;
    if (FileSystem::isSeparator(*i)) {
      return i - begin;
    }
  }
  return -1;
}

void FileSystem::makeFolderRecursive(const String& path)
{
  MemoryDiff i = 0;
  while (i >= 0) {
    i = findSeparator(path, i);
    String sub;
    if (i >= 0) {
      sub = path.substring(0, i);
      ++i; // skip separator
    } else {
      sub = path;
    }
    if (!sub) {
      continue;
    }
    if (!folderExists(sub)) {
      makeFolder(sub);
    }
  }
}

bool FileSystem::doesSupportLinks() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return true; // since Vista - but use dynamic linking to check

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
// String getFileSystem(const String& path) noexcept {...}
// return infomation about the file system capabilities
// unsigned int FileSystem::getCapabilities(const String& path) noexcept {
// #if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   DWORD flags = 0;
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

bool FileSystem::isLink(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);
  DWORD attributes = GetFileAttributesW(nativePath);
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return false;
  }
  return attributes & FILE_ATTRIBUTE_REPARSE_POINT;

#if 0
  if (cachedSupportsLinks == -1) {
    doesSupportLinks();
  }
  bassert(cachedSupportsLinks == 1, NotSupported(Type::getType<FileSystem>()));

  const char* elements = path.getElements();
  if ((elements[0] == '.') && ((elements[1] == 0) || ((elements[1] == '.') && (elements[2] == 0)))) { // "." or ".."
    return false;
  }
  
  DWORD attributes = ::GetFileAttributes(ToWCharString(path));
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return false;
  }
  if ((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
    HANDLE link = ::CreateFile(ToWCharString(path),
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
        _throw FileSystemException(Type::getType<FileSystem>());
      }
    }
    
    // TAG: test if partial support works - ERROR_MORE_DATA
    PrimitiveArray<uint8> buffer(17000); // TAG: fixme
    REPARSE_DATA_BUFFER* reparseHeader = reinterpret_cast<REPARSE_DATA_BUFFER*>(static_cast<uint8*>(buffer));
    DWORD bytesWritten = 0;
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
        BASSERT(bytesWritten >= REPARSE_DATA_BUFFER_HEADER_SIZE);
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

_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct ShortcutHeader {
      LittleEndian<uint32> identifier; // 'L'
      unsigned char guid[16] = {0};
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
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

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
      ToWCharString(path),
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
    DWORD result = 0;
    bool error = ::ReadFile(link, &header, sizeof(header), &result, 0) == 0;
    ::CloseHandle(link);
    
    return !error &&
      (header.identifier == static_cast<unsigned int>('L')) &&
      (result == sizeof(header)) &&
      ((header.flags & (POINTS_TO_FILE_OR_FOLDER|HAS_RELATIVE_PATH)) == (POINTS_TO_FILE_OR_FOLDER|HAS_RELATIVE_PATH)) &&
      (compare(header.guid, GUID, sizeof(GUID)) == 0);
  } else {
    return false;
  }
#endif

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  return false;
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
      _throw FileSystemException(Type::getType<FileSystem>());
    }
  }
#endif
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
class FileSystemImpl {
public:

  // TAG: the link target may not exist
  
  static String getLinkTarget(const String& path)
  {
//     if (cachedSupportsLinks == -1) {
//       doesSupportLinks();
//     }
//     bassert(
//       cachedSupportsLinks == 1,
//       NotSupported("Symbolic link", Type::getType<FileSystem>())
//     );
    
    HANDLE link = ::CreateFile(
      ToWCharString(path),
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
        _throw FileSystemException("Not a link.", Type::getType<FileSystem>());
      }
    }
    while (link != INVALID_HANDLE_VALUE) {
      PrimitiveArray<uint8> buffer(17000); // need alternative - first attempt to get length first failed
      REPARSE_DATA_BUFFER* reparseHeader = reinterpret_cast<REPARSE_DATA_BUFFER*>(static_cast<uint8*>(buffer));
      DWORD bytesWritten = 0;
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
        //   _throw FileSystemException("Not a link.", Type::getType<FileSystem>());
        // }
        break;
      }
      
      switch (reparseHeader->ReparseTag) {
      case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
        {
          BASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0) &&
                 (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/sizeof(wchar) > 4));
          wchar* substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer +
            reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset + 4;
          // skip prefix "\??\"
          unsigned int substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2 - 4;
          substPath[substLength] = 0; // add terminator
          return String(substPath, substLength);
        }
      case IO_REPARSE_TAG_MOUNT_POINT:
        {
          BASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0) &&
                 (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/sizeof(wchar) > 4));
          wchar* substPath = reparseHeader->MountPointReparseBuffer.PathBuffer +
            reparseHeader->MountPointReparseBuffer.SubstituteNameOffset + 4;
          // skip prefix "\??\"
          unsigned int substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 - 4;
          substPath[substLength] = 0; // add terminator
          return String(substPath, substLength);
        }
      default:
        error = true;
      }
      if (error) {
        break;
      }
    }

    return String();
  }
  
  static bool enablePrivileges() noexcept
  {
    HANDLE token = NULL;
    // ATTENTION: TOKEN_PRIVILEGES includes 1 LUID_AND_ATTRIBUTES!
    char buffer[sizeof(TOKEN_PRIVILEGES) + sizeof(LUID_AND_ATTRIBUTES)]; // make room for backup and restore privileges
    clear(buffer);
    TOKEN_PRIVILEGES* privileges = (TOKEN_PRIVILEGES*)&buffer;
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
      return false;
    }
    
    privileges->PrivilegeCount = 2;
    LUID_AND_ATTRIBUTES* attributes = &privileges->Privileges[0]; // room for 2

    // enable SeBackupPrivilege
    privileges->Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // ask for privilege
    if (!::LookupPrivilegeValue(0, SE_BACKUP_NAME, &attributes[0].Luid)) {
      ::CloseHandle(token);
      return false;
    }
    
    // enable SeRestorePrivilege
    attributes[1].Attributes = SE_PRIVILEGE_ENABLED; // ask for privilege
    if (!::LookupPrivilegeValue(0, SE_RESTORE_NAME, &attributes[1].Luid)) {
      ::CloseHandle(token);
      return false;
    }
    
    if (!::AdjustTokenPrivileges(token, FALSE, privileges, 0, NULL, NULL)) {
      ::CloseHandle(token);
      return false;
    }
    ::CloseHandle(token);
    return true;
  }
  
  static inline bool makeHardLink(const String& target, const String& path) noexcept
  {
    static bool elevatedPrivileges = false;
    if (!elevatedPrivileges) {
      enablePrivileges();
      elevatedPrivileges = true;
    }
    
    HANDLE handle = ::CreateFile(ToWCharString(path), 0, 0, 0, OPEN_EXISTING, 0, 0);
    if (handle != INVALID_HANDLE_VALUE) { // make path does not exist
      ::CloseHandle(handle);
      return false;
    }
    
    handle = ::CreateFile(
      ToWCharString(target),
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
    
    wchar fullPath[MAX_PATH];
    wchar* filename = nullptr;
    ::GetFullPathName(ToWCharString(path), MAX_PATH, fullPath, &filename);
    WideString wideFullPath(fullPath);
    
    WIN32_STREAM_ID stream; // setup hard link
    stream.dwStreamId = BACKUP_LINK;
    stream.dwStreamAttributes = 0;
    stream.dwStreamNameSize = 0;
    stream.Size.QuadPart = (wideFullPath.getLength() + 1) * sizeof(wchar); // including terminator
    
    bool success = false;
    DWORD bytesWritten = 0;
    void* context = nullptr;
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

void FileSystem::makeHardLink(const String& target, const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if (_COM_AZURE_DEV__BASE__OS >= _COM_AZURE_DEV__BASE__W2K)
  ToWCharString nativePath(path);
  ToWCharString nativeTarget(target);
  bassert(::CreateHardLink(nativePath, nativeTarget, NULL) != 0,
         FileSystemException("Unable to make hard link.", Type::getType<FileSystem>()));
#else
  typedef BOOL (*PCreateHardLink)(LPCSTR, LPCSTR, LPSECURITY_ATTRIBUTES);
  static PCreateHardLink CreateHardLink = 0;
  static bool cachedCreateHardLink = false;
  if (!cachedCreateHardLink) {
    CreateHardLink = (PCreateHardLink)::GetProcAddress(::GetModuleHandle("kernel32.dll"), "CreateHardLinkA");
    cachedCreateHardLink = true;
  }
  bool error = true;
  if (CreateHardLink) {
    error = CreateHardLink(path.getElements(), target.getElements(), 0) == 0;
  } else {
    error = !FileSystemImpl::makeHardLink(target, path);
  }
  bassert(!error, FileSystemException("Unable to make hard link.", Type::getType<FileSystem>()));
#endif // w2k or later
#else // unix
  bassert(
    ::link(target.getElements(), path.getElements()) == 0,
    FileSystemException("Unable to make hard link.", Type::getType<FileSystem>())
  );
#endif // flavor
}

void FileSystem::makeLink(const String& target, const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  const bool folder = folderExists(path);
  BOOLEAN status = ::CreateSymbolicLinkW(
    ToWCharString(path.getElements()),
    ToWCharString(target.getElements()),
    (folder ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0) | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
  );
  // DWORD error = ::GetLastError();
  bassert(
    status,
    FileSystemException("Unable to make link.", Type::getType<FileSystem>())
  );

#if 0
  if (cachedSupportsLinks == -1) {
    doesSupportLinks();
  }
  bassert(cachedSupportsLinks == 1, NotSupported(Type::getType<FileSystem>()));
  
  wchar* fileNameComponent = nullptr;
  
	wchar fullTargetPath[MAX_PATH];
  bassert(
    ::GetFullPathName(
      ToWCharString(target),
      MAX_PATH,
      fullTargetPath,
      &fileNameComponent
    ),
    FileSystemException(Type::getType<FileSystem>())
  );
  
	// make the native target name
	WideString nativePath(L"\\??\\");
  nativePath += NativeWideString(fullTargetPath);
  
  DWORD attributes = ::GetFileAttributes(ToWCharString(target));
  bassert(
    attributes != INVALID_FILE_ATTRIBUTES,
    FileSystemException(Type::getType<FileSystem>())
  );
  bool isDirectory = attributes & FILE_ATTRIBUTE_DIRECTORY;
  bool directoryCreated = false;
  
  HANDLE link;
  if (isDirectory) {
    if (!nativePath.endsWith(WIDEMESSAGE(":\\"))) {
      nativePath -= WIDEMESSAGE("\\");
    }
    bassert(
      nativePath.getLength() <= MAX_PATH,
      FileSystemException(Type::getType<FileSystem>())
    ); // watch out for buffer overflow
    // we do not care whether or not it already exists
    directoryCreated = ::CreateDirectory(ToWCharString(path), 0) != 0;
    bassert(directoryCreated, FileSystemException(Type::getType<FileSystem>()));
    link = ::CreateFile(
      ToWCharString(path),
      GENERIC_WRITE,
      0,
      0,
      OPEN_EXISTING,
      FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS,
      0
    );
  } else {
    bassert(
      !nativePath.endsWith("\\"),
      FileSystemException(Type::getType<FileSystem>())
    );
    bassert(
      nativePath.getLength() <= MAX_PATH,
      FileSystemException(Type::getType<FileSystem>())
    ); // watch out for buffer overflow
    link = ::CreateFile(
      ToWCharString(path),
      GENERIC_WRITE,
      0,
      0,
      CREATE_NEW,
      FILE_FLAG_OPEN_REPARSE_POINT,
      0
    );
  }
  bassert(
    link != INVALID_HANDLE_VALUE,
    FileSystemException(Type::getType<FileSystem>())
  );
  
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
  
  Unicode::UTF8ToUTF16(
    Cast::pointer<utf16*>(reparseInfo->MountPointReparseBuffer.PathBuffer),
    Cast::pointer<const uint8*>(nativePath.getElements()),
    nativePath.getLength(),
    0
  );
  
	// set the link
  DWORD returnedLength = 0;
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
        ::RemoveDirectory(ToWCharString(path)); // clean up
      }
    } else {
      ::DeleteFile(ToWCharString(path)); // clean up // TAG: can we use FILE_FLAG_DELETE_ON_CLOSE
    }
  }
  bassert(!error, FileSystemException("Unable to make link.", Type::getType<FileSystem>()));
#endif

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  bassert(
    !::symlink(target.getElements(), path.getElements()),
    FileSystemException("Unable to make link.", Type::getType<FileSystem>())
  );
#endif // flavor
}

#if 0
class NativeHandle {
public:

  HANDLE handle = INVALID_HANDLE_VALUE;

  inline NativeHandle()
  {
  }

  inline NativeHandle(HANDLE _handle)
    handle(_handle)
  {
  }

  inline operator HANDLE()
  {
    return handle;
  }

  inline ~NativeHandle()
  {
    ::CloseHandle(handle);
  }
};
#endif

Array<String> FileSystem::split(const String path)
{
  Array<String> result;
  result.ensureCapacity(32);
  
  MemorySize current = 0;
  const MemorySize length = path.getLength();
  
  bool absolute = false;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (length >= 2) { // check for device C:
    if ((path[0] == '\\') && (path[1] == '\\')) { // UNC
      current = 2;
      absolute = true;
    } else if (ASCIITraits::isAlpha(path[0]) && (path[1] == ':')) {
      current = 2;
      if (length > current) {
        absolute = isSeparator(path[2]); // this makes path absolute
        current = 3;
      }
    } else if (length >= 1) {
      absolute = isSeparator(path[0]); // this makes path absolute
    }
  } else if (length >= 1) {
    absolute = isSeparator(path[0]);
    current = absolute ? 1 : 0;
  }
#else
  if (length >= 1) {
    absolute = isSeparator(path[0]);
    current = absolute ? 1 : 0;
  }
#endif
  
  // TAG: strip or change separators to use default separator
  if (current > 0) {
    result.append(path.substring(0, current));
  }
  while (current < length) {
    MemoryDiff index = FileSystem::findSeparator(path, current);
    if (index < 0) {
      result.append(path.substring(current));
      break;
    } else if (current < index) {
      String subpath = path.substring(current, index + 1);
      result.append(subpath);
    } else {
      // skip if empty
    }
    current = index + 1;
  }
  return result;
}

String FileSystem::join(const Array<String>& _paths)
{
  if (!_paths) {
    return String();
  }
  Array<String> paths = _paths;
  normalizeSplit(paths);
  if (!paths) {
    return String();
  }

#if 0
  MemorySize size = 0;
  for (const String& path : paths) {
    size += path.getLength() + 1;
  }
  String result(size);
#endif
  String result;
  for (const String& path : paths) {
    if (!path) {
      continue;
    }
    if (result) {
      result = getPath(result, path);
    } else {
      result.append(path);
    }
  }
  return result;
}

String FileSystem::join(const std::initializer_list<String>& paths) noexcept
{
  String result;
  if (paths.size() == 0) {
    return result;
  }
  auto i = paths.begin();
  const auto end = paths.end();
  result = *i++;
  while (i != end) {
    result = join(result, *i++);
  }
  return result;
}

String FileSystem::normalize(const String& path)
{
  Array<String> paths = split(path);
  normalizeSplit(paths);
  return join(paths);
}

String FileSystem::getRelativePath(const String& _folder, const String& _path)
{
  const String current = getCurrentFolder();
  const String folder = toAbsolutePath(current, _folder);
  const String path = toAbsolutePath(current, _path);
  Array<String> a = split(folder);
  Array<String> b = split(path);
  
  MemorySize i = 0;
  // skip common folders
  while ((i < a.getSize()) && (i < b.getSize())) {
    if (a[i] != b[i]) {
      break;
    }
    ++i;
  }
  
  if (i == 0) {
    return _path;
  }
  
  // a/b/c/d/e/f
  // a/b/c/g/h/i
  // ../../../g/h/i
  Array<String> result;
  result.ensureCapacity(a.getSize() + b.getSize());
  MemorySize j = a.getSize();
  while (j > i) {
    --j;
    result.append(parentFolder);
  }
  while (i < b.getSize()) {
    result.append(b[i]);
    ++i;
  }
  return join(result);
}

// TAG: need option disallow transparent link/strict transparency
String FileSystem::getLinkTarget(const String& path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ToWCharString nativePath(path);

  DWORD attributes = GetFileAttributesW(nativePath);
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return String();
  }
  if (attributes & (FILE_ATTRIBUTE_REPARSE_POINT == 0)) {
    return String();
  }

  // TAG: how can we get target even if target doesn't exist

  HANDLE file = ::CreateFile(
    nativePath, // file name
    FILE_READ_ATTRIBUTES | FILE_READ_EA /*| READ_CONTROL*/, // access mode
    FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
    0, // security descriptor
    OPEN_EXISTING, // how to create
    FILE_FLAG_BACKUP_SEMANTICS /*| FILE_FLAG_OPEN_REPARSE_POINT*/, // file attributes // do follow link
    0
  ); // handle to template file
  if (file == INVALID_HANDLE_VALUE) {
    return String();
  }

  String result;
  PrimitiveStackArray<wchar> buffer(1024);
  while (buffer.size() < (64 * 1024)) {
    DWORD length = GetFinalPathNameByHandleW(file, buffer, buffer.size(), FILE_NAME_NORMALIZED /*FILE_NAME_OPENED*/);
    if (length == 0) {
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        continue;
      }
      break;
    }
    result = String(buffer, length);
    break;
  }
  ::CloseHandle(file); // TAG: handle to use automation object

  file = ::CreateFile(
    nativePath, // file name
    FILE_READ_ATTRIBUTES | FILE_READ_EA /*| READ_CONTROL*/, // access mode
    FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
    0, // security descriptor
    OPEN_EXISTING, // how to create
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, // file attributes // do NOT follow link
    0
  ); // handle to template file
  if (file == INVALID_HANDLE_VALUE) {
    return String();
  }

  String source;
  while (buffer.size() < (64 * 1024)) {
    DWORD length = GetFinalPathNameByHandleW(file, buffer, buffer.size(), FILE_NAME_NORMALIZED /*FILE_NAME_OPENED*/);
    if (length == 0) {
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        continue;
      }
      break;
    }
    source = String(buffer, length);
    break;
  }

  BY_HANDLE_FILE_INFORMATION information;
  ::GetFileInformationByHandle(file, &information);
  const bool folder = (information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
  ::CloseHandle(file); // TAG: handle to use automation object

  const String root = folder ? source : FileSystem::getParent(source);
  return getRelativePath(root, result);

#if 0
  if (cachedSupportsLinks == -1) {
    doesSupportLinks();
  }
  bassert(cachedSupportsLinks == 1, NotSupported("Symbolic link.", Type::getType<FileSystem>()));
  
  ToWCharString nativePath(path);
  HANDLE link = ::CreateFile(
    nativePath,
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
      _throw FileSystemException("Not a link.", Type::getType<FileSystem>());
    }
  }
  while (link != INVALID_HANDLE_VALUE) {
    PrimitiveArray<uint8> buffer(17000); // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = reinterpret_cast<REPARSE_DATA_BUFFER*>(static_cast<uint8*>(buffer));
    DWORD bytesWritten = 0;
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
      //   _throw FileSystemException("Not a link.", Type::getType<FileSystem>());
      // }
      break;
    }
    
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      {
        BASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
          (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
        wchar* substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer +
          reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset + 4;
        // skip prefix "\??\"
        unsigned int substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2 - 4;
        substPath[substLength] = 0; // add terminator
        return String(substPath, substLength);
      }
    case IO_REPARSE_TAG_MOUNT_POINT:
      {
        BASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
               (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
        wchar* substPath = reparseHeader->MountPointReparseBuffer.PathBuffer +
          reparseHeader->MountPointReparseBuffer.SubstituteNameOffset + 4;
        // skip prefix "\??\"
        unsigned int substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 - 4;
        substPath[substLength] = 0; // add terminator
        return String(substPath, substLength);
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
  
  _COM_AZURE_DEV__BASE__PACKED__BEGIN
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
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct FileLocationInfo {
    LittleEndian<uint32> size; // size of structure and data
    LittleEndian<uint32> offset; // 0x1c
    LittleEndian<uint32> flags;
    LittleEndian<uint32> volumeOffset;
    LittleEndian<uint32> pathOffset;
    LittleEndian<uint32> networkVolumeOffset;
    LittleEndian<uint32> remainingPathOffset;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct LocalVolume {
    LittleEndian<uint32> size; // size of structure
    LittleEndian<uint32> type;
    LittleEndian<uint32> serialNumber;
    LittleEndian<uint32> labelOffset;
    char label;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct NetworkVolume {
    LittleEndian<uint32> size; // size of structure
    LittleEndian<uint32> reserved0;
    LittleEndian<uint32> shareOffset;
    LittleEndian<uint32> reserved1;
    LittleEndian<uint32> reserved2;
    char share;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

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
    link = ::CreateFile(nativePath,
                        GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                        0, OPEN_EXISTING, 0, 0);
    if (link == INVALID_HANDLE_VALUE) { // fails if directory
      // we may not have read permission or file could be locked
      // so file could be link after all but we may never know for sure
      break;
    }
    
    char buffer[4096 * 2]; // maximum link size
    const ShortcutHeader* header = (const ShortcutHeader*)buffer;
    
    DWORD linkLength = 0;
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
//       char* fullFilename = nullptr;
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
  _throw FileSystemException("Not a link.", Type::getType<FileSystem>());
#endif

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#else // unix
  char buffer[PATH_MAX + 1];
  ssize_t length = ::readlink(path.getElements(), buffer, sizeof(buffer));
  bassert(length >= 0, FileSystemException(Type::getType<FileSystem>()));
  return String(buffer, length);
#endif
}

String FileSystem::getTempFolder(TemporaryFolder folder) noexcept
{
  // TAG: need to expand variables (win32) (e.g. set TMP=%HOME%\temp)
  switch (folder) {
  case FileSystem::USER_SPECIFIED:
    {
      const Map<String, String>& environment = Application::getApplication()->getEnvironment();
      if (const auto found = environment.find(MESSAGE("TMP"))) {
        return *found;
      }
      if (const auto found = environment.find(MESSAGE("TEMP"))) {
        return *found;
      }
    }
    // fallthrough
  case FileSystem::MACHINE_NONPERSISTENT:
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return Literal("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return Literal("/tmp");
#endif // flavor
  case FileSystem::MACHINE_PERSISTENT:
  default:
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return Literal("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return Literal("/var/tmp");
#endif // flavor
  }
}

String FileSystem::getTempFileName(unsigned int options) noexcept {
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
    stream << HEX << ZEROPAD << tempfileCounter++;
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
    // first = false;
    unsigned int random = Random::random<unsigned int>();
    stream << HEX << ZEROPAD << random;
  }
  if (options & SUFFIX) {
    stream << ".tmp";
  }
  stream << FLUSH;
  return stream.getString();
}

File FileSystem::getTempFile(unsigned int options)
{
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
/*
  unsigned int attempts = 16;
  while (attempts--) {
    // change name policy if to many attempts
    String path = FileSystem::combine(getTempFolder(), getTempFileName());
    File file;
    try {
      file = File(path, opts);
      return file;
    } catch (FileNotFound&) {
    }
  }
*/
}

unsigned long FileSystem::getVariable(const String& path, Variable variable) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
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
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
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

String FileSystem::getFolder(Folder folder) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  switch (folder) {
  case FileSystem::ROOT:
    {
      wchar buffer[MAX_PATH + 1];
      UINT length = ::GetWindowsDirectoryW(buffer, getArraySize(buffer));
      if (length == 0) {
        return String();
      }
      const wchar* end = find(buffer, length, L'\\');
      if (!end) {
        return String();
      }
      return String(buffer, end - buffer + 1);
    }
  case FileSystem::DEVICES:
    return Literal("\\\\.");
  case FileSystem::TEMP:
  default:
    {
      wchar buffer[MAX_PATH + 1];
      UINT length = ::GetWindowsDirectoryW(buffer, getArraySize(buffer));
      if (length == 0) {
        return String();
      }
      return String(buffer) + MESSAGE("\\temp");
    }
  }
#else // unix
  static const Literal FOLDERS[] = {
    Literal("/"),
    Literal("/dev"),
    Literal("/tmp")
  };
  return String(FOLDERS[folder]);
#endif // flavor
}

FileSystem::Quota FileSystem::getQuota(const String& path, Trustee trustee)
{
  Quota result;
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65))
  int id = 0;
  switch (trustee.getType()) {
  case Trustee::USER:
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  case Trustee::GROUP:
#endif
    id = trustee.getIntegralId();
    break;
  default:
    _throw FileSystemException(Type::getType<FileSystem>());
  }
  
  struct dqblk temp;
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
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
    _throw FileSystemException(Type::getType<FileSystem>());
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
  result.hardLimit = temp.dqb_bhardlimit;
  result.softLimit = temp.dqb_bsoftlimit;
  result.currentUsage = temp.dqb_curblocks;
#else
  result.hardLimit = temp.dqb_ihardlimit;
  result.softLimit = temp.dqb_isoftlimit;
  result.currentUsage = temp.dqb_curinodes;
#endif
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
  struct dqblk temp;
  struct quotctl operation;
  operation.op = Q_GETQUOTA;
  operation.addr = Cast::pointer<caddr_t>(&temp);
  
  switch (trustee.getType()) {
  case Trustee::USER:
    operation.uid = trustee.getIntegralId();
    break;
  default:
    _throw FileSystemException(Type::getType<FileSystem>());
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
  _throw bindError(FileSystemException(Type::getType<FileSystem>()), errno);
#else
  result.hardLimit = 0;
  result.softLimit = 0;
  result.currentUsage = 0;
  return result;
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(FileSystem) : public UnitTest {
public:

  TEST_PRIORITY(500);
  TEST_PROJECT("base/filesystem");
  TEST_IMPACT(NORMAL);
  TEST_EXTERNAL();

  void run() override
  {
    const Path testFolder = makeFolder();
    
    auto split = FileSystem::split("a/b/c/");
    if (TEST_IS_DEFINED("writeResults")) {
      fout << "Split:" << split << ENDL;
    }
    TEST_ASSERT(split == (decltype(split){"a/","b/","c/"}));
    split = FileSystem::split("/a/b/c/");
    if (TEST_IS_DEFINED("writeResults")) {
      fout << "Split:" << split << ENDL;
    }
    TEST_ASSERT(split == (decltype(split){"/","a/","b/","c/"}));

    String relativePath = FileSystem::getRelativePath("a/b/c/", "a/b/c/g/h/i");
    String relativePath2 = FileSystem::join({"g", "h", "i"});
    TEST_ASSERT(relativePath == relativePath2);

    relativePath = FileSystem::getRelativePath("a/b/c/d/e/f", "a/b/c/g/h/i");
    relativePath2 = FileSystem::join({"..", "..", "..", "g", "h", "i"});
    TEST_ASSERT(relativePath == relativePath2);

    String path = FileSystem::getFolder(FileSystem::TEMP);
    TEST_ASSERT(FileSystem::isAbsolutePath(path));
    TEST_ASSERT(path);
    path = FileSystem::getFolder(FileSystem::ROOT);
    TEST_ASSERT(FileSystem::isAbsolutePath(path));
    TEST_ASSERT(path);
    path = FileSystem::getTempFileName();
    TEST_ASSERT(path);

    path = FileSystem::getCurrentFolder();
    TEST_ASSERT(path);

    TEST_ASSERT(FileSystem::entryExists(testFolder));
    TEST_ASSERT(FileSystem::folderExists(testFolder));
    TEST_ASSERT(!FileSystem::isAbsolutePath(testFolder));

    String textfile = testFolder / "remove.txt";
    const String exampleText = "Hello, World!\n";
    TEST_ASSERT(File::createFile(textfile, exampleText));
    TEST_ASSERT(!FileSystem::fileExists(testFolder));
    TEST_ASSERT(FileSystem::folderExists(testFolder));
    TEST_ASSERT(FileSystem::getSize(textfile) == 14);
    TEST_ASSERT(!FileSystem::isLink(textfile));

    unsigned int type = FileSystem::getType(textfile);
    TEST_ASSERT((type & FileSystem::LINK) == 0);
    TEST_ASSERT((type & FileSystem::REGULAR) != 0);
    TEST_ASSERT((type & FileSystem::FOLDER) == 0);

    type = FileSystem::getType(testFolder);
    TEST_ASSERT((type & FileSystem::FOLDER) != 0);

    if (FileSystem::doesSupportLinks()) {
      const String hardlink = testFolder / "hardlink.txt";
      if (FileSystem::fileExists(hardlink)) {
        FileSystem::removeFile(hardlink);
      }

      TEST_ASSERT(!FileSystem::entryExists(hardlink));
      try {
        FileSystem::makeHardLink(textfile, hardlink);
      } catch (...) {
        TEST_ASSERT(!"Failed to make hard link.");
      }

      const String symlink = testFolder / "symlink.txt";
      if (FileSystem::isLink(symlink)) {
        FileSystem::removeFile(symlink);
      }

      TEST_ASSERT(FileSystem::entryExists(textfile));
      TEST_ASSERT(!FileSystem::isLink(symlink));
      try {
        FileSystem::makeLink("remove.txt", symlink);
      } catch (...) {
        TEST_ASSERT(!"Failed to make symbolic link.");
      }
      TEST_ASSERT(FileSystem::isLink(symlink));
      unsigned int type = FileSystem::getType(symlink); // follows link
      TEST_ASSERT((type & FileSystem::LINK) == 0);

      String s = File::readFile(symlink);
      TEST_ASSERT(s == exampleText);
      
      String target = FileSystem::getLinkTarget(symlink);
      TEST_EQUAL(target, "remove.txt");
    }

    try {
      FileSystem::removeFile(textfile);
    } catch (...) {
      TEST_ASSERT(!"Failed to remove file.");
    }

    const String abc = testFolder / "a" / "b" / "c";
    try {
      FileSystem::makeFolderRecursive(abc);
    } catch (...) {
      TEST_ASSERT(!"Failed to create folders.");
    }

    try {
      FileSystem::removeFolder(abc);
    } catch (...) {
      TEST_ASSERT(!"Failed to remove folder.");
    }
    TEST_ASSERT(!FileSystem::folderExists(abc));

    // TAG: add support for removing all files/subfolders

    path = Path("a") / "b" / "c";
    path = Path("a") / L"b" / String("c");
    path = "a" / Path(L"b") / String("c");
    path = "a" / Path(WideString(L"b")) / WideString("c");

    TEST_ASSERT(path);
  }
};

TEST_REGISTER(FileSystem);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
