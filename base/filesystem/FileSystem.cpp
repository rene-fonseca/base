/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #define _WIN32_WINNT 0x500
  #include <windows.h>
  #include <winioctl.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h> // lstat
  #include <fcntl.h>
  #include <unistd.h> // readlink, symlink
  #include <limits.h> // defines PATH_MAX
  #include <errno.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  throw NotImplemented(Type::getType<FileSystem>());
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  int separator = path.lastIndexOf('/');
  switch (component) {
  case FileSystem::DIRECTORY:
    if (separator > 0) {
      return path.substring(0, separator);
    } else {
      return String();
    }
  case FileSystem::NAME:
    return String();
  case FileSystem::DOTEXTENSION:
    {
      int dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot);
      } else {
        return String(); // extension not present
      }
    }
  case FileSystem::EXTENSION:
    {
      int dot = path.indexOf('.', separator + 1);
      if (dot >= 0) {
        return path.substring(dot + 1);
      } else {
        return String(); // extension not present
      }
    }
  }
#endif
}

String FileSystem::toAbsolutePath(const String& base, const String& path) throw(FileSystemException) {
  throw NotImplemented(Type::getType<FileSystem>());
}

String FileSystem::toUrl(const String& path) throw() {
  throw NotImplemented(Type::getType<FileSystem>());
}

String FileSystem::getCurrentFolder() throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
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

bool FileSystem::fileExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  assert(result != INVALID_FILE_ATTRIBUTES, FileSystemException(Type::getType<FileSystem>()));
  return ((result & FILE_ATTRIBUTE_DIRECTORY) == 0) && ((result & FILE_ATTRIBUTE_DEVICE) == 0);
  // we ignore FILE_ATTRIBUTE_REPARSE_POINT
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
#endif // flavor
}

