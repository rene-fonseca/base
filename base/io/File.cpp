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
#include <base/io/File.h>
#include <base/io/AccessDenied.h>
#include <base/io/EndOfFile.h>
#include <base/io/FileException.h>
#include <base/io/FileNotFound.h>
#include <base/filesystem/FileSystem.h>
#include <base/concurrency/Thread.h>
#include <base/Base.h>
#include <base/Functor.h>
#include <base/security/User.h>
#include <base/UnitTest.h>
#include <base/ResourceHandle.h>
#include <base/Profiler.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
#  define _LARGEFILE64_SOURCE 1
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadFileContext.h> // platform specific
#  include <base/platforms/win32/AsyncWriteFileContext.h> // platform specific
#  if !defined(_WIN32_WINNT)
#    define _WIN32_WINNT _WIN32_WINNT_WINXP
#  endif
#  include <windows.h>
#  include <aclapi.h>
#  include <winioctl.h>
#  include <base/platforms/win32/Reparse.h>

#if defined(READ_ATTRIBUTES)
#  undef READ_ATTRIBUTES
#endif

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
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <sys/mman.h>
#endif

#  if !defined(SSIZE_MAX)
#    define SSIZE_MAX (1024*1024)
#  endif

#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#    include <sys/acl.h> // solaris and irix
enum {
  _COM_SUN__ACL_USER = USER,
  _COM_SUN__ACL_GROUP = GROUP
};
#    undef USER
#    undef GROUP
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
#    include <sys/acl.h> // irix
#  endif

#  undef assert
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  inline Date FileTimeToDate(const FILETIME& time)
  {
    return Date((Cast::impersonate<int64>(time) - 116444736000000000LL)/10000000);
  }
#endif // flavor

class FileHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = INVALID_HANDLE_VALUE;
#else
  int handle = -1;
#endif

  inline bool isValid() const noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return handle != INVALID_HANDLE_VALUE;
#else // unix
    return handle >= 0;
#endif
  }

  void close()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (handle == INVALID_HANDLE_VALUE) {
      return;
    }
    if (!::CloseHandle(handle)) {
      _throw FileException("Unable to close file.", this);
    }
    handle = INVALID_HANDLE_VALUE;
#else // unix
    if (handle < 0) {
      return;
    }
    if (::close(handle)) {
      _throw FileException("Unable to close file.", this);
    }
    handle = -1;
#endif // flavor
  }

  ~FileHandle()
  {
    close();
  }
};

namespace {

  inline FileHandle* toFileHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<FileHandle>().getValue();
    }
    return nullptr;
  }
}

File::File() noexcept
{
}

File::File(const String& path, Access access, unsigned int options)
{
  Profiler::ResourceCreateTask profile("File::File()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD creationFlags = 0;
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
    ToWCharString(path.getElements()), // file name
    (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)), // access mode
    (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), // share mode
    0, // security descriptor
    creationFlags, // how to create
    FILE_ATTRIBUTE_NORMAL | ((options & ASYNCHRONOUS) ? FILE_FLAG_OVERLAPPED : 0), // file attributes
    0 // handle to template file
  );
  const DWORD originalError = GetLastError();

