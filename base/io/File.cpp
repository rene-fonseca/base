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
#include <base/io/File.h>
#include <base/io/EndOfFile.h>
#include <base/concurrency/Thread.h>
#include <base/Base.h>
#include <base/Functor.h>
#include <base/NotImplemented.h>
#include <base/security/User.h>

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
#  define _LARGEFILE64_SOURCE 1
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadFileContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteFileContext.h> // platform specific
#  if !defined(_WIN32_WINNT)
#    define _WIN32_WINNT 0x0400
#  endif
#  include <windows.h>
#  include <aclapi.h>
#  include <winioctl.h>
#  ifdef SYNCHRONIZE
#    undef SYNCHRONIZE
     enum {SYNCHRONIZE = 0x100000};
#  endif
#else // unix
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/time.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <limits.h>
#  include <string.h> // required by FD_SET on solaris
#  include <sys/mman.h>

#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
#    include <sys/acl.h> // solaris and irix
enum {
  _COM_SUN__ACL_USER = USER,
  _COM_SUN__ACL_GROUP = GROUP
};
#    undef USER
#    undef GROUP
#  elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
#    include <sys/acl.h> // irix
#  endif

#  undef assert
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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  inline Date FileTimeToDate(const FILETIME& time) {
    return Date((Cast::impersonate<int64>(time) - 116444736000000000LL)/10000000);
  }
#endif // flavor

File::FileHandle::~FileHandle() throw(FileException) {
  // TAG: throw exception if region of file is still locked
  if (isValid()) { // dont try to close if handle is invalidated
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    if (!::CloseHandle(getHandle())) {
      throw FileException("Unable to close file", this);
    }
#else // unix
    if (::close(getHandle())) {
      throw FileException("Unable to close file", this);
    }
#endif // flavor
  }
}

File::File() throw() : fd(File::FileHandle::invalid) {}

File::File(const String& path, Access access, unsigned int options) throw(AccessDenied, FileNotFound) : fd(File::FileHandle::invalid) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD creationFlags;
  switch (options & (CREATE | TRUNCATE)) {
  case 0:
    creationFlags = OPEN_EXISTING;
    break;
  case CREATE:
    creationFlags = (options & EXCLUSIVE) ? CREATE_NEW : OPEN_ALWAYS;
    break;
  case TRUNCATE:
    creationFlags = TRUNCATE_EXISTING;
    break;
  case CREATE | TRUNCATE:
    creationFlags = (options & EXCLUSIVE) ? CREATE_NEW : CREATE_ALWAYS;
    break;
  }
  
  bool error = false;
  OperatingSystem::Handle handle = ::CreateFile( // TAG: check out FILE_FLAG_POSIX_SEMANTICS
    path.getElements(), // file name
    (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)), // access mode
    (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), // share mode
    0, // security descriptor
    creationFlags, // how to create
    FILE_ATTRIBUTE_NORMAL | ((options & ASYNCHRONOUS) ? FILE_FLAG_OVERLAPPED : 0), // file attributes
    0 // handle to template file
  );
  const DWORD originalError = GetLastError();
  unsigned int linkLevel = 0;
  const unsigned int maximumLinkLevel = 16;
  while ((handle == OperatingSystem::INVALID_HANDLE) && (++linkLevel <= maximumLinkLevel)) {    
    OperatingSystem::Handle link = ::CreateFile(path.getElements(), // file name
                                                0 | READ_CONTROL, // access mode
                                                FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
                                                0, // security descriptor
                                                OPEN_EXISTING, // how to create
                                                FILE_FLAG_OPEN_REPARSE_POINT, // file attributes
                                                0 // handle to template file
    );
    if (link == OperatingSystem::INVALID_HANDLE) {
      break;
    }
    
    // TAG: fix buffer size (protect against buffer overflow)
    char* buffer[17000]; // need alternative - first attempt to get length first failed
    REPARSE_DATA_BUFFER* reparseHeader = (REPARSE_DATA_BUFFER*)&buffer;
    DWORD bytesWritten;
    error |= ::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT, // handle and ctrl
                               0, 0, // input
                               reparseHeader, sizeof(buffer), // output
                               &bytesWritten, 0) == 0;
    ::CloseHandle(link);
    if (error) {
      break;
    }
    
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
      throw FileNotFound("Unsupported link", this);
    }
    substPath[1] = '\\'; // convert '\??\' to '\\?\'
    substPath[substLength] = 0; // add terminator
    
    handle = ::CreateFileW( // TAG: check out FILE_FLAG_POSIX_SEMANTICS
      substPath, // file name
      (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)), // access mode
      (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), // share mode
      0, // security descriptor
      creationFlags, // how to create
      FILE_ATTRIBUTE_NORMAL | ((options & ASYNCHRONOUS) ? FILE_FLAG_OVERLAPPED : 0), // file attributes
      0 // handle to template file
    );
  }
  if (handle == OperatingSystem::INVALID_HANDLE) {
    if (linkLevel > maximumLinkLevel) {
      throw FileNotFound("Too many levels of symbolic links", this);
    } else if (originalError == ERROR_ACCESS_DENIED) {
      throw AccessDenied(this);
    } else {
      throw FileNotFound("Unable to open file", this);
    }
  }
  
  fd = new FileHandle(handle);
