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
#include <base/filesystem/FileInfo.h>
#include <base/security/Trustee.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _WIN32_WINNT 0x0400
#  include <windows.h>
#  include <aclapi.h>
#  include <winioctl.h>
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileInfo::FileInfo(const String& _path) throw(FileSystemException)
  : path(_path), mode(0), links(0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bool error = false;
  HANDLE file = ::CreateFile(path.getElements(), // file name
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
    HANDLE link = ::CreateFile(path.getElements(), // file name
                               0 | READ_CONTROL, // access mode
                               FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                               0, // security descriptor
                               OPEN_EXISTING, // how to create
                               FILE_FLAG_OPEN_REPARSE_POINT, // file attributes
                               0 // handle to template file
    );
    assert(link != INVALID_HANDLE_VALUE, FileSystemException("Not a file", this));
    
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
      throw FileSystemException("Too many levels of symbolic links", this);
    } else {
      throw FileSystemException("Not a file", this);
    }
  }
  
  BY_HANDLE_FILE_INFORMATION information;
  error |= ::GetFileInformationByHandle(file, &information) == 0;
  
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID ownerSID;
  PSID groupSID;
  PACL acl;
  error |= ::GetSecurityInfo(file,
                             SE_FILE_OBJECT,
                             OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
                             &ownerSID,
                             &groupSID,
                             &acl,
                             0,
                             &securityDescriptor) != ERROR_SUCCESS;
  ::CloseHandle(file);
  assert(!error, FileSystemException("Not a file", this));
  
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
  trustee.ptstrName = (char*)ownerSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &ownerAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (char*)groupSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &groupAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (char*)&EVERYONE_SID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &everyoneAccessMask); // ERROR_SUCCESS expected
  
  ACL_SIZE_INFORMATION aclInfo;
  if (::GetAclInformation(acl, &aclInfo, sizeof(aclInfo), AclSizeInformation) == 0) {
    ::LocalFree(securityDescriptor);
    throw FileSystemException("Unable to get ACL", this);
  }
  
  // avoid GetEffectiveRightsFromAcl: get groups of owner and implement required functionality
  bool explicitOwner = false;
  bool explicitGroup = false;
  bool explicitEveryone = false;
  
  for (unsigned int i = 0; i < aclInfo.AceCount; ++i) {
    ACE_HEADER* ace;
    ::GetAce(acl, i, &ace);
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
  const long long fileTimeOffset = 116444736000000000ULL;
  access = (Cast::impersonate<int64>(information.ftLastAccessTime) - fileTimeOffset)/10000000;
  modification = (Cast::impersonate<int64>(information.ftLastWriteTime) - fileTimeOffset)/10000000;
  change = (Cast::impersonate<int64>(information.ftCreationTime) - fileTimeOffset)/10000000;
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
    } _DK_SDU_MIP__BASE__PACKED;
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
  #else
    struct stat64 status;
  #endif // GNU Linux
    if (::stat64(path.getElements(), (struct stat64*)&status) || (!S_ISREG(status.st_mode))) {
      throw FileSystemException("Not a file", this);
    }
  #else
    struct stat status;
    if (::stat(path.getElements(), &status) || (!S_ISREG(status.st_mode))) {
      throw FileSystemException("Not a file", this);
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
  
  size = Cast::impersonate<uint64>(status.st_size);
  owner = User(status.st_uid);
  group = Group(status.st_gid);
  access = status.st_atime;
  modification = status.st_mtime;
  change = status.st_ctime;
  links = status.st_nlink;
#endif // flavor
}

long long FileInfo::getSize() const throw() {
  return size;
}

Date FileInfo::getLastModification() const throw() {
  return modification;
}

Date FileInfo::getLastAccess() const throw() {
  return access;
}

Date FileInfo::getLastChange() const throw() {
  return change;
}

String FileInfo::getPath() const throw() {
  return path;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