#if 0
  unsigned int linkLevel = 0;
  const unsigned int maximumLinkLevel = 16;
  while ((handle == OperatingSystem::INVALID_HANDLE) && (++linkLevel <= maximumLinkLevel)) {    
    OperatingSystem::Handle link = ::CreateFile(ToWCharString(path.getElements()), // file name
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
    
    bool reparsePoint = false;
    String substPath = native::getTargetPath(link, reparsePoint);
    ::CloseHandle(link);
    link = INVALID_HANDLE_VALUE;
    if (!reparsePoint) {
      break;
    }
    
    handle = ::CreateFileW( // TAG: check out FILE_FLAG_POSIX_SEMANTICS
      ToWCharString(substPath), // file name
      (access == READ) ? GENERIC_READ : ((access == WRITE) ? GENERIC_WRITE : (GENERIC_READ | GENERIC_WRITE)), // access mode
      (options & EXCLUSIVE) ? 0 : (FILE_SHARE_READ | FILE_SHARE_WRITE), // share mode
      0, // security descriptor
      creationFlags, // how to create
      FILE_ATTRIBUTE_NORMAL | ((options & ASYNCHRONOUS) ? FILE_FLAG_OVERLAPPED : 0), // file attributes
      0 // handle to template file
    );
  }
#endif

  if (handle == OperatingSystem::INVALID_HANDLE) {
#if 0
    if (linkLevel > maximumLinkLevel) {
      _throw FileNotFound("Too many levels of symbolic links.", this);
    } else if (originalError == ERROR_ACCESS_DENIED) {
      _throw AccessDenied(this);
    } else {
      _throw FileNotFound("Unable to open file.", this);
    }
#endif
    if (originalError == ERROR_ACCESS_DENIED) {
      _throw AccessDenied(this);
    } else {
      _throw FileNotFound("Unable to open file.", this);
    }
  }
  
  Reference<FileHandle> _handle = new FileHandle();
  _handle->handle = handle;
#else // unix
  // TAG: exclusive file locking problem for NFS
#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
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
      _throw AccessDenied(this);
    } else {
      // WASI: ENOTCAPABLE
      _throw FileNotFound("Unable to open file.", this);
    }
  }

  Reference<FileHandle> _handle = new FileHandle();
  _handle->handle = handle;
#endif
  this->handle = _handle;
  if (options & File::APPEND) {
    setPosition(0, File::END);
  }
  profile.setHandle(*_handle, path);
}

OperatingSystem::Handle File::getHandle() const noexcept
{
  if (FileHandle* handle = toFileHandle(this->handle)) {
    return handle->handle;
  }
  return 0;
}

void File::close()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  handle->close();
}

bool File::isClosed() const noexcept
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    return true;
  }
  return !handle->isValid();
}

// TAG: need methods get owner, get group...

unsigned int File::getMode() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID ownerSID = nullptr;
  PSID groupSID = nullptr;
  PACL acl = nullptr;
  bool error = ::GetSecurityInfo(handle->handle,
                                 SE_FILE_OBJECT,
                                 OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,
                                 &ownerSID, &groupSID, &acl, 0,
                                 &securityDescriptor) != ERROR_SUCCESS;
  bassert(!error, FileException("Not a file.", this));
  
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
  trustee.ptstrName = (wchar*)ownerSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &ownerAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (wchar*)groupSID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &groupAccessMask); // ERROR_SUCCESS expected
  trustee.ptstrName = (wchar*)&EVERYONE_SID;
  ::GetEffectiveRightsFromAcl(acl, &trustee, &everyoneAccessMask); // ERROR_SUCCESS expected
  
  ACL_SIZE_INFORMATION aclInfo;
  if (::GetAclInformation(acl, &aclInfo, sizeof(aclInfo), AclSizeInformation) == 0) {
    ::LocalFree(securityDescriptor);
    _throw FileException("Unable to get ACL.", this);
  }
  
  // avoid GetEffectiveRightsFromAcl: get groups of owner and implement required functionality
  bool explicitOwner = false;
  bool explicitGroup = false;
  bool explicitEveryone = false;
  
  for (unsigned int i = 0; i < aclInfo.AceCount; ++i) {
    ACE_HEADER* ace = nullptr;
    ::GetAce(acl, i, (void**)&ace);
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
    if (ownerAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|WIN32_DELETE|WRITE_OWNER)) {
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
    if (groupAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|WIN32_DELETE|WRITE_OWNER)) {
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
    if (everyoneAccessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_ATTRIBUTES|FILE_WRITE_EA|FILE_DELETE_CHILD|WRITE_DAC|WIN32_DELETE|WRITE_OWNER)) {
      mode |= File::WOTH;
    }
    if (everyoneAccessMask & FILE_EXECUTE) {
      mode |= File::XOTH;
    }
  }
  return mode;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int error = ::fstat64(handle->handle, &status);
  #else
    struct stat status;
    int error = ::fstat(handle->handle, &status);
  #endif
  bassert(error == 0, FileException(this));

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

