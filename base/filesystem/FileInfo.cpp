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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #define _WIN32_WINNT 0x0400
  #include <windows.h>
  #include <aclapi.h>
  #include <winioctl.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileInfo::FileInfo(const String& _path) throw(FileSystemException) : path(_path), mode(0) {
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
  group = Group((const void*)groupSID);
  ::LocalFree(securityDescriptor);

  ULARGE_INTEGER temp;
  temp.LowPart = information.nFileSizeLow;
  temp.HighPart = information.nFileSizeHigh;
  size = temp.QuadPart;
  const long long fileTimeOffset = 116444736000000000ULL;
  access = (*pointer_cast<const long long*>(&information.ftLastAccessTime) - fileTimeOffset)/10000000;
  modification = (*pointer_cast<const long long*>(&information.ftLastWriteTime) - fileTimeOffset)/10000000;
  change = (*pointer_cast<const long long*>(&information.ftCreationTime) - fileTimeOffset)/10000000;
  links = information.nNumberOfLinks;
  
// #if 0 // disabled
//   SECURITY_DESCRIPTOR_CONTROL* control;
//   DWORD revision;
// 	if (::GetSecurityDescriptorControl(securityDescriptor, &control, &revision)) {
// 		throw FileSystemException("Unable to get secirity descriptor control", this);
// 	}
//   ::GetSecurityDescriptorLength(securityDescriptor);
  
//   fout << '{' << length << ' ' << revision << HEX << (unsigned int)control << '}';
  
//   ULONG numberOfEntries;
//   PEXPLICIT_ACCESS entries;
//   error |= ::GetExplicitEntriesFromAcl(acl, &numberOfEntries, &entries) != ERROR_SUCCESS;
//   if (error) {
//     ::LocalFree(securityDescriptor);
//     throw FileSystemException("Unable to get ACL", this);
//   }

// //   ::GetSecurityDescriptorOwner();
// //   ::GetSecurityDescriptorGroup();

// 	if (::GetSecurityDescriptorDacl( psd, &daclPresent, &dacl, &daclDefaulted) == 0) {
//     throw FileSystemException("Unable to get DACL", this);
// 	}

//   ACL_SIZE_INFORMATION information;
//   if (::GetAclInformation(acl, &information, sizeof(information), AclSizeInformation) == 0) {
//     throw FileSystemException("Unable to get ACL information", this);
// 	}

//   fout << '{' << aci.AceCount << ' ' << aci.AclBytesInUse << ' ' << aci.AclBytesFree << '}';
  
// 	for (unsigned int i = 0; i < aci.AceCount; ++i) {
//     ACE_HEADER* ace;
//     ::GetAce(acl, i, (void**)&ace);
//     fout << "ace{entry=" << i " acetype=" << ace->AceType << ' ' << '}';
    

// 	printf( "%sACE type: %s (%lu)\n",type, (DWORD) ace->AceType );

// //	  User(psid).getName();

//   switch (ace->AceType) {
//   case ACCESS_ALLOWED_ACE_TYPE:
//     type = "ACCESS_ALLOWED_ACE_TYPE";
//     psid = &((ACCESS_ALLOWED_ACE*)ace)->SidStart;
//     break;
//   case ACCESS_DENIED_ACE_TYPE:
//     type = "ACCESS_DENIED_ACE_TYPE";
//     psid = &((ACCESS_DENIED_ACE*)ace)->SidStart;
//     break;
//   default:
//     Trace::message("default");
//   }
// #endif // disable


  
// 		case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
// 			type = "ACCESS_ALLOWED_OBJECT_ACE_TYPE";
// 			psid = &( (ACCESS_ALLOWED_OBJECT_ACE *) ace )->SidStart;
// 			break;
// 		case ACCESS_DENIED_OBJECT_ACE_TYPE:
// 			type = "ACCESS_DENIED_OBJECT_ACE_TYPE";
// 			psid = &( (ACCESS_DENIED_OBJECT_ACE *) ace )->SidStart;
// 			break;
//       }
  
//   PEXPLICIT_ACCESS entry = entries;
//   for (int i = 0; i < numberOfEntries; ++i) {
//     fout << "{" << i << ' ' << "permissions:" << HEX << entry->grfAccessPermissions << ' '
//          << "access mode:" << entry->grfAccessMode << ' '
//          << "inherit:" << HEX << entry->grfInheritance << ' '
//          << "form:" << entry->Trustee.TrusteeForm << ' '
//          << "type:" << entry->Trustee.TrusteeType << '}';
//     ++entry;
//   }
//   ::LocalFree(entry);
  
  ACCESS_MASK accessMask = 0;
  TRUSTEE trustee;
  trustee.pMultipleTrustee = 0;
  trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
  trustee.TrusteeForm = TRUSTEE_IS_SID;
  trustee.TrusteeType = TRUSTEE_IS_UNKNOWN;
  
  // trustee.ptstrName = (char*)ownerSID;
  ::BuildTrusteeWithSid(&trustee, ownerSID);
  ::GetEffectiveRightsFromAcl(acl, &trustee, &accessMask); // ERROR_SUCCESS
  if (accessMask & FILE_READ_DATA == FILE_READ_DATA) { // mode reset above
    mode |= FileInfo::RUSR;
  }
  if (accessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA) == (FILE_WRITE_DATA|FILE_APPEND_DATA)) {
    mode |= FileInfo::WUSR;
  }
  if (accessMask & FILE_EXECUTE == FILE_EXECUTE) {
    mode |= FileInfo::XUSR;
  }
  
//  trustee.ptstrName = (char*)groupSID;
  ::BuildTrusteeWithSid(&trustee, groupSID);
  ::GetEffectiveRightsFromAcl(acl, &trustee, &accessMask); // ERROR_SUCCESS
  if (accessMask & FILE_READ_DATA == FILE_READ_DATA) {
    mode |= FileInfo::RGRP;
  }
  if (accessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA) == (FILE_WRITE_DATA|FILE_APPEND_DATA)) {
    mode |= FileInfo::WGRP;
  }
  if (accessMask & FILE_EXECUTE == FILE_EXECUTE) {
    mode |= FileInfo::XGRP;
  }

  static const char EVERYONE_SID[1 + 1 + 6 + 4] = {
    1, // revision
    1, // number of sub authorities
    0, 0, 0, 0, 0, 1, // SECURITY_WORLD_SID_AUTHORITY
    0, 0, 0, 0 // SECURITY_WORLD_RID
  };
  
//   //trustee.ptstrName = (char*)&EVERYONE_SID;
  ::BuildTrusteeWithSid(&trustee, (void*)&EVERYONE_SID);
  ::GetEffectiveRightsFromAcl(acl, &trustee, &accessMask); // ERROR_SUCCESS
  if (accessMask & FILE_READ_DATA == FILE_READ_DATA) {
    mode |= FileInfo::ROTH;
  }
  if (accessMask & (FILE_WRITE_DATA|FILE_APPEND_DATA) == (FILE_WRITE_DATA|FILE_APPEND_DATA)) {
    mode |= FileInfo::WOTH;
  }
  if (accessMask & FILE_EXECUTE == FILE_EXECUTE) {
    mode |= FileInfo::XOTH;
  }

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
    
  if (status.st_mode & S_IRUSR) { // mode reset above
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
  
  size = status.st_size;
  owner = User((const void*)(ptrdiff_t)status.st_uid);
  group = Group((const void*)(ptrdiff_t)status.st_gid);
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