#else // unix
  // TAG: exclusive file locking problem for NFS
#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  int flags = O_LARGEFILE;
#else
  int flags = 0;
#endif

  switch (access) {
  case READ:
    flags |= O_RDONLY;
    break;
  case WRITE:
    flags |= O_WRONLY;
    break;
  case READWRITE:
    flags |= O_RDWR;
    break;
  }

  if (options & CREATE) {
    flags |= O_CREAT;
  }
  if (options & TRUNCATE) {
    flags |= O_TRUNC;
  }
  if (options & EXCLUSIVE) {
    flags |= O_EXCL;
  }

  OperatingSystem::Handle handle;
  if ((handle = ::open(path.getElements(), flags, S_IRUSR | S_IWUSR | S_IRGRP)) == OperatingSystem::INVALID_HANDLE) {
    if (errno == EACCES) {
      throw AccessDenied(this);
    } else {
      throw FileNotFound("Unable to open file", this);
    }
  }
  fd = new FileHandle(handle);
#endif
  if (options & File::APPEND) {
    setPosition(0, File::END);
  }
}

File& File::operator=(const File& eq) throw() {
  fd = eq.fd; // no need to protect against self-assignment
  return *this;
}

void File::close() throw(FileException) {
  fd = FileHandle::invalid; // invalidate
}

bool File::isClosed() const throw() {
  return !fd->isValid();
}

// TAG: need methods get owner, get group...

unsigned int File::getMode() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID ownerSID;
  PSID groupSID;
  PACL acl;
  bool error = ::GetSecurityInfo(fd->getHandle(),
                                 SE_FILE_OBJECT,
                                 OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
                                 &ownerSID, &groupSID, &acl, 0,
                                 &securityDescriptor) != ERROR_SUCCESS;
  assert(!error, FileException("Not a file", this));
  
  const DWORD ownerSize = ::GetLengthSid((PSID)ownerSID);
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
    throw FileException("Unable to get ACL", this);
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

  unsigned int mode = 0;
  if (explicitOwner) {
    if (ownerAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= File::RUSR;
    }
    if (ownerAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= File::WUSR;
    }
    if (ownerAccessMask & FILE_EXECUTE) {
      mode |= File::XUSR;
    }
  }

  if (explicitGroup) {
    if (groupAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= File::RGRP;
    }
    if (groupAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= File::WGRP;
    }
    if (groupAccessMask & FILE_EXECUTE) {
      mode |= File::XGRP;
    }
  }

  if (explicitEveryone) {
    if (everyoneAccessMask & (FILE_READ_DATA|FILE_READ_ATTRIBUTES|FILE_READ_EA|READ_CONTROL)) { // READ_CONTROL should not be required
      mode |= File::ROTH;
    }
    if (everyoneAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|DELETE|WRITE_OWNER)) {
      mode |= File::WOTH;
    }
    if (everyoneAccessMask & FILE_EXECUTE) {
      mode |= File::XOTH;
    }
  }
  return mode;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64 bit aligned
    int error = ::fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int error = ::fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int error = ::fstat(fd->getHandle(), &status);
  #endif
  assert(error == 0, FileException(this));