AccessControlList File::getACL() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  AccessControlList result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PACL acl = nullptr;
  bassert(::GetSecurityInfo(handle->handle, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
                           0, 0, &acl, 0, &securityDescriptor) == ERROR_SUCCESS,
     FileException("Unable to get ACL.", this)
  );

  SECURITY_DESCRIPTOR_CONTROL control;
  DWORD revision = 0;
	if (::GetSecurityDescriptorControl(securityDescriptor, &control, &revision) == 0) {
    ::LocalFree(securityDescriptor);
		throw FileException("Unable to get ACL.", this);
	}
  DWORD size = ::GetSecurityDescriptorLength(securityDescriptor); // TAG: remove when done
  
//   ferr << "Security descriptor:" << EOL
//        << indent(2) << "size: " << size << EOL
//        << "  revision: " << revision << EOL
//        << "  control: " << HEX << (WORD)control << EOL;

//   ULONG numberOfEntries; // TAG: what is explicit entries
//   PEXPLICIT_ACCESS entries;
//   if (::GetExplicitEntriesFromAcl(acl, &numberOfEntries, &entries) != ERROR_SUCCESS) {
//     ::LocalFree(securityDescriptor);
//     _throw FileException("Unable to get ACL.", this);
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
      _throw FileException("Unable to get ACL.", this);
  }

//   ferr << "ACL:" << EOL
//        << "  #entries: " << information.AceCount << EOL
//        << "  bytes used: " << information.AclBytesInUse << EOL
//        << "  bytes free: " << information.AclBytesFree << EOL;
  
  for (unsigned int i = 0; i < information.AceCount; ++i) {
    ACE_HEADER* ace = nullptr;
    ::GetAce(acl, i, (void**)&ace);
    
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
    if (mask & WIN32_DELETE) {
      accessMask |= AccessControlEntry::REMOVE;
    }
    if (mask & FILE_DELETE_CHILD) {
      accessMask |= AccessControlEntry::REMOVE_COMPONENT;
    }
    if (mask & WIN32_SYNCHRONIZE) {
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
  aclent_t entries[MAX_ACL_ENTRIES];
  int numberOfEntries = ::facl(handle->handle, GETACL, MAX_ACL_ENTRIES, entries);
  bassert(numberOfEntries >= 0, FileException("Unable to get ACL.", this));
  
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    bassert(::fstat64(handle->handle, &status) == 0, FileException(this));
  #else
    struct stat status;
    bassert(::fstat(handle->handle, &status) == 0, FileException(this));
  #endif
    
//   const unsigned int ownerMode = ((status.st_mode & S_IRUSR) ? AccessControlEntry::READ : 0) |
//     ((status.st_mode & S_IWUSR) ? AccessControlEntry::WRITE : 0) |
//     ((status.st_mode & S_IXUSR) ? AccessControlEntry::EXECUTE : 0);
//   result.add(AccessControlEntry(User(status.st_uid), AccessControlEntry::ALLOW, ownerMode)); // TAG: mark as owner in ACL?
  
//   const unsigned int groupMode = ((status.st_mode & S_IRGRP) ? AccessControlEntry::READ : 0) |
//     ((status.st_mode & S_IWGRP) ? AccessControlEntry::WRITE : 0) |
//     ((status.st_mode & S_IXGRP) ? AccessControlEntry::EXECUTE : 0);
//   if (groupMode != 0) {
//     result.add(AccessControlEntry(Group(status.st_gid), AccessControlEntry::ALLOW, groupMode));
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

Trustee File::getOwner() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID ownerSID = nullptr;
  bassert(::GetSecurityInfo(handle->handle, SE_FILE_OBJECT,
                           OWNER_SECURITY_INFORMATION, &ownerSID, 0, 0, 0,
                           &securityDescriptor) == ERROR_SUCCESS,
         FileException(this)
  );
  Trustee owner(Trustee::UNSPECIFIED, (const void*)ownerSID);
  ::LocalFree(securityDescriptor);
  return owner;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::fstat64(handle->handle, &status) || (!S_ISREG(status.st_mode))) {
      _throw FileException("Not a file.", this);
    }
  #else
    struct stat status;
    if (::fstat(handle->handle, &status) || (!S_ISREG(status.st_mode))) {
      _throw FileException("Not a file.", this);
    }
  #endif
  return Trustee(Trustee::USER, (const void*)(MemoryDiff)status.st_uid);
#endif // flavor
}