bool FileSystem::folderExists(const String& path) throw(FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD result = ::GetFileAttributes(path.getElements());
  assert(result != INVALID_FILE_ATTRIBUTES, FileSystemException(Type::getType<FileSystem>()));
  return ((result & FILE_ATTRIBUTE_DIRECTORY) != 0); // we ignore FILE_ATTRIBUTE_REPARSE_POINT
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
  if (!::RemoveDirectory(path.getElements())) {
    throw FileSystemException("Unable to remove folder", Type::getType<FileSystem>());
  }
#else // unix
  if (rmdir(path.getElements())) {
    throw FileSystemException("Unable to remove file", Type::getType<FileSystem>());
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
  assert(attributes != INVALID_FILE_ATTRIBUTES, FileSystemException(Type::getType<FileSystem>()));
  if ((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
    HANDLE link = ::CreateFile(path.getElements(),
                               0,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               0,
                               OPEN_EXISTING,
                               FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,
                               //(attributes & FILE_ATTRIBUTE_DIRECTORY) ? (FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT) : (FILE_FLAG_OPEN_REPARSE_POINT),
                               0);
    // error code 4390 - ERROR_NOT_A_REPARSE_POINT???
    assert(link != INVALID_HANDLE_VALUE, FileSystemException(Type::getType<FileSystem>()));
    
    // TAG: test if partial support works - ERROR_MORE_DATA
    char* buffer[17000]; // TAG: fixme
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)&buffer;
    DWORD bytesWritten;
    bool error = ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT,
                                   0, 0, // input
                                   reparseHeader, sizeof(buffer), // output
                                   &bytesWritten, 0) == 0;
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
        Trace::message("qazxsw"); // TAG: FIXME
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
    // TAG: need variable to disable/enable this symbolic link
    // check if shell symbolic link
    // TAG: need support for link to link for open file, cycle, ... (problem: reparse->shell link->reparse->shell link->shell link->...)
    static const unsigned char GUID[16] = {0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
    
    typedef struct {
      LittleEndian::UnsignedInt identifier; // 'L'
      unsigned char guid[16];
      LittleEndian::UnsignedInt flags;
      LittleEndian::UnsignedInt attributes;
      LittleEndian::UnsignedLongLong time1;
      LittleEndian::UnsignedLongLong time2;
      LittleEndian::UnsignedLongLong time3;
      LittleEndian::UnsignedInt length;
      LittleEndian::UnsignedInt icon;
      LittleEndian::UnsignedInt windowMode;
      LittleEndian::UnsignedInt hotKey;
      LittleEndian::UnsignedInt reserved[2];
    } __attribute__ ((packed)) ShortcutHeader;
    
    enum Flags {
      SHELL_ITEM_ID_PRESENT = 1 << 0,
      POINTS_TO_FILE_OR_FOLDER = 1 << 1,
      HAS_DESCRIPTION = 1 << 2,
      HAS_RELATIVE_PATH = 1 << 3,
      HAS_WORKING_FOLDER = 1 << 4,
      HAS_COMMAND_LINE_ARGUMENTS = 1 << 5,
      HAS_ICON = 1 << 6
    };
    
    HANDLE link = ::CreateFile(path.getElements(),
                               GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               0,
                               OPEN_EXISTING,
                               0,
                               0);
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
  }
#else
  struct stat status;
  int result = ::lstat(path.getElements(), &status);
  switch (result) {
  case 0:
    return S_ISLNK(status.st_mode);
  case ENOENT:
    return false;
  default:
    throw FileSystemException(Type::getType<FileSystem>());
  }
#endif
}

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
bool enablePrivileges() throw() {
  HANDLE token;
	char buffer[sizeof(TOKEN_PRIVILEGES) * 2];
  TOKEN_PRIVILEGES& privileges = *((TOKEN_PRIVILEGES*)buffer);
  if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
    return false;
  }
  
  // enable SeBackupPrivilege
  if (!::LookupPrivilegeValue(0, SE_BACKUP_NAME, &privileges.Privileges[0].Luid)) {
    return false;  
  }
  
  // enable SeRestorePrivilege
  if (!::LookupPrivilegeValue(0, SE_RESTORE_NAME, &privileges.Privileges[1].Luid)) {
    return false;
  }

  privileges.PrivilegeCount = 2;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	privileges.Privileges[1].Attributes = SE_PRIVILEGE_ENABLED;

	if (!::AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), 0, 0)) {
    return false;
  }
  return true;
}

void createHardLinkForWINNT4(const String& target, const String& destination) throw() {
  if (true) { // TAG: fixme
    enablePrivileges();
  }
  
	HANDLE handle = ::CreateFile(target.getElements(), 0, 0, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE) {
		::CloseHandle(handle);
    throw FileSystemException("Unable to create hard link", Type::getType<FileSystem>());
	}
  
	handle = ::CreateFile(
    destination.getElements(),
    GENERIC_WRITE,
    0,
    0,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_POSIX_SEMANTICS,
    0
  );
	if (handle == INVALID_HANDLE_VALUE) {
    throw FileSystemException("Unable to create hard link", Type::getType<FileSystem>());
  }
  
	char fullTarget[MAX_PATH];
	char* filename;
  ::GetFullPathName(target.getElements(), MAX_PATH, fullTarget, &filename);
  WideString wideFullTarget(fullTarget);
  
	WIN32_STREAM_ID stream;
	stream.dwStreamId = BACKUP_LINK;
	stream.dwStreamAttributes = 0;
	stream.dwStreamNameSize = 0;
	stream.Size.QuadPart = (wideFullTarget.getLength() + 1) * sizeof(wchar_t); // including terminator
  
  bool success = false;
	DWORD bytesWritten;
	void* context = 0;
	if (::BackupWrite(handle, (BYTE*)&stream, (char*)&stream.cStreamName-(char*)&stream, &bytesWritten, FALSE, FALSE, &context) != 0) {
    if (bytesWritten == (char*)&stream.cStreamName-(char*)&stream) {
      if (::BackupWrite(handle, (BYTE*)wideFullTarget.getElements(), stream.Size.LowPart, &bytesWritten, FALSE, FALSE, &context) != 0) {
        if (bytesWritten == stream.Size.LowPart) {
          success = true;
        }
      }
    }
    // release the backup context
    ::BackupWrite(handle, (BYTE*)fullTarget, 0, &bytesWritten, TRUE, FALSE, &context); // abort
  }
	::CloseHandle(handle);
  assert(success, FileSystemException("Unable to create hard link", Type::getType<FileSystem>()));
}
#endif // flavor

