/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/SharedMemory.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#  if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
#    define _LARGEFILE64_SOURCE 1
#  endif
#  include <sys/mman.h>
#  include <unistd.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SharedMemory::SharedMemoryImpl::SharedMemoryImpl(const File& _file, const FileRegion& _region, unsigned int _access) throw(MemoryException)
  : file(_file), region(_region), access(_access) {

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD protection = (access & SharedMemory::WRITE) ? PAGE_READWRITE : PAGE_READONLY;
  handle = ::CreateFileMapping(SharedMemory::getHandle(file),
                               0, // security
                               protection, // protection
                               0, // high word of size
                               region.getSize(), // low word of size
                               0 // name of mapping
  );
  assert(handle != OperatingSystem::INVALID_HANDLE, MemoryException("Unable to open shared memory", this));

  address = (char*)::MapViewOfFile(handle, // handle to file-mapping object
                                   (access & SharedMemory::WRITE) ? FILE_MAP_WRITE : FILE_MAP_READ, // access mode
                                   getHighWordOf64(region.getOffset()), // high word of offset
                                   getLowWordOf64(region.getOffset()), // low word of offset
                                   region.getSize() // number of bytes to map
  );
  if (access == 0) {
    ::CloseHandle(handle);
    throw MemoryException("Unable to open shared memory", this);
  }
#else // unix
  handle = OperatingSystem::INVALID_HANDLE;
  int protection;
  if (access == 0) {
    protection = PROT_NONE;
  } else {
    protection = 0;
    if (access & SharedMemory::READ) {
      protection |= PROT_READ;
    }
    if (access & SharedMemory::WRITE) {
      protection |= PROT_WRITE;
    }
#if defined(PROT_EXEC)
    if (access & SharedMemory::EXECUTE) {
      protection |= PROT_EXEC;
    }
#endif
  }
  
  #if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
    address = (char*)::mmap64(0, region.getSize(), protection, MAP_SHARED, getHandle(file), region.getOffset());
  #else
    assert((region.getOffset() >= 0) && (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM), MemoryException("Unable to open shared memory", this));
    address = (char*)::mmap(0, region.getSize(), protection, MAP_SHARED, getHandle(file), region.getOffset());
  #endif
  assert(address !=  MAP_FAILED, MemoryException("Unable to open shared memory", this));
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::lock() throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::VirtualLock(address, region.getSize()), MemoryException("Unable to lock process memory", this));
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    throw NotSupported(this);
  #else
    assert(::mlock(address, region.getSize()) == 0, MemoryException("Unable to lock memory", this));
  #endif
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::unlock() throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(::VirtualUnlock(address, region.getSize()), MemoryException("Unable to unlock process memory", this));
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    throw NotSupported(this);
  #else  
    assert(::munlock(address, region.getSize()) == 0, MemoryException("Unable to unlock process memory", this));
  #endif
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::setProtection(unsigned int access) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD protection = 0;
  if (access == 0) {
    protection = PAGE_NOACCESS;
  } else if (access & SharedMemory::READ == SharedMemory::READ) {
    protection = PAGE_READONLY;
  } else if (access & SharedMemory::WRITE == SharedMemory::WRITE) {
    protection = PAGE_READWRITE;
  } else if (access & (SharedMemory::READ|SharedMemory::WRITE) == (SharedMemory::READ|SharedMemory::WRITE)) {
    protection = PAGE_READWRITE;
  } else if (access & SharedMemory::EXECUTE) {
    if (access & SharedMemory::WRITE) {
      protection = PAGE_EXECUTE_READWRITE;
    } else if (access & SharedMemory::READ) {
      protection = PAGE_EXECUTE_READ;
    } else {
      protection = PAGE_EXECUTE;
    }
  }
  
  DWORD previousProtection;
  assert(
    ::VirtualProtect(address, region.getSize(), protection, &previousProtection),
    MemoryException(this)
  );
#else // unix
  int protection;
  if (access == 0) {
    protection = PROT_NONE;
  } else {
    if (access & SharedMemory::READ) {
      protection |= PROT_READ;
    }
    if (access & SharedMemory::WRITE) {
      protection |= PROT_WRITE;
    }
#if defined(PROT_EXEC)
    if (access & SharedMemory::EXECUTE) {
      protection |= PROT_EXEC;
    }
#endif
  }
  assert(
    ::mprotect(address, region.getSize(), protection) == 0,
    MemoryException("Unable to unlock process memory", Type::getType<SharedMemory>())
  );
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::synchronize(bool asynchronous) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  BOOL status = ::FlushViewOfFile(address, getSize());
  assert(status != 0, MemoryException("Unable to synchronize memory", Type::getType<SharedMemory>()));
#else // unix
  int flags = (asynchronous) ? (MS_ASYNC) : (MS_SYNC);
  assert(
    ::msync(address, region.getSize(), flags) == 0,
    MemoryException("Unable to synchronize memory", Type::getType<SharedMemory>())
  );
#endif // flavor
}

SharedMemory::SharedMemoryImpl::~SharedMemoryImpl() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  BOOL status = ::UnmapViewOfFile(address);
  ASSERT(status != 0);
  ::CloseHandle(handle);
#else // unix
  int status = ::munmap(address, getSize());
  ASSERT(status == 0);
#endif // flavor
}

// see http://www.engelschall.com/sw/mm

unsigned int SharedMemory::getGranularity() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SYSTEM_INFO info;
  ::GetSystemInfo(&info);
  return info.dwAllocationGranularity;
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    return 4096 * 2; // TAG: fixme
  #else
    long size = ::sysconf(_SC_PAGE_SIZE);
    ASSERT((size > 0) && (size <= PrimitiveTraits<unsigned int>::MAXIMUM));
    return size;
  #endif
#endif // flavor
}

// TAG: security - need ACL and mode support - owner, group, and other
SharedMemory::SharedMemory(const String& path, const FileRegion& region, unsigned int access, unsigned int options) throw(FileException, MemoryException) {
  File::Access fileAccess = File::READ;
  if (access & SharedMemory::WRITE) {
    if (access & (SharedMemory::READ|SharedMemory::EXECUTE)) {
      fileAccess = File::READWRITE;
    } else {
      fileAccess = File::WRITE;
    }
  }
  unsigned int fileOptions = 0;
  if (options & SharedMemory::PREPARE) {
    fileOptions |= File::CREATE;
  }
  File file(path, fileAccess, fileOptions);
  if (options & SharedMemory::PREPARE) {
    long long size = file.getSize();
    if (size  < region.getEnd()) {
      file.truncate(region.getEnd()); // raises exception of no write access
    }
  }

  sharedMemory = new SharedMemoryImpl(file, region, access); // TAG: resource leak if exception
  
  if (options & SharedMemory::CLEAR) {
    clear();
  }
}

SharedMemory::SharedMemory(const SharedMemory& copy) throw() : sharedMemory(copy.sharedMemory) {
}

SharedMemory& SharedMemory::operator=(const SharedMemory& eq) throw() {
  sharedMemory = eq.sharedMemory;
  return *this;
}

void SharedMemory::lock() throw(MemoryException) {
  sharedMemory->lock();
}

void SharedMemory::unlock() throw(MemoryException) {
  sharedMemory->unlock();
}

void SharedMemory::setProtection(unsigned int access) throw(MemoryException) {
  sharedMemory->setProtection(access);
}

void SharedMemory::synchronize(bool asynchronous) throw(MemoryException) {
  sharedMemory->synchronize(asynchronous);
}

void SharedMemory::clear() throw() {
  fill<char>(sharedMemory->getBytes(), sharedMemory->getSize(), 0);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