#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  if ((S_ISUID == File::SET_UID) && (S_ISGID == File::SET_GID) && (S_ISVTX == File::RESTRICT) &&
      (S_IRUSR == File::RUSR) && (S_IWUSR == File::WUSR) && (S_IXUSR == File::XUSR) &&
      (S_IRGRP == File::RGRP) && (S_IWGRP == File::WGRP) && (S_IXGRP == File::XGRP) &&
      (S_IROTH == File::ROTH) && (S_IWOTH == File::WOTH) && (S_IXOTH == File::XOTH)) {
    return status.st_mode & (File::PERMISSION_MASK|STICKY_MASK);
  } else {
#endif
    unsigned int result = 0;
#if defined(S_ISUID)
    if (status.st_mode & S_ISUID) {
      result |= File::SET_UID;
    }
#endif
#if defined(S_ISGID)
    if (status.st_mode & S_ISGID) {
      result |= File::SET_GID;
    }
#endif
#if defined(S_ISVTX)
    if (status.st_mode & S_ISVTX) {
      result |= File::RESTRICT;
    }
#endif
    if (status.st_mode & S_IRUSR) {
      result |= File::RUSR;
    }
    if (status.st_mode & S_IWUSR) {
      result |= File::WUSR;
    }
    if (status.st_mode & S_IXUSR) {
      result |= File::XUSR;
    }
    if (status.st_mode & S_IRGRP) {
      result |= File::RGRP;
    }
    if (status.st_mode & S_IWGRP) {
      result |= File::WGRP;
    }
    if (status.st_mode & S_IXGRP) {
      result |= File::XGRP;
    }
    if (status.st_mode & S_IROTH) {
      result |= File::ROTH;
    }
    if (status.st_mode & S_IWOTH) {
      result |= File::WOTH;
    }
    if (status.st_mode & S_IXOTH) {
      result |= File::XOTH;
    }
    return result;
#if defined(S_ISUID) && defined(S_ISGID) && defined(S_ISVTX)
  }
#endif  
#endif // flavor
}