void File::changeOwner(const String& path, const Trustee& owner, const Trustee& group, bool followLink)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();

  //Privilege::askFor(Privilege::CHANGE_OWNER|Privilege::RESTORE|Privilege::BACKUP|Privilege::CHANGE_NOTIFY);

// if (!::InitializeSecurityDescriptor(psdFileSDrel, SECURITY_DESCRIPTOR_REVISION)) {
// }
// if (!::SetSecurityDescriptorOwner(psdFileSDrel, psidOwner, FALSE)) {
// }
// if (!::IsValidSecurityDescriptor(psdFileSDrel)) {
// }
// if (!::SetFileSecurity(path.getElements(), (SECURITY_INFORMATION)(OWNER_SECURITY_INFORMATION), psdFileSDrel)) {
// }


#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
  BASSERT(!"Not supported.");
#else // unix
  bassert((owner.getType() == Trustee::USER) && (group.getType() == Trustee::GROUP), FileException(Type::getType<File>()));
  
  uid_t uid = static_cast<uid_t>(owner.getIntegralId());
  gid_t gid = static_cast<gid_t>(group.getIntegralId());
  
  int error = 0;
  if (followLink) {
    error = ::chown(path.getElements(), uid, gid);
  } else {
    error = ::lchown(path.getElements(), uid, gid);
  }
  bassert(error == 0, FileException("Unable to change owner.", Type::getType<File>()));
#endif // flavor
}

Trustee File::getGroup() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PSECURITY_DESCRIPTOR securityDescriptor = nullptr;
  PSID groupSID = nullptr;
  bassert(::GetSecurityInfo(handle->handle, SE_FILE_OBJECT,
                           GROUP_SECURITY_INFORMATION, 0, &groupSID, 0, 0,
                           &securityDescriptor) == ERROR_SUCCESS,
         FileException(this)
  );
  Trustee group(Trustee::UNSPECIFIED, (const void*)groupSID);
  ::LocalFree(securityDescriptor);
  return group;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    if (::fstat64(handle->handle, &status) || (!S_ISREG(status.st_mode))) {
      _throw FileException("Not a file.", this);
    }
  #else
    struct stat status;
    if (::fstat(handle->handle, &status) || (!S_ISREG(status.st_mode))) {
      _throw FileException("Not a file.", this);
    }
  #endif
  return Trustee(Trustee::GROUP, (const void*)(MemoryDiff)status.st_gid);
#endif // flavor
}

long long File::getSize() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  #if (_COM_AZURE_DEV__BASE__OS >= _COM_AZURE_DEV__BASE__W2K)
    LARGE_INTEGER size; // TAG: unresolved possible byte order problem for big endian architectures
    bassert(::GetFileSizeEx(handle->handle, &size), FileException("Unable to get file size.", this));
  #else
    ULARGE_INTEGER size;
    size.LowPart = ::GetFileSize(handle->handle, &size.HighPart);
    if ((size.LowPart == INVALID_FILE_SIZE) && (::GetLastError() != NO_ERROR )) {
      _throw FileException("Unable to get file size.", this);
    }
  #endif
  return size.QuadPart;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = ::fstat64(handle->handle, &status);
  #else
    struct stat status;
    int result = ::fstat(handle->handle, &status);
  #endif // LFS
    bassert(result == 0, FileException("Unable to get file size.", this));
    return status.st_size;
#endif
}

