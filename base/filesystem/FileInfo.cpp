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
#include <base/filesystem/FileInfo.h>
#include <base/filesystem/FileSystemException.h>
#include <base/security/Trustee.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _WIN32_WINNT _WIN32_WINNT_WINXP
#  include <windows.h>
#  include <aclapi.h>
// #  include <winioctl.h>

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
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FileInfo::FileInfo(const String& _path)
  : path(_path)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bool error = false;
  HANDLE file = ::CreateFile(ToWCharString(path), // file name
                             0 | READ_CONTROL, // access mode
                             FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                             0, // security descriptor
                             OPEN_EXISTING, // how to create
                             0, // file attributes
                             0 // handle to template file
  );
  if (file == INVALID_HANDLE_VALUE) {
    switch (::GetLastError()) {
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION: // possible with page file
    case ERROR_LOCK_VIOLATION:
      return;
    }
  }
  unsigned int linkLevel = 0;
  const unsigned int maximumLinkLevel = 16;
  while ((file == INVALID_HANDLE_VALUE) && (++linkLevel <= maximumLinkLevel)) {    
    HANDLE link = ::CreateFile(ToWCharString(path), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_OPEN_REPARSE_POINT, // file attributes
                               0 // handle to template file
    );
    bassert(link != INVALID_HANDLE_VALUE, FileSystemException("Not a file.", this));
    
    // TAG: fix buffer size (protect against buffer overflow)
    char* buffer[17000]; // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)&buffer;
    DWORD bytesWritten = 0;
    error |= ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT, // handle and ctrl
                               0, 0, // input
                               reparseHeader, sizeof(buffer), // output
                               &bytesWritten, 0) == 0;
    ::CloseHandle(link);
    bassert(!error, FileSystemException(this));
    
    wchar* substPath = nullptr;
    unsigned int substLength = 0;
    switch (reparseHeader->ReparseTag) {
    case 0x80000000|IO_REPARSE_TAG_SYMBOLIC_LINK:
      substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer +
        reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset;
      BASSERT(
        reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength % 2 == 0
      );
      substLength =
        reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength/2;
      break;
    case IO_REPARSE_TAG_MOUNT_POINT:
      substPath = reparseHeader->MountPointReparseBuffer.PathBuffer +
        reparseHeader->MountPointReparseBuffer.SubstituteNameOffset;
      BASSERT(
        reparseHeader->MountPointReparseBuffer.SubstituteNameLength % 2 == 0
      );
      substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength/2; // keep prefix "\??\"
      break;
    default:
      throw FileSystemException("Unsupported link.", this);
    }
    substPath[1] = '\\'; // convert '\??\' to '\\?\'
    substPath[substLength] = 0; // add terminator
    
    file = ::CreateFileW(substPath, // file name
                         0 | READ_CONTROL, // access mode
                         FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                         0, // security descriptor
                         OPEN_EXISTING, // how to create
                         0, // file attributes
                         0 // handle to template file
    );
    // TAG: try software link if INVALID_HANDLE_VALUE
  }
  if (file == INVALID_HANDLE_VALUE) {
    if (linkLevel > maximumLinkLevel) {
      throw FileSystemException("Too many levels of symbolic links.", this);
    } else {
      throw FileSystemException("Not a file.", this);
    }
  }
  
  BY_HANDLE_FILE_INFORMATION information;
  error |= ::GetFileInformationByHandle(file, &information) == 0;
  
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID ownerSID = nullptr;
  PSID groupSID = nullptr;
  PACL acl = nullptr;
  error |= ::GetSecurityInfo(
    file,
    SE_FILE_OBJECT,
    OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
    &ownerSID,
    &groupSID,
    &acl,
    0,
    &securityDescriptor
  ) != ERROR_SUCCESS;
  
  ::CloseHandle(file);
  bassert(!error, FileSystemException("Not a file.", this));
  
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
    throw FileSystemException("Unable to get ACL.", this);
  }
  
  // avoid GetEffectiveRightsFromAcl: get groups of owner and implement required functionality
  bool explicitOwner = false;
  bool explicitGroup = false;
  bool explicitEveryone = false;
  
  for (unsigned int i = 0; i < aclInfo.AceCount; ++i) {
    ACE_HEADER* ace = nullptr;
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
    if (ownerAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FileInfo::RUSR;
    }
    if (ownerAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FileInfo::WUSR;
    }
    if (ownerAccessMask & FILE_EXECUTE) {
      mode |= FileInfo::XUSR;
    }
  }

  if (explicitGroup) {
    if (groupAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FileInfo::RGRP;
    }
    if (groupAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FileInfo::WGRP;
    }
    if (groupAccessMask & FILE_EXECUTE) {
      mode |= FileInfo::XGRP;
    }
  }

  if (explicitEveryone) {
    if (everyoneAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= FileInfo::ROTH;
    }
    if (everyoneAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= FileInfo::WOTH;
    }
    if (everyoneAccessMask & FILE_EXECUTE) {
      mode |= FileInfo::XOTH;
    }
  }
  
  ULARGE_INTEGER temp;
  temp.LowPart = information.nFileSizeLow;
  temp.HighPart = information.nFileSizeHigh;
  size = temp.QuadPart;
  const long long fileTimeOffset = 116444736000000000LL;
  access = (Cast::impersonate<int64>(information.ftLastAccessTime) - fileTimeOffset)/10;
  modification = (Cast::impersonate<int64>(information.ftLastWriteTime) - fileTimeOffset)/10;
  change = (Cast::impersonate<int64>(information.ftCreationTime) - fileTimeOffset)/10;
  links = information.nNumberOfLinks;
  
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::stat64(path.getElements(), &status) || (!S_ISREG(status.st_mode))) {
      throw FileSystemException("Not a file.", this);
    }
  #else
    struct stat status;
    if (::stat(path.getElements(), &status) || (!S_ISREG(status.st_mode))) {
      throw FileSystemException("Not a file.", this);
    }
  #endif

  // mode reset above
