/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/MappedFile.h>
#include <base/Base.h>
#include <base/Functor.h>

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #define _LARGEFILE64_SOURCE 1
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <limits.h>
  #include <string.h> // required by FD_SET on solaris
  #include <sys/mman.h>

  #undef assert
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MappedFile::MappedFileImpl::MappedFileImpl(const File& _file, const FileRegion& _region, bool _writeable) throw(FileException) :
  file(_file), region(_region), writeable(_writeable) {
  assert(region.getOffset() >= 0, FileException("Unable to map file region", this));
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE handle = ::CreateFileMapping((HANDLE)getHandle(file), 0, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, 0);
  assert(handle, FileException("Unable to map file region", this));
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
  void* address;
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    address = mmap64(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #else
    assert((region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM), FileException("Unable to map file region", this));
    address = mmap(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #endif
  assert(address != (void*)-1, FileException("Unable to map file region", this));
#endif // flavor
  bytes = address;
}

void MappedFile::MappedFileImpl::flush() const throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::FlushViewOfFile(bytes, 0)) {
    throw FileException("Unable to flush", this);
  }
#else // unix
  if (msync(bytes, region.getSize(), MS_SYNC)) {
    throw FileException("Unable to flush", this);
  }
#endif // flavor
}

MappedFile::MappedFileImpl::~MappedFileImpl() throw(FileException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::UnmapViewOfFile(bytes)) {
    throw FileException("Unable to unmap file", this);
  }
#else // unix
  if (munmap(bytes, region.getSize())) {
    throw FileException("Unable to unmap file", this);
  }
#endif // flavor
}

unsigned int MappedFile::getGranularity() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEM_INFO info;
  ::GetSystemInfo(&info);
  return info.dwAllocationGranularity;
#else // unix
  ASSERT(sysconf(_SC_PAGE_SIZE) <= PrimitiveTraits<unsigned int>::MAXIMUM); // this should never happen
  return sysconf(_SC_PAGE_SIZE);
#endif // flavor
}

MappedFile::MappedFile(const File& file, const FileRegion& region, bool writeable) throw(FileException) : map(0) {
  map = new MappedFileImpl(file, region, writeable);
//  assert(r.getOffset() >= 0, FileException("Unable to map file", this));
//#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
//  HANDLE handle = ::CreateFileMapping((HANDLE)f.fd->getHandle(), 0, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, 0);
//  if (handle) {
//    throw FileException("Unable to map file", this);
//  }
//
//  void* address = ::MapViewOfFile(
//    handle,
//    writeable ? FILE_MAP_WRITE : FILE_MAP_READ,
//    (pointer_cast<LARGE_INTEGER*>(&offset))->HighPart,
//    (pointer_cast<LARGE_INTEGER*>(&offset))->LowPart,
//    size
//  );
//
//  CloseHandle(handle); // this should not fail
//
//  map = new MappedFileImpl(f, address, size);
//#else
//  void* address;
//#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
//  address = mmap64(0, r.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, f.fd->getHandle(), r.getOffset());
//#else
//  assert((r.getOffset() >= 0) && (r.getOffset() <= PrimitiveTraits<int>::MAXIMUM), FileException("Unable to map file", this));
//  address = mmap(0, r.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, f.fd->getHandle(), r.getOffset());
//#endif
//  if (address == (void*)-1) {
//    throw FileException("Unable to map file", this);
//  }
//  map = new MappedFileImpl(f, address, r);
//#endif // flavor
}

MappedFile& MappedFile::operator=(const MappedFile& eq) throw() {
  if (&eq == this) {
    map = eq.map;
  }
  return *this;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
