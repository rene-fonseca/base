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
#include <base/filesystem/FolderInfo.h>
#include <base/filesystem/FileSystemException.h>
#include <base/concurrency/Thread.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _WIN32_WINNT _WIN32_WINNT_WINXP
#  include <windows.h>
#  include <aclapi.h>
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

#if !defined(IO_REPARSE_TAG_SYMBOLIC_LINK)
#  define IO_REPARSE_TAG_SYMBOLIC_LINK 0
#endif

#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <unistd.h>
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS)
#  include <dirent.h>
#endif
#  include <errno.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

FolderInfo::FolderInfo()
{
}

FolderInfo::FolderInfo(const String& _path)
  : path(_path), mode(0), links(0)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bool error = false;
  HANDLE folder = ::CreateFile(ToWCharString(path), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                               0 // handle to template file
  );
  if (folder == INVALID_HANDLE_VALUE) {
    switch (::GetLastError()) {
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION: // possible with page file
    case ERROR_LOCK_VIOLATION:
      return;
    }
  }
  unsigned int linkLevel = 0;
  const unsigned int maximumLinkLevel = 16;
  while ((folder == INVALID_HANDLE_VALUE) && (++linkLevel <= maximumLinkLevel)) {    
    HANDLE link = ::CreateFile(ToWCharString(path), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                               0 // handle to template file
    );
    bassert(link != INVALID_HANDLE_VALUE, FileSystemException("Not a folder.", this));
    
    PrimitiveArray<uint8> buffer(17000); // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = reinterpret_cast<REPARSE_DATA_BUFFER*>(static_cast<uint8*>(buffer));
    DWORD bytesWritten = 0;
#if 0 // TAG: FIXME
    error |= ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT, // handle and ctrl
                               0, 0, // input
                               reparseHeader, sizeof(buffer), // output
                               &bytesWritten, 0) == 0;