long long File::getPosition() const
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  LARGE_INTEGER position;
  position.QuadPart = 0;
  position.LowPart = ::SetFilePointer(handle->handle, 0, &position.HighPart, FILE_CURRENT);
  if ((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    _throw FileException("Unable to get file position.", this);
  }
//  if (!::SetFilePointerEx(handle->handle, 0, &position, FILE_CURRENT)) {
//    _throw FileException("Unable to get file position.", this);
//  }
  return position.QuadPart;
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    return ::lseek64(handle->handle, 0, SEEK_CUR); // should never fail
  #else
    return ::lseek(handle->handle, 0, SEEK_CUR); // should never fail
  #endif
#endif
}

void File::setPosition(long long position, Whence whence)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static DWORD relativeTo[] = {FILE_BEGIN, FILE_CURRENT, FILE_END};
  LARGE_INTEGER temp;
  temp.QuadPart = position;
  temp.LowPart = ::SetFilePointer(handle->handle, temp.LowPart, &temp.HighPart, relativeTo[whence]);
  if ((temp.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR)) {
    _throw FileException("Unable to get file position.", this);
  }
//  if (!SetFilePointerEx(handle->handle, position, 0, relativeTo[whence])) {
//    _throw FileException("Unable to set position.", this);
//  }
#else // unix
  static int relativeTo[] = {SEEK_SET, SEEK_CUR, SEEK_END};
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    ::lseek64(handle->handle, position, relativeTo[whence]); // should never fail
  #else
    bassert(position <= PrimitiveTraits<int>::MAXIMUM, FileException("Unable to set position.", this));
    ::lseek(handle->handle, position, relativeTo[whence]); // should never fail
  #endif
#endif
}

void File::truncate(long long size)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  long long oldSize = File::getSize();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  setPosition(size);
  if (!::SetEndOfFile(handle->handle)) {
    _throw FileException("Unable to truncate.", this);
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
  #elif defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    if (::ftruncate64(handle->handle, size)) {
      _throw FileException("Unable to truncate.", this);
    }
  #else
    bassert(
      (size >= 0) && (size <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to truncate.", this)
    );
    if (::ftruncate(handle->handle, size)) {
      _throw FileException("Unable to truncate.", this);
    }
  #endif
  if (File::getSize() > oldSize) { // has file been extended
    return;
  }
#endif
  if (size > oldSize) {
    Thread::UseThreadLocalBuffer _buffer;
    Allocator<uint8>& buffer = _buffer;
    fill<uint8>(buffer.getElements(), buffer.getSize(), 0);
    setPosition(oldSize);
    long long count = size - oldSize;
    while (count > 0) {
      count -= write(
        buffer.getElements(),
        (unsigned int)minimum<long long>(count, buffer.getSize())
      ); // blocking write
    }
  }
}

void File::flush()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    return;
  }

  Profiler::IOFlushTask profile("File::flush()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::FlushFileBuffers(handle->handle)) {
    _throw FileException("Unable to flush.", this);
  }
#else // unix
  if (fsync(handle->handle)) {
    _throw FileException("Unable to flush.", this);
  }
#endif
}

void File::lock(const FileRegion& region, bool exclusive)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  Profiler::WaitTask profile("File::lock()", handle);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    (region.getOffset() >= 0) && (region.getSize() >= 0),
    FileException("Unable to lock region.", this)
  );
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    _throw FileException("Unable to lock region.", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    handle->handle,
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped
  )) {
    ::CloseHandle(overlapped.hEvent);
    _throw FileException("Unable to lock region.", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
  ::CloseHandle(overlapped.hEvent);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(handle->handle, F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        _throw FileException("Unable to lock region.", this);
      }
      break;
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    bassert(
      (region.getOffset() >= 0) &&
      (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) &&
      (region.getSize() >= 0) &&
      (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region.", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (fcntl(handle->handle, F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        _throw FileException("Unable to lock region.", this);
      }
      break;
    }
  #endif
#endif
}

bool File::tryLock(const FileRegion& region, bool exclusive)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  Profiler::WaitTask profile("File::tryLock()", handle);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to lock region.", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();
  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    _throw FileException("Unable to lock region.", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::LockFileEx(
    handle->handle,
    exclusive ? LOCKFILE_EXCLUSIVE_LOCK : 0,
    0,
    size.LowPart,
    size.HighPart,
    &overlapped)) {
    CloseHandle(overlapped.hEvent);
    _throw FileException("Unable to lock region.", this);
  }
  DWORD result = ::WaitForSingleObject(overlapped.hEvent, 0); // return immediately
  ::CloseHandle(overlapped.hEvent);
  return result == WAIT_OBJECT_0; // was the region locked
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct flock64 lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(handle->handle, F_SETLK64, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      _throw FileException("Unable to lock region.", this);
    }
    return true;
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
    return false;
  #else
    bassert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to lock region.", this)
    );
    struct flock lock;
    lock.l_type = exclusive ? F_WRLCK : F_RDLCK; // request exclusive or shared lock
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    if (::fcntl(handle->handle, F_SETLK, &lock) < 0) {
      // this function is not interrupted by a signal
      if ((errno == EACCES) || (errno == EAGAIN)) {
        return false;
      }
      _throw FileException("Unable to lock region.", this);
    }
    return true;
  #endif
