/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/io/MappedFile.h>
#include <base/Base.h>
#include <base/Functor.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
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
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MappedFile::MappedFileImpl::MappedFileImpl(const File& f, const FileRegion& r, bool w) throw(FileException) :
  file(f), region(r), writeable(w) {
  assert(region.getOffset() >= 0, FileException("Unable to map file region"));
#if defined(__win32__)
  HANDLE handle = CreateFileMapping((HANDLE)getHandle(file), NULL, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, NULL);
  assert(handle != NULL, FileException("Unable to map file region"));
  LARGE_INTEGER offset;
  offset.QuadPart = region.getOffset();

  void* address = MapViewOfFile(
    handle,
    writeable ? FILE_MAP_WRITE : FILE_MAP_READ,
    offset.HighPart,
    offset.LowPart,
    region.getSize()
  );

  CloseHandle(handle); // this should not fail
#else
  void* address;
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    address = mmap64(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #else
    assert((region.getOffset() >= 0) && (region.getOffset() <= INT_MAX), FileException("Unable to map file region"));
    address = mmap(0, region.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, getHandle(file), region.getOffset());
  #endif
  assert(address != (void*)-1, FileException("Unable to map file region"));
#endif
  bytes = address;
}

void MappedFile::MappedFileImpl::flush() const throw(FileException) {
#if defined(__win32__)
  if (!FlushViewOfFile(bytes, 0)) {
    throw FileException("Unable to flush");
  }
#else // __unix__
  if (msync(bytes, region.getSize(), MS_SYNC)) {
    throw FileException("Unable to flush");
  }
#endif
}

MappedFile::MappedFileImpl::~MappedFileImpl() throw(FileException) {
#if defined(__win32__)
  if (!UnmapViewOfFile(bytes)) {
    throw FileException("Unable to unmap file");
  }
#else // __unix__
  if (munmap(bytes, region.getSize())) {
    throw FileException("Unable to unmap file");
  }
#endif
}

unsigned int MappedFile::getGranularity() throw() {
#if defined(__win32__)
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwAllocationGranularity;
#else // __unix__
  ASSERT(sysconf(_SC_PAGE_SIZE) <= UINT_MAX); // this should never happen
  return sysconf(_SC_PAGE_SIZE);
#endif
}

MappedFile::MappedFile(const File& file, const FileRegion& region, bool writeable) throw(FileException) : map(0) {
  map = new MappedFileImpl(file, region, writeable);
//  assert(r.getOffset() >= 0, FileException("Unable to map file"));
//#if defined(__win32__)
//  HANDLE handle = CreateFileMapping((HANDLE)f.fd->getHandle(), NULL, writeable ? PAGE_READWRITE : PAGE_READONLY, 0, 0, NULL);
//  if (handle == NULL) {
//    throw FileException("Unable to map file");
//  }
//
//  void* address = MapViewOfFile(
//    handle,
//    writeable ? FILE_MAP_WRITE : FILE_MAP_READ,
//    ((LARGE_INTEGER*)&offset)->HighPart,
//    ((LARGE_INTEGER*)&offset)->LowPart,
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
//  assert((r.getOffset() >= 0) && (r.getOffset() <= INT_MAX), FileException("Unable to map file"));
//  address = mmap(0, r.getSize(), writeable ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, f.fd->getHandle(), r.getOffset());
//#endif
//  if (address == (void*)-1) {
//    throw FileException("Unable to map file");
//  }
//  map = new MappedFileImpl(f, address, r);
//#endif
}

MappedFile& MappedFile::operator=(const MappedFile& eq) throw() {
  if (&eq == this) {
    map = eq.map;
  }
  return *this;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