AccessControlList File::getACL() const throw(FileException) {
  AccessControlList result;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SECURITY_DESCRIPTOR* securityDescriptor;
  PACL acl;
  assert(::GetSecurityInfo(fd->getHandle(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
                           0, 0, &acl, 0, &securityDescriptor) == ERROR_SUCCESS,
         FileException("Unable to get ACL", this)
  );

  SECURITY_DESCRIPTOR_CONTROL control;
  DWORD revision;
	if (::GetSecurityDescriptorControl(securityDescriptor, &control, &revision) == 0) {
    ::LocalFree(securityDescriptor);
		throw FileException("Unable to get ACL", this);
	}
  DWORD size = ::GetSecurityDescriptorLength(securityDescriptor); // TAG: remove when done
  
//   ferr << "Security descriptor:" << EOL
//        << "  size: " << size << EOL
//        << "  revision: " << revision << EOL
//        << "  control: " << HEX << (WORD)control << EOL;

//   ULONG numberOfEntries; // TAG: what is explicit entries
//   PEXPLICIT_ACCESS entries;
//   if (::GetExplicitEntriesFromAcl(acl, &numberOfEntries, &entries) != ERROR_SUCCESS) {
//     ::LocalFree(securityDescriptor);
//     throw FileException("Unable to get ACL", this);
//   }

//   ferr << "Explicit entries:" << EOL
//        << "  #entries: " << numberOfEntries << EOL;
  
//   PEXPLICIT_ACCESS entry = entries;
//   for (unsigned int i = 0; i < numberOfEntries; ++i) {
//     ferr << "  index: " << i << EOL
//          << "  permissions: " << HEX << entry->grfAccessPermissions << EOL
//          << "  access mode: " << entry->grfAccessMode << EOL
//          << "  inherit: " << HEX << entry->grfInheritance << EOL
//          << "  form: " << entry->Trustee.TrusteeForm << EOL
//          << "  type: " << entry->Trustee.TrusteeType << EOL;
//     ++entry;
//   }
//   ::LocalFree(entries);

  ACL_SIZE_INFORMATION information;
  if (::GetAclInformation(acl, &information, sizeof(information), AclSizeInformation) == 0) {
      ::LocalFree(securityDescriptor);
      throw FileException("Unable to get ACL", this);
  }

//   ferr << "ACL:" << EOL
//        << "  #entries: " << information.AceCount << EOL
//        << "  bytes used: " << information.AclBytesInUse << EOL
//        << "  bytes free: " << information.AclBytesFree << EOL;
  
  for (unsigned int i = 0; i < information.AceCount; ++i) {
    ACE_HEADER* ace;
    ::GetAce(acl, i, &ace);
    
    AccessControlEntry::AccessMask accessMask = 0;
    
    PSID sid;
    ACCESS_MASK mask;
    bool denied = false;
    switch (ace->AceType) {
    case ACCESS_ALLOWED_ACE_TYPE:
      mask = ((ACCESS_ALLOWED_ACE*)ace)->Mask;
      sid = &((ACCESS_ALLOWED_ACE*)ace)->SidStart;
      break;
    case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
      mask = ((ACCESS_ALLOWED_OBJECT_ACE*)ace)->Mask;
      sid = &((ACCESS_ALLOWED_OBJECT_ACE*)ace)->SidStart;
      break;
    case ACCESS_DENIED_ACE_TYPE:
      mask = ((ACCESS_DENIED_ACE*)ace)->Mask;
      sid = &((ACCESS_DENIED_ACE*)ace)->SidStart;
      denied = true;
      break;
    case ACCESS_DENIED_OBJECT_ACE_TYPE:
      mask = ((ACCESS_DENIED_OBJECT_ACE*)ace)->Mask;
      sid = &((ACCESS_DENIED_OBJECT_ACE*)ace)->SidStart;
      denied = true;
      break;
    }
    
    if (mask & FILE_EXECUTE) {
      accessMask |= AccessControlEntry::EXECUTE;
    }

    if (mask & FILE_READ_DATA) {
      accessMask |= AccessControlEntry::READ_DATA;
    }
    if (mask & FILE_READ_ATTRIBUTES) {
      accessMask |= AccessControlEntry::READ_ATTRIBUTES;
    }
    if (mask & FILE_READ_EA) {
      accessMask |= AccessControlEntry::READ_EXTENDED_ATTRIBUTES;
    }
    if (mask & READ_CONTROL) {
      accessMask |= AccessControlEntry::READ_PERMISSIONS;
    }

    if (mask & FILE_WRITE_DATA) {
      accessMask |= AccessControlEntry::WRITE_DATA;
    }
    if (mask & FILE_APPEND_DATA) {
      accessMask |= AccessControlEntry::APPEND_DATA;
    }
    if (mask & FILE_WRITE_ATTRIBUTES) {
      accessMask |= AccessControlEntry::CHANGE_ATTRIBUTES;
    }
    if (mask & FILE_WRITE_EA) {
      accessMask |= AccessControlEntry::CHANGE_EXTENDED_ATTRIBUTES;
    }
    if (mask & WRITE_DAC) {
      accessMask |= AccessControlEntry::CHANGE_PERMISSIONS;
    }
    if (mask & WRITE_OWNER) {
      accessMask |= AccessControlEntry::CHANGE_OWNER;
    }
    if (mask & DELETE) {
      accessMask |= AccessControlEntry::REMOVE;
    }
    if (mask & FILE_DELETE_CHILD) {
      accessMask |= AccessControlEntry::REMOVE_COMPONENT;
    }
    if (mask & SYNCHRONIZE) {
      accessMask |= AccessControlEntry::SYNCHRONIZE;
    }
    
    AccessControlEntry::Permissions permissions;
    if (denied) {
      permissions.allowed = 0;
      permissions.denied = accessMask;
    } else {
      permissions.allowed = accessMask;
      permissions.denied = 0;
    }
    
//     ferr << "ACE: " << i << EOL
//          << "  type: " << ace->AceType << EOL
//          << "  flags: " << ace->AceFlags << EOL
//          << "  size: " << ace->AceSize << EOL
//          << "  access: " << setWidth(8) << ZEROPAD << HEX << mask << EOL
//          << "  id: " << Trustee(Trustee::UNSPECIFIED, (const void*)sid) << EOL;
    
    result.add(AccessControlEntry(Trustee(Trustee::UNSPECIFIED, (const void*)sid), permissions));
  }
  ::LocalFree(securityDescriptor);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  aclent_t entries[MAX_ACL_ENTRIES];
  int numberOfEntries = ::facl(fd->getHandle(), GETACL, MAX_ACL_ENTRIES, entries);
  assert(numberOfEntries >= 0, FileException("Unable to get ACL", this));
  
  for (int i = 0; i < numberOfEntries; ++i) {
    const unsigned int mode = ((entries[i].a_perm & S_IRGRP) ? AccessControlEntry::READ : 0) |
      ((entries[i].a_perm & S_IWGRP) ? AccessControlEntry::WRITE : 0) |
      ((entries[i].a_perm & S_IXGRP) ? AccessControlEntry::EXECUTE : 0);
    const int type = entries[i].a_type;
    if (type & USER_OBJ) {
      result.add(AccessControlEntry(Trustee(Trustee::USER, (void*)(MemoryDiff)entries[i].a_id), ((mode & S_IRUSR) ? AccessControlEntry::READ : 0) | ((mode & S_IWUSR) ? AccessControlEntry::WRITE : 0) | ((mode & S_IXUSR) ? AccessControlEntry::EXECUTE : 0)));
    } else if (type & _COM_SUN__ACL_USER) {
      result.add(AccessControlEntry(Trustee(Trustee::USER, (void*)(MemoryDiff)entries[i].a_id), ((mode & S_IRUSR) ? AccessControlEntry::READ : 0) | ((mode & S_IWUSR) ? AccessControlEntry::WRITE : 0) | ((mode & S_IXUSR) ? AccessControlEntry::EXECUTE : 0)));
    } else if (type & GROUP_OBJ) {
      result.add(AccessControlEntry(Trustee(Trustee::GROUP, (void*)(MemoryDiff)entries[i].a_id), ((mode & S_IRGRP) ? AccessControlEntry::READ : 0) | ((mode & S_IWGRP) ? AccessControlEntry::WRITE : 0) | ((mode & S_IXGRP) ? AccessControlEntry::EXECUTE : 0)));
    } else if (type & _COM_SUN__ACL_GROUP) {
      result.add(AccessControlEntry(Trustee(Trustee::GROUP, (void*)(MemoryDiff)entries[i].a_id), ((mode & S_IRGRP) ? AccessControlEntry::READ : 0) | ((mode & S_IWGRP) ? AccessControlEntry::WRITE : 0) | ((mode & S_IXGRP) ? AccessControlEntry::EXECUTE : 0)));
    } else if (type & CLASS_OBJ) {
      result.add(AccessControlEntry(Trustee(Trustee::CLASS, (void*)(MemoryDiff)entries[i].a_id), mode));
    } else if (type & OTHER_OBJ) {
      result.add(AccessControlEntry(Trustee(Trustee::EVERYONE, 0), ((mode & S_IROTH) ? AccessControlEntry::READ : 0) | ((mode & S_IWOTH) ? AccessControlEntry::WRITE : 0) | ((mode & S_IXOTH) ? AccessControlEntry::EXECUTE : 0)));
    }
  }
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
  throw NotImplemented(this);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    assert(::fstat64(fd->getHandle(), &status) == 0, FileException(this));
  #else
    struct stat status;
    assert(::fstat(fd->getHandle(), &status) == 0, FileException(this));
  #endif
    
//   const unsigned int ownerMode = ((status.st_mode & S_IRUSR) ? AccessControlEntry::READ : 0) |
//     ((status.st_mode & S_IWUSR) ? AccessControlEntry::WRITE : 0) |
//     ((status.st_mode & S_IXUSR) ? AccessControlEntry::EXECUTE : 0);
//   result.add(AccessControlEntry(User((const void*)(MemoryDiff)status.st_uid), AccessControlEntry::ALLOW, ownerMode)); // TAG: mark as owner in ACL?
  
//   const unsigned int groupMode = ((status.st_mode & S_IRGRP) ? AccessControlEntry::READ : 0) |
//     ((status.st_mode & S_IWGRP) ? AccessControlEntry::WRITE : 0) |
//     ((status.st_mode & S_IXGRP) ? AccessControlEntry::EXECUTE : 0);
//   if (groupMode != 0) {
//     result.add(AccessControlEntry(Group((const void*)(MemoryDiff)status.st_gid), AccessControlEntry::ALLOW, groupMode));
//   }
  
//   const unsigned int otherMode = ((status.st_mode & S_IROTH) ? AccessControlEntry::READ : 0) |
//     ((status.st_mode & S_IWOTH) ? AccessControlEntry::WRITE : 0) |
//     ((status.st_mode & S_IXOTH) ? AccessControlEntry::EXECUTE : 0);
//   if (otherMode != 0) {
//     result.add(AccessControlEntry(Trustee::EVERYONE, AccessControlEntry::ALLOW, otherMode));
//   }
#endif // flavor
  return result;
}

Trustee File::getOwner() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID ownerSID;
  assert(::GetSecurityInfo(fd->getHandle(), SE_FILE_OBJECT,
                           OWNER_SECURITY_INFORMATION, &ownerSID, 0, 0, 0,
                           &securityDescriptor) == ERROR_SUCCESS,
         FileException(this)
  );
  Trustee owner(Trustee::UNSPECIFIED, (const void*)ownerSID);
  ::LocalFree(securityDescriptor);
  return owner;
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
    if (::fstat64(fd->getHandle(), (struct stat64*)&status) || (!S_ISREG(status.st_mode))) {
      throw FileException("Not a file", this);
    }
  #else
    struct stat status;
    if (::fstat(fd->getHandle(), &status) || (!S_ISREG(status.st_mode))) {
      throw FileException("Not a file", this);
    }
  #endif
  return Trustee(Trustee::USER, (const void*)(MemoryDiff)status.st_uid);
#endif // flavor
}