// #if defined(DEBUG)
//   #define DMESSAGE(literal) MESSAGE(literal)
// #else
//   #define DMESSAGE(literal) MESSAGE("")
// #endif

// #define IMESSAGE(literal) MESSAGE(literal)

void FileSystem::makeHardLink(const String& target, const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__W2K)
  assert(::CreateHardLink(path.getElements(), target.getElements(), 0) != 0,
         FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#else
  typedef BOOL (*PCreateHardLink)(LPCSTR, LPCSTR, LPSECURITY_ATTRIBUTES);
  static PCreateHardLink CreateHardLink = 0;
  if (!CreateHardLink) {
    CreateHardLink = (PCreateHardLink)::GetProcAddress(::GetModuleHandle("kernel32.dll"), "CreateHardLinkA");
    assert(CreateHardLink, NotSupported("Hard link", Type::getType<FileSystem>()));
  }
  assert(CreateHardLink(path.getElements(), target.getElements(), 0) != 0,
         FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#endif // w2k or later
#else // unix
  assert(::link(target.getElements(), path.getElements()) == 0, FileSystemException("Unable to make hard link", Type::getType<FileSystem>()));
#endif // flavor
}

void FileSystem::makeLink(const String& target, const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    supportsLinks();
  }
  assert(cachedSupportsLinks == 1, NotSupported(Type::getType<FileSystem>()));
  
  char* fileNameComponent;
  
	char fullTargetPath[MAX_PATH];
  assert(
    ::GetFullPathName(target.getElements(), MAX_PATH, fullTargetPath, &fileNameComponent),
    FileSystemException(Type::getType<FileSystem>())
  );
  
	// make the native target name
	WideString nativePath(WIDEMESSAGE("\\??\\"));
  nativePath += fullTargetPath;
  
  DWORD attributes = ::GetFileAttributes(target.getElements());
  assert(attributes != INVALID_FILE_ATTRIBUTES, FileSystemException(Type::getType<FileSystem>()));
  bool isDirectory = attributes & FILE_ATTRIBUTE_DIRECTORY;
  bool directoryCreated = false;
  
  HANDLE link;
  if (isDirectory) {
    if (!nativePath.endsWith(WIDEMESSAGE(":\\"))) {
      nativePath -= WIDEMESSAGE("\\");
    }
    assert(nativePath.getLength() <= MAX_PATH, FileSystemException(Type::getType<FileSystem>())); // watch out for buffer overflow
    directoryCreated = ::CreateDirectory(path.getElements(), 0) != 0; // we do not care whether or not it already exists
    assert(directoryCreated, FileSystemException(Type::getType<FileSystem>()));
    link = ::CreateFile(path.getElements(),
                        GENERIC_WRITE,
                        0,
                        0,
                        OPEN_EXISTING,
                        FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS,
                        0);
  } else {
    assert(!nativePath.endsWith(WIDEMESSAGE("\\")), FileSystemException(Type::getType<FileSystem>()));
    assert(nativePath.getLength() <= MAX_PATH, FileSystemException(Type::getType<FileSystem>())); // watch out for buffer overflow
    link = ::CreateFile(path.getElements(),
                        GENERIC_WRITE,
                        0,
                        0,
                        CREATE_NEW,
                        FILE_FLAG_OPEN_REPARSE_POINT,
                        0);
  }
  assert(link != INVALID_HANDLE_VALUE, FileSystemException(Type::getType<FileSystem>()));
  
  char reparseBuffer[sizeof(REPARSE_DATA_BUFFER) + MAX_PATH * sizeof(WCHAR)];
  REPARSE_DATA_BUFFER* reparseInfo = (REPARSE_DATA_BUFFER*)reparseBuffer;
  clear(*reparseInfo);
  if (isDirectory) {
    reparseInfo->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
  } else {
    reparseInfo->ReparseTag = 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK;
  }

  reparseInfo->ReparseDataLength = nativePath.getLength() * sizeof(WCHAR) + 12;
  reparseInfo->MountPointReparseBuffer.SubstituteNameLength = nativePath.getLength() * sizeof(WCHAR);
  reparseInfo->MountPointReparseBuffer.PrintNameOffset = reparseInfo->MountPointReparseBuffer.SubstituteNameLength + sizeof(WCHAR);
  copy<WCHAR>(reparseInfo->MountPointReparseBuffer.PathBuffer, nativePath.getElements(), nativePath.getLength()); // exclude terminator
  
	// set the link
  DWORD returnedLength;
  bool error = ::DeviceIoControl(link, FSCTL_SET_REPARSE_POINT,
                                 reparseInfo, reparseInfo->ReparseDataLength + REPARSE_DATA_BUFFER_HEADER_SIZE,
                                 0, 0,
                                 &returnedLength, 0) == 0;
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
  assert(!::symlink(target.getElements(), path.getElements()),
         FileSystemException("Unable to make link", Type::getType<FileSystem>()));
#endif // flavor
}