#endif
}

void File::unlock(const FileRegion& region)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  Profiler::pushSignal("File::unlock()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert((region.getOffset() >= 0) && (region.getSize() >= 0), FileException("Unable to unlock region.", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  OVERLAPPED overlapped;
  overlapped.Offset = offset.LowPart;
  overlapped.OffsetHigh = offset.HighPart;
  overlapped.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
  if (overlapped.hEvent == 0) {
    _throw FileException("Unable to unlock region.", this);
  }

  LARGE_INTEGER size;
  size.QuadPart = region.getSize();
  if (!::UnlockFileEx(handle->handle, 0, size.LowPart, size.HighPart, &overlapped)) {
    ::CloseHandle(overlapped.hEvent);
    _throw FileException("Unable to unlock region.", this);
  }
  ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for unlock
  ::CloseHandle(overlapped.hEvent);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    bassert(
      (region.getOffset() >= 0) && (region.getSize() >= 0),
      FileException("Unable to unlock region.", this)
    );
    struct flock64 lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(handle->handle, F_SETLKW64, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        _throw FileException("Unable to unlock region.", this);
      }
      break;
    }
  #else
    bassert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM) && (region.getSize() >= 0) && (region.getSize() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to unlock region.", this)
    );
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET; // offset from beginning of file
    lock.l_start = region.getOffset();
    lock.l_len = region.getSize();

    while (true) {
      if (::fcntl(handle->handle, F_SETLKW, &lock) < 0) {
        if (errno == EINTR) { // interrupted by signal - try again
          continue;
        }
        _throw FileException("Unable to unlock region.", this);
      }
      break;
    }
  #endif
#endif
}

Date File::getLastModification()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(handle->handle, 0, 0, &time)) {
    _throw FileException("Unable to get file time.", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = ::fstat64(handle->handle, &status);
  #else
    struct stat status;
    int result = ::fstat(handle->handle, &status);
  #endif // LFS
    bassert(result == 0, FileException("Unable to get status.", this));
    return Date(status.st_mtime);
#endif
}

Date File::getLastAccess()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(handle->handle, 0, &time, 0)) {
    _throw FileException("Unable to get file time.", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = fstat64(handle->handle, &status);
  #else
    struct stat status;
    int result = ::fstat(handle->handle, &status);
  #endif // LFS
    bassert(result == 0, FileException("Unable to get status.", this));
    return Date(status.st_atime);
#endif
}

Date File::getLastChange()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  FILETIME time;
  if (::GetFileTime(handle->handle, &time, 0, 0)) {
    _throw FileException("Unable to get file time.", this);
  }
  return FileTimeToDate(time);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    struct stat64 status;
    int result = fstat64(handle->handle, &status);
  #else
    struct stat status;
    int result = ::fstat(handle->handle, &status);
  #endif // LFS
    bassert(result == 0, FileException("Unable to get status.", this));
    return Date(status.st_ctime);
#endif
}