void File::changeOwner(const String& path, const Trustee& owner, const Trustee& group, bool followLink) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotImplemented(Type::getType<File>()); // TAG: fixme

  //Privilege::askFor(Privilege::CHANGE_OWNER|Privilege::RESTORE|Privilege::BACKUP|Privilege::CHANGE_NOTIFY);

// if (!::InitializeSecurityDescriptor(psdFileSDrel, SECURITY_DESCRIPTOR_REVISION)) {
// }
// if (!::SetSecurityDescriptorOwner(psdFileSDrel, psidOwner, FALSE)) {
// }
// if (!::IsValidSecurityDescriptor(psdFileSDrel)) {
// }
// if (!::SetFileSecurity(path.getElements(), (SECURITY_INFORMATION)(OWNER_SECURITY_INFORMATION), psdFileSDrel)) {
// }


#else // unix
  assert((owner.getType() == Trustee::USER) && (group.getType() == Trustee::GROUP), FileException(Type::getType<File>()));
  
  uid_t uid = owner.getIntegralId();
  gid_t gid = group.getIntegralId();
  
  int error;
  if (followLink) {
    error = ::chown(path.getElements(), uid, gid);
  } else {
    error = ::lchown(path.getElements(), uid, gid);
  }
  assert(error == 0, FileException("Unable to change owner", Type::getType<File>()));