// TAG: need option allow no transparent link/strict transparency
String FileSystem::getLink(const String& path) throw(NotSupported, FileSystemException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (cachedSupportsLinks == -1) {
    supportsLinks();
  }
  assert(cachedSupportsLinks == 1, NotSupported("Symbolic link", Type::getType<FileSystem>()));
  
  HANDLE link = ::CreateFile(path.getElements(),
                             0,
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             0,
                             OPEN_EXISTING,
                             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
                             0);
  while (link != INVALID_HANDLE_VALUE) {
    // TAG: fix buffer size
    char* buffer[17000]; // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)&buffer;
    DWORD bytesWritten;
    bool error = ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT,
                                   0, 0,
                                   reparseHeader, sizeof(buffer),
                                   &bytesWritten, 0) == 0;
    if (error) {
      // bool reparse = ::GetLastError() != 4390; // ERROR_NOT_A_REPARSE_POINT
      ::CloseHandle(link);
      // if (reparse) { // no need to check for shell link
      //   throw FileSystemException("Not a link", Type::getType<FileSystem>());
      // }
      break;
    }
    
    wchar_t* substPath;
    unsigned int substLength;
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
             (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
      substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer + reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset + 4;
      substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2 - 4; // skip prefix "\??\"
      break;
    case IO_REPARSE_TAG_MOUNT_POINT:
      ASSERT((reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0) &&
             (reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 > 4));
      substPath = reparseHeader->MountPointReparseBuffer.PathBuffer + reparseHeader->MountPointReparseBuffer.SubstituteNameOffset + 4;
      substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2 - 4; // skip prefix "\??\"
      break;
    default:
      error = true;
    }
    
    if (error) {
      break;
    }
    
    substPath[substLength] = 0; // add terminator
    return WideString::getMultibyteString(substPath /*, substLength*/); // TAG: fixme
  }
  
  // TAG: need variable to disable/enable this symbolic link
  // TAG: need support for link to link for open file, cycle, ... (problem: reparse->shell link->reparse->shell link->shell link->...)
  // check if shell symbolic link
  static const unsigned char GUID[16] = {0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
  
  typedef struct {
    LittleEndian::UnsignedInt identifier; // 'L'
    unsigned char guid[16];
    LittleEndian::UnsignedInt flags;
    LittleEndian::UnsignedInt attributes;
    LittleEndian::UnsignedLongLong time1;
    LittleEndian::UnsignedLongLong time2;
    LittleEndian::UnsignedLongLong time3;
    LittleEndian::UnsignedInt length;
    LittleEndian::UnsignedInt icon;
    LittleEndian::UnsignedInt windowMode;
    LittleEndian::UnsignedInt hotKey;
    LittleEndian::UnsignedInt reserved[2];
  } __attribute__ ((packed)) ShortcutHeader;
  
  typedef struct {
    LittleEndian::UnsignedInt size; // size of structure and data
    LittleEndian::UnsignedInt offset; // 0x1c
    LittleEndian::UnsignedInt flags;
    LittleEndian::UnsignedInt volumeOffset;
    LittleEndian::UnsignedInt pathOffset;
    LittleEndian::UnsignedInt networkVolumeOffset;
    LittleEndian::UnsignedInt remainingPathOffset;
  } __attribute__ ((packed)) FileLocationInfo;
  
  typedef struct {
    LittleEndian::UnsignedInt size; // size of structure
    LittleEndian::UnsignedInt type;
    LittleEndian::UnsignedInt serialNumber;
    LittleEndian::UnsignedInt labelOffset;
    char label;
  } __attribute__ ((packed)) LocalVolume;
  
  typedef struct {
    LittleEndian::UnsignedInt size; // size of structure
    LittleEndian::UnsignedInt reserved0;
    LittleEndian::UnsignedInt shareOffset;
    LittleEndian::UnsignedInt reserved1;
    LittleEndian::UnsignedInt reserved2;
    char share;
  } __attribute__ ((packed)) NetworkVolume;
  
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
    const ShortcutHeader* header = (const ShortcutHeader*)&buffer;
    
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
      if (offset + sizeof(LittleEndian::UnsignedShort) > linkLength) {
        break;
      }
      const LittleEndian::UnsignedShort* shellItemSize = (const LittleEndian::UnsignedShort*)(buffer + offset);
      offset += *shellItemSize + sizeof(LittleEndian::UnsignedShort);
    }
    
    if (header->flags & POINTS_TO_FILE_OR_FOLDER) {
      if (offset + sizeof(FileLocationInfo) > linkLength) {
        break;
      }
      const FileLocationInfo* fileLocationInfo = (const FileLocationInfo*)(buffer + offset);
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
        if (offset + sizeof(LittleEndian::UnsignedShort) > linkLength) {
          break;
        }
        const LittleEndian::UnsignedShort* length = (const LittleEndian::UnsignedShort*)(buffer + offset);
        offset += *length + sizeof(LittleEndian::UnsignedShort); // skip description
        if (offset > linkLength) {
          break;
        }
      }

      // read relative path
      if (offset + sizeof(LittleEndian::UnsignedShort) > linkLength) {
        break;
      }
      const LittleEndian::UnsignedShort* length = (const LittleEndian::UnsignedShort*)(buffer + offset);
      if (offset + *length + sizeof(LittleEndian::UnsignedShort) > linkLength) { // make sure path fits in file (with terminator)
        break;
      }
      
      // we have to do some basic validation 'cause anybody can create a shell link
      // TAG: check if valid chars
      // TAG: if path looks valid
//       if (linkLength + 1 > sizeof(buffer)) {
//         break; // no room for terminator
//       }
//       buffer[offset + sizeof(LittleEndian::UnsignedShort) + *length] = 0; // add terminator
//       char fullPath[MAX_PATH];
//       char* fullFilename;
//       bool error = ::GetFullPathName(buffer + offset + sizeof(LittleEndian::UnsignedShort),
//                                      sizeof(fullPath),
//                                      fullPath,
//                                      &fullFilename) == 0;
//       if (error) {
//         break;
//       }
      return String(buffer + offset + sizeof(LittleEndian::UnsignedShort), *length); // return relative path
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
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    return MESSAGE("C:\\temp"); // TAG: fixme - use same drive as windows directory
#else // unix
    return MESSAGE("/tmp");
#endif // flavor
  case MACHINE_PERSISTENT:
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
