/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/filesystem/FileSystemException.h>
#include <base/platforms/features.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32)
#  error native reparse interface requires win32 flavor
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>

enum {
  WIN32_DELETE = DELETE,
  WIN32_READ_CONTROL = READ_CONTROL,
  WIN32_WRITE_DAC = WRITE_DAC,
  WIN32_WRITE_OWNER = WRITE_OWNER,
  WIN32_SYNCHRONIZE = SYNCHRONIZE
};

#undef FILE_ALL_ACCESS
#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | WIN32_SYNCHRONIZE | 0x1FF)

// yikes
#undef DELETE
#undef SYNCHRONIZE

#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace native {

  // #  include <ntifs.h>
  // https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-fscc/b41f1cbf-10df-4a47-98d4-1c52a833d913
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
#  define IO_REPARSE_TAG_SYMBOLIC_LINK 0xa000000c
#endif

  /** Returns the target path for the reparse buffer. */
  inline String getTargetPath(native::REPARSE_DATA_BUFFER* reparseHeader, bool& supportedReparsePoint)
  {
    supportedReparsePoint = false;
    if (!reparseHeader) {
      return String();
    }
    wchar* substPath = nullptr;
    unsigned int substLength = 0;
    switch (reparseHeader->ReparseTag) {
    case IO_REPARSE_TAG_SYMBOLIC_LINK:
      supportedReparsePoint = true;
      substPath = reparseHeader->SymbolicLinkReparseBuffer.PathBuffer + reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(wchar);
      BASSERT(reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0);
      substLength = reparseHeader->SymbolicLinkReparseBuffer.SubstituteNameLength / sizeof(wchar);
      break;
    case IO_REPARSE_TAG_MOUNT_POINT:
      supportedReparsePoint = true;
      substPath = reparseHeader->MountPointReparseBuffer.PathBuffer + reparseHeader->MountPointReparseBuffer.SubstituteNameOffset / sizeof(wchar);
      BASSERT(reparseHeader->MountPointReparseBuffer.SubstituteNameLength % sizeof(wchar) == 0);
      substLength = reparseHeader->MountPointReparseBuffer.SubstituteNameLength / sizeof(wchar); // keep prefix "\??\"
      break;
    default:
      // _throw FileSystemException("Unsupported link.");
      return String();
    }
    if (substLength >= 4) {
      if ((substPath[0] == '\\') && (substPath[1] == '?') && (substPath[2] == '?') && (substPath[3] == '\\')) {
        substPath[1] = '\\'; // convert '\??\' to '\\?\'
      }
    }
    substPath[substLength] = 0; // add terminator
    return String(substPath, substLength);
  }

  /** Returns the target path for the reparse point handle. */
  inline String getTargetPath(HANDLE handle, bool& reparsePoint)
  {
    PrimitiveArray<uint8> buffer(1024); // need alternative - first attempt to get length first failed
    while (buffer.size() < (64 * 1024)) {
      native::REPARSE_DATA_BUFFER* reparseHeader = reinterpret_cast<native::REPARSE_DATA_BUFFER*>(static_cast<uint8*>(buffer));
      DWORD bytesWritten = 0;
      const bool status = ::DeviceIoControl(
        handle,
        FSCTL_GET_REPARSE_POINT,
        0,
        0,
        reparseHeader,
        buffer.size(),
        &bytesWritten,
        0
      );
      if (status == 0) {
        DWORD error = ::GetLastError();
        if (error == ERROR_INSUFFICIENT_BUFFER) {
          buffer.resize(buffer.size() * 2);
          continue;
        }
        const bool reparse = error != 4390; // ERROR_NOT_A_REPARSE_POINT
        if (!reparse) { // no need to check for shell link
          reparsePoint = false;
          return String();
        }
      }
      bool supportedReparsePoint = false;
      String path = getTargetPath(reparseHeader, supportedReparsePoint);
      if (!supportedReparsePoint) {
        reparsePoint = false;
        return String();
      }
      reparsePoint = true;
      return path;
    }
    reparsePoint = false;
    return String();
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