#endif // flavor
}

Trustee File::getGroup() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SECURITY_DESCRIPTOR* securityDescriptor;
  PSID groupSID;
  assert(::GetSecurityInfo(fd->getHandle(), SE_FILE_OBJECT,
                           GROUP_SECURITY_INFORMATION, 0, &groupSID, 0, 0,
                           &securityDescriptor) == ERROR_SUCCESS,
         FileException(this)
  );
  Trustee group(Trustee::UNSPECIFIED, (const void*)groupSID);
  ::LocalFree(securityDescriptor);
  return group;
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
    if (::fstat64(fd->getHandle(), (struct stat64*)&status) || (!S_ISREG(status.st_mode))) {
      throw FileException("Not a file", this);
    }
  #else
    struct stat status;
    if (::fstat(fd->getHandle(), &status) || (!S_ISREG(status.st_mode))) {
      throw FileException("Not a file", this);
    }
  #endif
  return Trustee(Trustee::GROUP, (const void*)(MemoryDiff)status.st_gid);
#endif // flavor
}

long long File::getSize() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #if (_DK_SDU_MIP__BASE__OS >= _DK_SDU_MIP__BASE__W2K)
    LARGE_INTEGER size; // TAG: unresolved possible byte order problem for big endian architectures
    assert(::GetFileSizeEx(fd->getHandle(), &size), FileException("Unable to get file size", this));
  #else
    ULARGE_INTEGER size;
    size.LowPart = ::GetFileSize(fd->getHandle(), &size.HighPart);
    if ((size.LowPart == INVALID_FILE_SIZE) && (::GetLastError() != NO_ERROR )) {
      throw FileException("Unable to get file size", this);
    }
  #endif
  return size.QuadPart;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64 bit aligned
    int result = ::fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = ::fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    assert(result == 0, FileException("Unable to get file size", this));
    return status.st_size;