#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  if ((S_ISUID == FileInfo::SET_UID) && (S_ISGID == FileInfo::SET_GID) && (S_ISVTX == FileInfo::RESTRICT) &&
      (S_IRUSR == FileInfo::RUSR) && (S_IWUSR == FileInfo::WUSR) && (S_IXUSR == FileInfo::XUSR) &&
      (S_IRGRP == FileInfo::RGRP) && (S_IWGRP == FileInfo::WGRP) && (S_IXGRP == FileInfo::XGRP) &&
      (S_IROTH == FileInfo::ROTH) && (S_IWOTH == FileInfo::WOTH) && (S_IXOTH == FileInfo::XOTH)) {
    mode = status.st_mode & (FileInfo::PERMISSION_MASK|FileInfo::STICKY_MASK);
  } else {
#endif
#if defined(S_ISUID)
  if (status.st_mode & S_ISUID) {
    mode |= FileInfo::SET_UID;
  }
#endif
#if defined(S_ISGID)
  if (status.st_mode & S_ISGID) {
    mode |= FileInfo::SET_GID;
  }
#endif
#if defined(S_ISVTX)
  if (status.st_mode & S_ISVTX) {
    mode |= FileInfo::RESTRICT;
  }
#endif
  if (status.st_mode & S_IRUSR) {
    mode |= FileInfo::RUSR;
  }
  if (status.st_mode & S_IWUSR) {
    mode |= FileInfo::WUSR;
  }
  if (status.st_mode & S_IXUSR) {
    mode |= FileInfo::XUSR;
  }
  if (status.st_mode & S_IRGRP) {
    mode |= FileInfo::RGRP;
  }
  if (status.st_mode & S_IWGRP) {
    mode |= FileInfo::WGRP;
  }
  if (status.st_mode & S_IXGRP) {
    mode |= FileInfo::XGRP;
  }
  if (status.st_mode & S_IROTH) {
    mode |= FileInfo::ROTH;
  }
  if (status.st_mode & S_IWOTH) {
    mode |= FileInfo::WOTH;
  }
  if (status.st_mode & S_IXOTH) {
    mode |= FileInfo::XOTH;
  }
#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  }
#endif
  
  size = status.st_size;
  owner = User(status.st_uid);
  group = Group(status.st_gid);
  access = status.st_atime * 1000000LL;
  modification = status.st_mtime * 1000000LL;
  change = status.st_ctime * 1000000LL;
  links = status.st_nlink;
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
