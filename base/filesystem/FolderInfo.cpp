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
#include <base/filesystem/FolderInfo.h>
#include <base/concurrency/Thread.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #define _WIN32_WINNT 0x0400
  #include <windows.h>
  #include <aclapi.h>
  #include <winioctl.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <errno.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FolderInfo::FolderInfo(const String& _path) throw(FileSystemException) : path(_path), mode(0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bool error = false;
  HANDLE folder = ::CreateFile(path.getElements(), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                               0 // handle to template file
  );
  unsigned int linkLevel = 0;
  const unsigned int maximumLinkLevel = 16;
  while ((folder == INVALID_HANDLE_VALUE) && (++linkLevel <= maximumLinkLevel)) {    
    HANDLE link = ::CreateFile(path.getElements(), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                               0 // handle to template file
    );
    assert(link != INVALID_HANDLE_VALUE, FileSystemException("Not a folder", this));
    
    // TAG: fix buffer size (protect against buffer overflow)
    char* buffer[17000]; // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)&buffer;
    DWORD bytesWritten;
    error |= ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT, // handle and ctrl
                               0, 0, // input
                               reparseHeader, sizeof(buffer), // output
                               &bytesWritten, 0) == 0;
    ::CloseHandle(link);
    assert(!error, FileSystemException(this));
    
    wchar_t* substPath;
    unsigned int substLength;
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer + reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset;
      ASSERT(reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength % 2 == 0);
      substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2;
      break;
    case IO_REPARSE_TAG_MOUNT_POINT:
      substPath = reparseHeader->MountPointReparseBuffer.PathBuffer + reparseHeader->MountPointReparseBuffer.SubstituteNameOffset;
      ASSERT(reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0);
      substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2; // keep prefix "\??\"
      break;
    default:
      throw FileSystemException("Unsupported link", this);
    }
    substPath[1] = '\\'; // convert '\??\' to '\\?\'
    substPath[substLength] = 0; // add terminator
    
    folder = ::CreateFileW(substPath, // file name
                         0 | READ_CONTROL, // access mode
                         FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                         0, // security descriptor
                         OPEN_EXISTING, // how to create
                         FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                         0 // handle to template file
    );
  }
  if (folder == INVALID_HANDLE_VALUE) {
    if (linkLevel > maximumLinkLevel) {
      throw FileSystemException("Too many levels of symbolic links", this);
    } else {
      throw FileSystemException("Not a folder", this);
    }
  }
  
  BY_HANDLE_FILE_INFORMATION information;
  error |= ::GetFileInformationByHandle(folder, &information) == 0;
  
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID ownerSID;
  PSID groupSID;
  error |= ::GetSecurityInfo(folder,
                             SE_FILE_OBJECT,
                             OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,
                             &ownerSID,
                             &groupSID,
                             0,
                             0,
                             &securityDescriptor) != ERROR_SUCCESS;
  ::CloseHandle(folder);
  if (!error) {
    owner = User(ownerSID);
    group = Group(groupSID);
    ::LocalFree(securityDescriptor);
  }
  
  static const long long fileTimeOffset = 116444736000000000ULL;
  path -= MESSAGE("\\");
  mode = PrimitiveTraits<unsigned int>::MAXIMUM;
  access = (*pointer_cast<const long long*>(&information.ftLastAccessTime) - fileTimeOffset)/10000000;
  modification = (*pointer_cast<const long long*>(&information.ftLastWriteTime) - fileTimeOffset)/10000000;
  change = (*pointer_cast<const long long*>(&information.ftCreationTime) - fileTimeOffset)/10000000;
  links = information.nNumberOfLinks;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
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
    } __attribute__ ((packed));
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64 bit aligned
  #else
    struct stat64 status;
  #endif // GNU Linux
    if (::stat64(path.getElements(), (struct stat64*)&status) || (!S_ISDIR(status.st_mode))) {
      throw FileSystemException("Not a folder", this);
    }
  #else
    struct stat status;
    if (::stat(path.getElements(), &status) || (!S_ISDIR(status.st_mode))) {
      throw FileSystemException("Not a folder", this);
    }
  #endif
    
  if (status.st_mode & S_IRUSR) { // mode reset above
    mode |= FolderInfo::RUSR;
  }
  if (status.st_mode & S_IWUSR) {
    mode |= FolderInfo::WUSR;
  }
  if (status.st_mode & S_IXUSR) {
    mode |= FolderInfo::XUSR;
  }
  if (status.st_mode & S_IRGRP) {
    mode |= FolderInfo::RGRP;
  }
  if (status.st_mode & S_IWGRP) {
    mode |= FolderInfo::WGRP;
  }
  if (status.st_mode & S_IXGRP) {
    mode |= FolderInfo::XGRP;
  }
  if (status.st_mode & S_IROTH) {
    mode |= FolderInfo::ROTH;
  }
  if (status.st_mode & S_IWOTH) {
    mode |= FolderInfo::WOTH;
  }
  if (status.st_mode & S_IXOTH) {
    mode |= FolderInfo::XOTH;
  }

  owner = User((const void*)(MemoryDiff)status.st_uid);
  group = Group((const void*)(MemoryDiff)status.st_gid);
  access = status.st_atime;
  modification = status.st_mtime;
  change = status.st_ctime;
  links = status.st_nlink;