#endif
}

long long File::getPosition() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LARGE_INTEGER position;
  position.QuadPart = 0;
  position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
  if ((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position", this);
  }
//  if (!::SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) {
//    throw FileException("Unable to get file position", this);
//  }
  return position.QuadPart;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    return ::lseek64(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #else
    return ::lseek(fd->getHandle(), 0, SEEK_CUR); // should never fail
  #endif
#endif
}

void File::setPosition(long long position, Whence whence) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static DWORD relativeTo[] = {FILE_BEGIN, FILE_CURRENT, FILE_END};
  LARGE_INTEGER temp;
  temp.QuadPart = position;
  temp.LowPart = ::SetFilePointer(fd->getHandle(), temp.LowPart, &temp.HighPart, relativeTo[whence]);
  if ((temp.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    throw FileException("Unable to get file position", this);
  }
//  if (!SetFilePointerEx(fd->getHandle(), position, 0, relativeTo[whence])) {
//    throw FileException("Unable to set position", this);
//  }
#else // unix
  static int relativeTo[] = {SEEK_SET, SEEK_CUR, SEEK_END};
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    ::lseek64(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #else
    assert(position <= PrimitiveTraits<int>::MAXIMUM, FileException("Unable to set position", this));
    ::lseek(fd->getHandle(), position, relativeTo[whence]); // should never fail
  #endif
#endif
}

void File::truncate(long long size) throw(FileException) {
  long long oldSize = File::getSize();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  setPosition(size);
  if (!::SetEndOfFile(fd->getHandle())) {
    throw FileException("Unable to truncate", this);
  }
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    if (::ftruncate64(fd->getHandle(), size)) {
      throw FileException("Unable to truncate", this);
    }
  #else
    assert((size >= 0) && (size <= PrimitiveTraits<int>::MAXIMUM), FileException("Unable to truncate", this));
    if (::ftruncate(fd->getHandle(), size)) {
      throw FileException("Unable to truncate", this);
    }
  #endif
  if (File::getSize() > oldSize) { // has file been extended
    return;
  }
#endif
  if (size > oldSize) {
    Allocator<char>* buffer = Thread::getLocalStorage();
    fill<char>(buffer->getElements(), buffer->getSize(), 0);
    setPosition(oldSize);
    long long count = size - oldSize;
    while (count > 0) {
      count -= write(buffer->getElements(), minimum<long long>(count, buffer->getSize())); // blocking write
    }
  }
}

void File::flush() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::FlushFileBuffers(fd->getHandle())) {
    throw FileException("Unable to flush", this);
  }
#else // unix
  if (fsync(fd->getHandle())) {
    throw FileException("Unable to flush", this);
  }
#endif
}

void File::lock(const FileRegion& region, bool exclusive) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to lock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped
  )) {
    ::CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
  ::CloseHandle(overlapped.hEvent);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to lock region", this);
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(fd->getHandle(), F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to lock region", this);
      }
      break;
    }
  #endif
#endif
}

bool File::tryLock(const FileRegion& region, bool exclusive) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to lock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    fd->getHandle(),
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped)) {
    CloseHandle(overlapped.hEvent);
    throw FileException("Unable to lock region", this);
  }
  DWORD result = ::WaitForSingleObject(overlapped.hEvent, 0); // return immediately
  ::CloseHandle(overlapped.hEvent);
  return result == WAIT_OBJECT_0; // was the region locked
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(fd->getHandle(), F_SETLK64, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region", this);
    }
    return true;
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(fd->getHandle(), F_SETLK, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      throw FileException("Unable to lock region", this);
    }
    return true;
  #endif
#endif
}

void File::unlock(const FileRegion& region) throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    throw FileException("Unable to unlock region", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::UnlockFileEx(fd->getHandle(), 0, size.LowPart, size.HighPart, &overlapped)) {
    ::CloseHandle(overlapped.hEvent);
    throw FileException("Unable to unlock region", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
  ::CloseHandle(overlapped.hEvent);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    assert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region", this));
    struct flock64 lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region", this);
      }
      break;
    }
  #else
    assert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to unlock region", this)
    );
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(fd->getHandle(), F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        throw FileException("Unable to unlock region", this);
      }
      break;
    }
  #endif
#endif
}