unsigned long File::getVariable(Variable variable)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

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
    long result = ::fpathconf(handle->handle, fileSystemVariable);
    if (!((result == -1) && (errno != 0))) {
      return result;
    }
  }
  if ((errno == EBADF) || (errno == EINVAL)) {
    _throw FileException("Unable to get variable.", this);
  }
  // TAG: add POSIX values here?
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif // flavor
  return 0;
}

unsigned int File::read(
  uint8* buffer,
  unsigned int bytesToRead,
  bool nonblocking)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::IOReadTask profile("File::read()", buffer);

  unsigned int bytesRead = 0;
  while (bytesToRead > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::ReadFile(handle->handle, buffer, bytesToRead, &result, 0);
    if (!success) { // has error occured
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking read
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(handle->handle, 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(handle->handle, 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(handle->handle, 0, 0, bytesToRead, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::ReadFile(handle->handle, buffer, bytesToRead, &result, 0)) {
                  if (::UnlockFileEx(handle->handle, 0, bytesToRead, 0, &overlapped)) { // // release shared lock
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
      _throw FileException("Unable to read from file.", this);
    }
#else // unix
    int result = 0;
    do {
      result = (int)::read(handle->handle, buffer, minimum<size_t>(bytesToRead, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) { // remember that errno is local to the thread
        case EINTR: // interrupted by signal before any data was read
          break; // try again
        case EAGAIN: // no data available (only in non-blocking mode)
          result = 0;
          break;
        default:
          _throw FileException("Unable to read from file.", this);
        }
      }
    } while (result < 0);
#endif
    profile.onBytesRead(result);
    bytesRead += result;
    buffer += result;
    bytesToRead -= result;
    if (nonblocking) { // accept whatever has been read in nonblocking mode
      break;
    }
    if (result == 0) { // has end been reached
      _throw EndOfFile(this); // attempt to read beyond end of stream
    }
  }
  return bytesRead;
}

unsigned int File::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  bool nonblocking)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  Profiler::IOWriteTask profile("File::write()", buffer);

  unsigned int bytesWritten = 0;
  while (bytesToWrite > 0) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DWORD result = 0;
    BOOL success = ::WriteFile(handle->handle, buffer, bytesToWrite, &result, 0);
    if (!success) {
      if (::GetLastError() == ERROR_LOCK_VIOLATION) { // TAG: I'm guessing this error code - please confirm
        if (nonblocking) {
          success = true;
          result = 0;
        } else { // do blocking write
          LARGE_INTEGER position;
          position.QuadPart = 0;
          position.LowPart = ::SetFilePointer(handle->handle, 0, &position.HighPart, FILE_CURRENT);
          if (!((position.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))) {
	    // if (SetFilePointerEx(handle->handle, 0, &position, FILE_CURRENT)) { // current position
            OVERLAPPED overlapped;
            overlapped.Offset = position.LowPart;
            overlapped.OffsetHigh = position.HighPart;
            overlapped.hEvent = ::CreateEvent(0, FALSE, FALSE, 0); // auto reset event

            if (overlapped.hEvent) { // was event created
              if (::LockFileEx(handle->handle, 0, 0, bytesToWrite, 0, &overlapped)) { // acquire shared lock
                ::WaitForSingleObject(overlapped.hEvent, INFINITE); // blocking wait for lock
                if (::WriteFile(handle->handle, buffer, bytesToWrite, &result, 0)) {
                  if (::UnlockFileEx(handle->handle, 0, bytesToWrite, 0, &overlapped)) { // // release shared lock
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
      _throw FileException("Unable to write to file.", this);
    }
#else // unix
    int result = 0;
    do {
      result = (int)::write(handle->handle, buffer, minimum<size_t>(bytesToWrite, SSIZE_MAX));
      if (result < 0) { // has an error occured
        switch (errno) {
        case EINTR: // interrupted by signal before any data was written
          break; // try again
        case EAGAIN: // no data could be written without blocking (only in non-blocking mode)
          result = 0;
          break;
        default:
          _throw FileException("Unable to write to file.", this);
        }
      }
    } while (result < 0);
#endif
    if (result == 0) {
      if (nonblocking) {
        break;
      } else {
        _throw FileException("Unable to write to file.", this);
      }
    }
    profile.onBytesWritten(result);
    bytesWritten += result;
    buffer += result;
    bytesToWrite -= result;
  }
  return bytesWritten;
}

void File::asyncCancel()
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::CancelIo(handle->handle);
#else // unix
  // TAG: fixme
#endif // flavor
}

AsynchronousReadOperation File::read(
  uint8* buffer,
  unsigned int bytesToRead,
  unsigned long long offset,
  AsynchronousReadEventListener* listener)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncReadFileContext(getHandle(), buffer, bytesToRead, offset, listener);
#else // unix
  return AsynchronousReadOperation(); // TAG: fixme
#endif // flavor
}

AsynchronousWriteOperation File::write(
  const uint8* buffer,
  unsigned int bytesToWrite,
  unsigned long long offset,
  AsynchronousWriteEventListener* listener)
{
  FileHandle* handle = toFileHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(listener, AsynchronousException(this)); // TAG: fixme
  return new win32::AsyncWriteFileContext(getHandle(), buffer, bytesToWrite, offset, listener);
#else // unix
  return AsynchronousWriteOperation(); // TAG: fixme
#endif // flavor
}

File::~File()
{
}

bool File::createFile(const String& path, const String& text)
{
  return createFile(path, text.getBytes(), text.getLength());
}

bool File::createFile(const String& path, const uint8* buffer, MemorySize size)
{
  try {
    File file(path, File::WRITE, File::CREATE);
    file.write(buffer, size);
    return true;
  } catch (...) {
  }
  return false;
}

#if 0
// could use a separate empty string to differentiate empty from invalid
const String& getInvalidString()
{
  static String s(Default());
  return s;
}

bool isInvalidString(const String& string)
{
  const String& invalidString = getInvalidString();
  if (invalidString.getContainer() == string.getContainer()) {
    return true;
  }
  return false;
}
#endif

Validified<String> File::readFile(const String& path, Encoding encoding)
{
  String result;
  try {
    File file(path, File::READ, 0);
    result.forceToLength(file.getSize());
    uint8* buffer = reinterpret_cast<uint8*>(result.getElements());
    unsigned int bytesRead = file.read(buffer, result.getLength());
    BASSERT(bytesRead == result.getLength());
  } catch (...) {
    return UNSPECIFIED;
  }
  
  switch (encoding) {
  case ENCODING_RAW:
    break;
  case ENCODING_ASCII:
    for (uint8 ch :  result) {
      if (ch >= 0x80) {
        return UNSPECIFIED;
      }
    }
    break;
  case ENCODING_UTF8:
    {
      // TAG: need to check for Unicode only
      MemorySize size = result.getLength();
      MemoryDiff length = Unicode::getUTF8StringLength(result.getBytes(), result.getBytes() + size);
      if (length < 0) {
        return UNSPECIFIED;
      }
    }
    break;
  case ENCODING_UTF8_ISO:
    {
      // TAG: need to check for ISO
      MemorySize size = result.getLength();
      MemoryDiff length = Unicode::getUTF8StringLength(result.getBytes(), result.getBytes() + size);
      if (length < 0) {
        return UNSPECIFIED;
      }
    }
    break;
  }
  
  return result;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(File) : public UnitTest {
public:

  TEST_PRIORITY(500);
  TEST_PROJECT("base/io");
  TEST_IMPACT(NORMAL);
  TEST_EXTERNAL();

  void run() override
  {
    const Path testFolder = makeFolder();

    File f1(testFolder / "test.txt", File::WRITE, File::CREATE);
    const char* text = "Hello, World!\n";
    f1.write(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
    f1.close();

#if 0 // TAG: fix end of file handling
    File f2(testFolder / "test.txt", File::READ, 0);
    uint8 buffer[128];
    unsigned int bytesRead = f2.read(buffer, getArraySize(buffer));
    f2.close();
    TEST_ASSERT(bytesRead == getNullTerminatedLength(text));
#endif
  }
};

TEST_REGISTER(File);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
