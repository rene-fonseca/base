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
#include <base/io/MappedFile.h>
#include <base/io/FileException.h>
#include <base/Base.h>
#include <base/Functor.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
#  define _LARGEFILE64_SOURCE 1
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
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
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <sys/mman.h>
#endif

#  undef assert
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MappedFile::MappedFileImpl::MappedFileImpl(const File& _file, const FileRegion& _region, bool _writeable)
  : file(_file), region(_region), writeable(_writeable)
{
  bassert(region.getOffset() >= 0, FileException("Unable to map file region.", this));
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = ::CreateFileMapping((HANDLE)getHandle(file), 0, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, 0);
  bassert(handle, FileException("Unable to map file region.", this));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  void* address = ::MapViewOfFile(
    handle,
    writeable ? FILE_MAP_WRITE : FILE_MAP_READ,
    offset.HighPart,
    offset.LowPart,
    region.getSize()
  );

  ::CloseHandle(handle); // this should not fail
#else // unix
  void* address = nullptr;
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    address = ::mmap64(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    bassert(
      (region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM),
      FileException("Unable to map file region.", this)
    );
    address = ::mmap(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #endif
  bassert(address != (void*)-1, FileException("Unable to map file region.", this));
#endif // flavor
  bytes = address;
}

void MappedFile::MappedFileImpl::synchronize() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::FlushViewOfFile(bytes, 0)) {
    _throw FileException("Unable to flush.", this);
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    if (::msync((caddr_t)bytes, region.getSize(), MS_SYNC)) {
      _throw FileException("Unable to flush.", this);
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    if (::msync(bytes, region.getSize(), MS_SYNC)) {
      _throw FileException("Unable to flush.", this);
    }
  #endif
#endif // flavor
}

MappedFile::MappedFileImpl::~MappedFileImpl() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::UnmapViewOfFile(bytes)) {
    _throw FileException("Unable to unmap file.", this);
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    if (::munmap((caddr_t)bytes, region.getSize())) {
      _throw FileException("Unable to unmap file.", this);
    }
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    if (::munmap(bytes, region.getSize())) {
      _throw FileException("Unable to unmap file.", this);
    }
  #endif
#endif // flavor
}

unsigned int MappedFile::getGranularity() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  SYSTEM_INFO info;
  ::GetSystemInfo(&info);
  return info.dwAllocationGranularity;
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    return 4096 * 2; // TAG: fixme
  #else
    long size = ::sysconf(_SC_PAGE_SIZE);
    BASSERT((size > 0) && (size <= PrimitiveTraits<unsigned int>::MAXIMUM));
    return static_cast<unsigned int>(size);
  #endif
#endif // flavor
}

MappedFile::MappedFile(const File& file, const FileRegion& region, bool writeable)
  : map(0)
{
  map = new MappedFileImpl(file, region, writeable);
//  bassert(r.getOffset() >= 0, FileException("Unable to map file.", this));
//#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//  HANDLE handle = ::CreateFileMapping((HANDLE)f.fd->getHandle(), 0, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, 0);
//  if (handle) {
//    _throw FileException("Unable to map file.", this);
//  }
//
//  void* address = ::MapViewOfFile(
//    handle,
//    writeable ? FILE_MAP_WRITE : FILE_MAP_READ,
//    (Cast::pointer<LARGE_INTEGER*>(&offset))->HighPart,
//    (Cast::pointer<LARGE_INTEGER*>(&offset))->LowPart,
//    size
//  );
//
//  CloseHandle(handle); // this should not fail
//
//  map = new MappedFileImpl(f, address, size);
//#else
//  void* address = nullptr;
//#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
//  address = mmap64(0, r.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, f.fd->getHandle(), r.getOffset());
//#else
//  bassert((r.getOffset() >= 0) && (r.getOffset() <= PrimitiveTraits<int>::MAXIMUM), FileException("Unable to map file.", this));
//  address = mmap(0, r.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, f.fd->getHandle(), r.getOffset());
//#endif
//  if (address == (void*)-1) {
//    _throw FileException("Unable to map file.", this);
//  }
//  map = new MappedFileImpl(f, address, r);
//#endif // flavor
}

MappedFile& MappedFile::operator=(const MappedFile& assign) noexcept
{
  if (&assign == this) {
    map = assign.map;
  }
  return *this;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