Date File::getLastModification() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), 0, 0, &time)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64 bit aligned
    int result = ::fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = ::fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    assert(result == 0, FileException("Unable to get status", this));
    return Date(status.st_mtime);
#endif
}

Date File::getLastAccess() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), 0, &time, 0)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
    int result = fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    assert(result == 0, FileException("Unable to get status", this));
    return Date(status.st_atime);
#endif
}

Date File::getLastChange() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(fd->getHandle(), &time, 0, 0)) {
    throw FileException("Unable to get file time", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
    struct packedStat64 status; // TAG: GLIBC: st_size is not 64bit aligned
    int result = fstat64(fd->getHandle(), (struct stat64*)&status);
  #else
    struct stat64 status;
    int result = fstat64(fd->getHandle(), &status);
  #endif // GNU Linux
  #else
    struct stat status;
    int result = ::fstat(fd->getHandle(), &status);
  #endif // LFS
    assert(result == 0, FileException("Unable to get status", this));
    return Date(status.st_ctime);
#endif
}

unsigned long File::getVariable(Variable variable) throw(FileException, NotSupported) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(this);
#else // unix
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
    long result = ::fpathconf(fd->getHandle(), fileSystemVariable);
    if (!((result == -1) && (errno != 0))) {
      return result;
    }
  }
  if ((errno == EBADF) || (errno == EINVAL)) {
    throw FileException("Unable to get variable", this);
  }
  // TAG: add POSIX values here?
  throw NotSupported(this);
#endif // flavor
}

unsigned int File::read(char* buffer, unsigned int bytesToRead, bool nonblocking) throw(FileException) {
  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::ReadFile(fd->getHandle(), buffer, bytesToRead, &result, 0);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking read
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(fd->getHandle(), 0, 0, bytesToRead, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::ReadFile(fd->getHandle(), buffer, bytesToRead, &result, 0)) {
                  if (::UnlockFileEx(fd->getHandle(), 0, bytesToRead, 0, &overlapped)) { // // release shared lock
                    ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              ::CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to read from file", this);
    }
#else // unix
    int result;
    do {
      result = ::read(fd->getHandle(), buffer, minimum<unsigned int>(bytesToRead, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) { // remember that errno is local to the thread
        case EINTR: // interrupted by signal before any data was read
          break; // try again
        case EAGAIN: // no data available (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to read from file", this);
        }
      }
    } while (result < 0);
#endif
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int File::write(const char* buffer, unsigned int bytesToWrite, bool nonblocking) throw(FileException) {
  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    DWORD result;
    BOOL success = ::WriteFile(fd->getHandle(), buffer, bytesToWrite, &result, 0);
    if (!success) {
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking write
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(fd->getHandle(), 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(fd->getHandle(), 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(fd->getHandle(), 0, 0, bytesToWrite, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::WriteFile(fd->getHandle(), buffer, bytesToWrite, &result, 0)) {
                  if (::UnlockFileEx(fd->getHandle(), 0, bytesToWrite, 0, &overlapped)) { // // release shared lock
                    WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
                    success = true;
                  }
                }
              }
              ::CloseHandle(overlapped.hEvent);
            }
          }
        }
      }
    }
    if (!success) {
      throw FileException("Unable to write to file", this);
    }
#else // unix
    int result;
    do {
      result = ::write(fd->getHandle(), buffer, minimum<unsigned int>(bytesToWrite, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) {
        case EINTR: // interrupted by signal before any data was written
          break; // try again
        case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
          result = 0;
          break;
        default:
          throw FileException("Unable to write to file", this);
        }
      }
    } while (result < 0);
#endif
    if (result == 0) {
      if (nonblocking) {
        break;
      } else {
        throw FileException("Unable to write to file", this);
      }
    }
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

void File::asyncCancel() throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::CancelIo(getHandle());
#else // unix
#endif // flavor
}

AsynchronousReadOperation File::read(char* buffer, unsigned int bytesToRead, unsigned long long offset, AsynchronousReadEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncReadFileContext(getHandle(), buffer, bytesToRead, offset, listener);
#endif // flavor
}

AsynchronousWriteOperation File::write(const char* buffer, unsigned int bytesToWrite, unsigned long long offset, AsynchronousWriteEventListener* listener) throw(AsynchronousException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncWriteFileContext(getHandle(), buffer, bytesToWrite, offset, listener);
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
