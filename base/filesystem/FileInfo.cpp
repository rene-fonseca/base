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

FileInfo::FileInfo(const String& _path) throw(FileSystemException) : path(_path) {
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
  error |= ::GetSecurityInfo(file,
                             SE_FILE_OBJECT,
                             OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,
                             &ownerSID,
                             &groupSID,
                             0,
                             0,
                             &securityDescriptor) != ERROR_SUCCESS;
  ::CloseHandle(file);
  if (!error) {
    owner = User(ownerSID);
    group = Group(groupSID);
    ::LocalFree(securityDescriptor);
  }
  
  static const long long fileTimeOffset = 116444736000000000ULL;
  ULARGE_INTEGER temp;
  temp.LowPart = information.nFileSizeLow;
  temp.HighPart = information.nFileSizeHigh;
  size = temp.QuadPart;
  mode = PrimitiveTraits<unsigned int>::MAXIMUM;
  access = (*pointer_cast<const long long*>(&information.ftLastAccessTime) - fileTimeOffset)/10000000;
  modification = (*pointer_cast<const long long*>(&information.ftLastWriteTime) - fileTimeOffset)/10000000;
  change = (*pointer_cast<const long long*>(&information.ftCreationTime) - fileTimeOffset)/10000000;
  links = information.nNumberOfLinks;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    struct stat64 buffer;
    if (stat64(path.getElements(), &buffer) || (!S_ISREG(buffer.st_mode))) {
      throw FileSystemException("Not a file", this);
    }
  #else
    struct stat buffer;
    if (stat(path.getElements(), &buffer) || (!S_ISREG(buffer.st_mode))) {
      throw FileSystemException("Not a file", this);
    }
  #endif
  size = buffer.st_size;
  mode = buffer.st_mode;
  owner = User((const void*)buffer.st_uid);
  group = Group((const void*)buffer.st_gid);
  access = buffer.st_atime;
  modification = buffer.st_mtime;
  change = buffer.st_ctime;
  links = buffer.st_nlink;
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