#endif
    ::CloseHandle(link);
    bassert(!error, FileSystemException(this));
    
    wchar* substPath = nullptr;
    unsigned int substLength = 0;
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer + reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset;
      BASSERT(reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength % 2 == 0);
      substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2;
      break;
    case IO_REPARSE_TAG_MOUNT_POINT:
      substPath = reparseHeader->MountPointReparseBuffer.PathBuffer + reparseHeader->MountPointReparseBuffer.SubstituteNameOffset;
      BASSERT(reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0);
      substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2; // keep prefix "\??\"
      break;
    default:
      _throw FileSystemException("Unsupported link.", this);
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
    // TAG: try software link if INVALID_HANDLE_VALUE
  }
  if (folder == INVALID_HANDLE_VALUE) {
    if (linkLevel > maximumLinkLevel) {
      _throw FileSystemException("Too many levels of symbolic links.", this);
    } else {
      _throw FileSystemException("Not a folder.", this);
    }
  }
  
  BY_HANDLE_FILE_INFORMATION information;
  error |= ::GetFileInformationByHandle(folder, &information) == 0;
  
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID ownerSID = nullptr;
  PSID groupSID = nullptr;
  PACL acl = nullptr;
  error |= ::GetSecurityInfo(folder,
                             SE_FILE_OBJECT,
                             OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
                             &ownerSID,
                             &groupSID,
                             &acl,
                             0,
                             &securityDescriptor) != ERROR_SUCCESS;
  ::CloseHandle(folder);
  bassert(!error, FileSystemException("Not a folder.", this));

  owner = User((const void*)ownerSID);
  const DWORD ownerSize = ::GetLengthSid((PSID)ownerSID);
  group = Group((const void*)groupSID);
  const DWORD groupSize = ::GetLengthSid((PSID)groupSID);
  
  static const char EVERYONE_SID[1 + 1 + 6 + 4] = {
    1, // revision
    1, // number of sub authorities
    0, 0, 0, 0, 0, 1, // SECURITY_WORLD_SID_AUTHORITY
    0, 0, 0, 0 // SECURITY_WORLD_RID
  };
  const DWORD everyoneSize = sizeof(EVERYONE_SID);
  
  ACCESS_MASK ownerAccessMask;
  ACCESS_MASK groupAccessMask;
  ACCESS_MASK everyoneAccessMask;
  
  TRUSTEE trustee;
  trustee.pMultipleTrustee = 0;
  trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
  trustee.TrusteeForm = TRUSTEE_IS_SID;
  trustee.TrusteeType = TRUSTEE_IS_UNKNOWN;
  trustee.ptstrName = (wchar*)ownerSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &ownerAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (wchar*)groupSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &groupAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (wchar*)&EVERYONE_SID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &everyoneAccessMask); // ERROR_SUCCESS expected
  
  ACL_SIZE_INFORMATION aclInfo;
  if (::GetAclInformation(acl, &aclInfo, sizeof(aclInfo), AclSizeInformation) == 0) {
    ::LocalFree(securityDescriptor);
    _throw FileSystemException("Unable to get ACL.", this);
  }
  
  // avoid GetEffectiveRightsFromAcl: get groups of owner and implement required functionality
  bool explicitOwner = false;
  bool explicitGroup = false;
  bool explicitEveryone = false;
  
  for (unsigned int i = 0; i < aclInfo.AceCount; ++i) {
    ACE_HEADER* ace;
    ::GetAce(acl, i, (void**)&ace); // TAG: why cast under cygwin
    PSID sid;
    ACCESS_MASK mask;
    switch (ace->AceType) {
    case ACCESS_ALLOWED_ACE_TYPE:
      mask = ((ACCESS_ALLOWED_ACE*)ace)->Mask;
      sid = &((ACCESS_ALLOWED_ACE*)ace)->SidStart;
      break;
    case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
      mask = ((ACCESS_ALLOWED_OBJECT_ACE*)ace)->Mask;
      sid = &((ACCESS_ALLOWED_OBJECT_ACE*)ace)->SidStart;
      break;
    default:
      continue;
    }
    
    const DWORD sidSize = ::GetLengthSid(sid);
    
    if ((sidSize == ownerSize) && (compare((const char*)sid, (const char*)ownerSID, sidSize) == 0)) {
      explicitOwner = true;
      ownerAccessMask &= mask; // leave explicit/inherited permissions
    } else if ((sidSize == groupSize) && (compare((const char*)sid, (const char*)groupSID, sidSize) == 0)) {
      explicitGroup = true;
      groupAccessMask &= mask; // leave explicit/inherited permissions
    } else if ((sidSize == everyoneSize) && (compare((const char*)sid, (const char*)EVERYONE_SID, sidSize) == 0)) {
      explicitEveryone = true;
      everyoneAccessMask &= mask; // leave explicit/inherited permissions
    }
  }
  ::LocalFree(securityDescriptor);

  if (explicitOwner) {
    if (ownerAccessMask & (FILE_LIST_DIRECTORY|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FolderInfo::RUSR;
    }
    if (ownerAccessMask & (FILE_ADD_FILE|FILE_ADD_SUBDIRECTORY|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FolderInfo::WUSR;
    }
    if (ownerAccessMask & FILE_TRAVERSE) {
      mode |= FolderInfo::XUSR;
    }
  }

  if (explicitGroup) {
    if (groupAccessMask & (FILE_LIST_DIRECTORY|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FolderInfo::RGRP;
    }
    if (groupAccessMask & (FILE_ADD_FILE|FILE_ADD_SUBDIRECTORY|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FolderInfo::WGRP;
    }
    if (groupAccessMask & FILE_TRAVERSE) {
      mode |= FolderInfo::XGRP;
    }
  }

  if (explicitEveryone) {
    if (everyoneAccessMask & (FILE_LIST_DIRECTORY|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FolderInfo::ROTH;
    }
    if (everyoneAccessMask & (FILE_ADD_FILE|FILE_ADD_SUBDIRECTORY|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FolderInfo::WOTH;
    }
    if (everyoneAccessMask & FILE_TRAVERSE) {
      mode |= FolderInfo::XOTH;
    }
  }

  static const long long fileTimeOffset = 116444736000000000LL;
  path -= Literal("\\");
  access = (Cast::impersonate<int64>(information.ftLastAccessTime) - fileTimeOffset)/10;
  modification = (Cast::impersonate<int64>(information.ftLastWriteTime) - fileTimeOffset)/10;
  change = (Cast::impersonate<int64>(information.ftCreationTime) - fileTimeOffset)/10;
  links = information.nNumberOfLinks;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::stat64(path.getElements(), &status) || (!S_ISDIR(status.st_mode))) {
      _throw FileSystemException("Not a folder.", this);
    }
  #else
    struct stat status;
    if (::stat(path.getElements(), &status) || (!S_ISDIR(status.st_mode))) {
      _throw FileSystemException("Not a folder.", this);
    }
  #endif
    
  // mode reset above
#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  if ((S_ISUID == FolderInfo::SET_UID) && (S_ISGID == FolderInfo::SET_GID) && (S_ISVTX == FolderInfo::RESTRICT) &&
      (S_IRUSR == FolderInfo::RUSR) && (S_IWUSR == FolderInfo::WUSR) && (S_IXUSR == FolderInfo::XUSR) &&
      (S_IRGRP == FolderInfo::RGRP) && (S_IWGRP == FolderInfo::WGRP) && (S_IXGRP == FolderInfo::XGRP) &&
      (S_IROTH == FolderInfo::ROTH) && (S_IWOTH == FolderInfo::WOTH) && (S_IXOTH == FolderInfo::XOTH)) {
    mode = status.st_mode & (FolderInfo::PERMISSION_MASK|FolderInfo::STICKY_MASK);
  } else {
#endif
#if defined(S_ISUID)
  if (status.st_mode & S_ISUID) {
    mode |= FolderInfo::SET_UID;
  }
#endif
#if defined(S_ISGID)
  if (status.st_mode & S_ISGID) {
    mode |= FolderInfo::SET_GID;
  }
#endif
#if defined(S_ISVTX)
  if (status.st_mode & S_ISVTX) {
    mode |= FolderInfo::RESTRICT;
  }
#endif
  if (status.st_mode & S_IRUSR) {
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
#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  }
#endif
  owner = User(status.st_uid);
  group = Group(status.st_gid);
  access = status.st_atime * 1000000LL;
  modification = status.st_mtime * 1000000LL;
  change = status.st_ctime * 1000000LL;
  links = status.st_nlink;
#endif // flavor
}

FolderInfo FolderInfo::getParent() const
{
  return FolderInfo(); // not impl
}

Array<String> FolderInfo::getEntries() const
{
  Array<String> result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle;
  WIN32_FIND_DATA entry;

  if (path.endsWith("\\")) {
    handle = ::FindFirstFile(ToWCharString(path + "*"), &entry);
  } else {
    handle = ::FindFirstFile(ToWCharString(path+ "\\*"), &entry);
  }
  
  if (handle == INVALID_HANDLE_VALUE) {
    if (::GetLastError() != ERROR_NO_MORE_FILES) {
      _throw FileSystemException("Unable to read entries of folder.", this);
    }
  } else {
    while (true) {
      result.append(String(entry.cFileName));
      if (!::FindNextFile(handle, &entry)) {
        if (::GetLastError() == ERROR_NO_MORE_FILES) {
          break;
        }
        ::FindClose(handle); // avoid that resource leak
        _throw FileSystemException("Unable to read entries of folder.", this);
      }
    }
    
    if (!::FindClose(handle)) {
      _throw FileSystemException("Unable to close folder.", this);
    }
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS)
  return result;
#else // unix
  Thread::UseThreadLocalBuffer _buffer;
  Allocator<uint8>& buffer = _buffer;

  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    DIR* directory = nullptr;

    if ((directory = ::opendir(path.getElements())) == 0) {
      _throw FileSystemException("Unable to read entries of folder.", this);
    }

    while (true) {
      int status = 0;
      struct dirent64* entry = nullptr;

      errno = 0;
      if ((status = ::readdir64_r(directory, Cast::pointer<struct dirent64*>(buffer.getElements()), &entry)) != 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        _throw FileSystemException("Unable to read entries of folder.", this);
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      _throw FileSystemException("Unable to close folder.", this);
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    // TAG: should detect is readdir_r is available (_COM_AZURE_DEV__BASE__READDIR_R)
    // #warning using non-reentrant api - readdir
    DIR* directory = nullptr;
    if ((directory = ::opendir(path.getElements())) == 0) {
      _throw FileSystemException("Unable to read entries of folder.", this);
    }

    while (true) {
      int status = 0;
      errno = 0;
      struct dirent* entry = ::readdir(directory);
      if (entry == 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        _throw FileSystemException("Unable to read entries of folder.", this);
      }
      result.append(String(entry->d_name));
    }
    if (::closedir(directory) != 0) {
      _throw FileSystemException("Unable to close folder.", this);
    }
  #else
    DIR* directory = nullptr;
    if ((directory = ::opendir(path.getElements())) == 0) {
      _throw FileSystemException("Unable to read entries of folder.", this);
    }
  
    buffer.setSize(maximum(buffer.getSize(), sizeof(struct dirent) + 1));

    while (true) {
      int status = 0;
      struct dirent* entry = nullptr;
      
      errno = 0;
      if ((status = ::readdir_r(directory, Cast::pointer<struct dirent*>(buffer.getElements()), &entry)) != 0) {
        if (errno == 0) { // stop if last entry has been read
          break;
        }
        ::closedir(directory);
        _throw FileSystemException("Unable to read entries of folder.", this);
      }
      if (!entry) { // only required for Linux
        break;
      }
      result.append(String(entry->d_name));
    }

    if (::closedir(directory) != 0) {
      _throw FileSystemException("Unable to close folder.", this);
    }
  #endif
#endif // flavor
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
extern "C" int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
  return -1;
}
#endif