#endif // flavor
}

FolderInfo FolderInfo::getParent() const throw(FileSystemException) {
  return *this; // not impl
}

Array<String> FolderInfo::getEntries() const throw(FileSystemException) {
  Array<String> result;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE handle;
  WIN32_FIND_DATA entry;

  if (path.endsWith(MESSAGE("\\"))) {
    handle = ::FindFirstFile((path + MESSAGE("*")).getElements(), &entry);
  } else {
    handle = ::FindFirstFile((path + MESSAGE("\\*")).getElements(), &entry);
  }
  
  if (handle == INVALID_HANDLE_VALUE) {
    if (::GetLastError() != ERROR_NO_MORE_FILES) {
      throw FileSystemException("Unable to read entries of folder", this);
    }
  } else {
    while (true) {
      result.append(String(entry.cFileName));
      if (!::FindNextFile(handle, &entry)) {
        if (::GetLastError() == ERROR_NO_MORE_FILES) {
          break;
        }
        ::FindClose(handle); // avoid that resource leak
        throw FileSystemException("Unable to read entries of folder", this);
      }
    }
    
    if (!::FindClose(handle)) {
      throw FileSystemException("Unable to close folder", this);
    }
  }
#else // unix
  Allocator<char>* buffer = Thread::getLocalStorage();

  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    DIR* directory;

    if ((directory = ::opendir(path.getElements())) == 0) {
      throw FileSystemException("Unable to read entries of folder", this);
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
        throw FileSystemException("Unable to read entries of folder", this);
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      throw FileSystemException("Unable to close folder", this);
    }
  #elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    // TAG: should detect is readdir_r is available (_DK_SDU_MIP__BASE__READDIR_R)
    #warning using non-reentrant api - readdir
    DIR* directory;
    if ((directory = ::opendir(path.getElements())) == 0) {
      throw FileSystemException("Unable to read entries of folder", this);
    }

    while (true) {
      int status;
      errno = 0;
      struct dirent* entry = ::readdir(directory);
      if (entry == 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        throw FileSystemException("Unable to read entries of folder", this);
      }
      result.append(String(entry->d_name));
    }
    if (::closedir(directory) != 0) {
      throw FileSystemException("Unable to close folder", this);
    }
  #else
    DIR* directory;
    if ((directory = ::opendir(path.getElements())) == 0) {
      throw FileSystemException("Unable to read entries of folder", this);
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
        throw FileSystemException("Unable to read entries of folder", this);
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      throw FileSystemException("Unable to close folder", this);
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
