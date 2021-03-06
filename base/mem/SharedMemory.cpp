/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/SharedMemory.h>
#include <base/Profiler.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
#    define _LARGEFILE64_SOURCE 1
#  endif
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREERTOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <sys/mman.h>
#endif
#  include <unistd.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SharedMemory::SharedMemoryImpl::SharedMemoryImpl(
  const File& _file,
  const FileRegion& _region,
  unsigned int _access)
  : file(_file), region(_region), access(_access)
{

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD protection =
    (access & SharedMemory::WRITE) ? PAGE_READWRITE : PAGE_READONLY;
  handle = ::CreateFileMapping(
    file.getHandle(),
    0, // security
    protection, // protection
    0, // high word of size
    region.getSize(), // low word of size
    0 // name of mapping
  );
  if (!handle) {
    _throw MemoryException("Unable to open shared memory.", this);
  }
  if (handle == OperatingSystem::INVALID_HANDLE) {
    _throw MemoryException("Unable to open shared memory.", this);
  }

  address = (uint8*)::MapViewOfFile(
    handle, // handle to file-mapping object
    (access & SharedMemory::WRITE) ? FILE_MAP_WRITE : FILE_MAP_READ, // access mode
    getHighWordOf64(region.getOffset()), // high word of offset
    getLowWordOf64(region.getOffset()), // low word of offset
    region.getSize() // number of bytes to map
  );
  if (access == NULL) {
    ::CloseHandle(handle);
    _throw MemoryException("Unable to open shared memory.", this);
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  handle = OperatingSystem::INVALID_HANDLE;
  int protection = 0;
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
  
  #if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM)
    address = (uint8*)::mmap64(
      0,
      region.getSize(),
      protection,
      MAP_SHARED,
      file.getHandle(),
      region.getOffset()
    );
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    bassert(
      (region.getOffset() >= 0) &&
      (region.getOffset() <= PrimitiveTraits<int>::MAXIMUM),
      MemoryException("Unable to open shared memory.", this)
    );
    address = (uint8*)::mmap(
      0,
      region.getSize(),
      protection,
      MAP_SHARED,
      file.getHandle(),
      region.getOffset()
    );
  #endif
  bassert(
    address !=  MAP_FAILED,
    MemoryException("Unable to open shared memory.", this)
  );
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::lock()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::VirtualLock(address, region.getSize()),
    MemoryException("Unable to lock process memory.", this)
  );
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else
    bassert(
      ::mlock(address, region.getSize()) == 0,
      MemoryException("Unable to lock memory.", this)
    );
  #endif
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::unlock()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  bassert(
    ::VirtualUnlock(address, region.getSize()),
    MemoryException("Unable to unlock process memory.", this)
  );
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
  #elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
        (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
    BASSERT(!"Not supported.");
  #else  
    bassert(
      ::munlock(address, region.getSize()) == 0,
      MemoryException("Unable to unlock process memory.", this)
    );
  #endif
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::setProtection(
  unsigned int access)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD protection = 0;
  if (access == 0) {
    protection = PAGE_NOACCESS;
  } else if ((access & SharedMemory::READ) == SharedMemory::READ) {
    protection = PAGE_READONLY;
  } else if ((access & SharedMemory::WRITE) == SharedMemory::WRITE) {
    protection = PAGE_READWRITE;
  } else if ((access & (SharedMemory::READ|SharedMemory::WRITE)) == (SharedMemory::READ|SharedMemory::WRITE)) {
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
  
  DWORD previousProtection = 0;
  bassert(
    ::VirtualProtect(address, region.getSize(), protection, &previousProtection),
    MemoryException(this)
  );
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  int protection = 0;
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
  bassert(
    ::mprotect(address, region.getSize(), protection) == 0,
    MemoryException(
      "Unable to unlock process memory.",
      Type::getType<SharedMemory>()
    )
  );
#endif // flavor
}

void SharedMemory::SharedMemoryImpl::synchronize(
  bool asynchronous)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  BOOL status = ::FlushViewOfFile(address, getSize());
  bassert(
    status != 0,
    MemoryException(
      "Unable to synchronize memory.",
      Type::getType<SharedMemory>())
  );
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  int flags = (asynchronous) ? (MS_ASYNC) : (MS_SYNC);
  bassert(
    ::msync(address, region.getSize(), flags) == 0,
    MemoryException(
      "Unable to synchronize memory.",
      Type::getType<SharedMemory>()
    )
  );
#endif // flavor
}

SharedMemory::SharedMemoryImpl::~SharedMemoryImpl() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  BOOL status = ::UnmapViewOfFile(address);
  BASSERT(status != 0);
  ::CloseHandle(handle);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  BASSERT(!"Not supported.");
#else // unix
  int status = ::munmap(address, getSize());
  BASSERT(status == 0);
#endif // flavor
}

// see http://www.engelschall.com/sw/mm

MemorySize SharedMemory::getGranularity() noexcept
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
    return size;
  #endif
#endif // flavor
}

// TAG: security - need ACL and mode support - owner, group, and other
SharedMemory::SharedMemory(
  const String& path,
  const FileRegion& region,
  unsigned int access,
  unsigned int options)
{
  Profiler::ResourceCreateTask profile("SharedMemory::SharedMemory()");

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

SharedMemory::SharedMemory(const SharedMemory& copy) noexcept
  : sharedMemory(copy.sharedMemory)
{
}

SharedMemory& SharedMemory::operator=(const SharedMemory& assign) noexcept
{
  sharedMemory = assign.sharedMemory;
  return *this;
}

void SharedMemory::lock()
{
  Profiler::WaitTask profile("SharedMemory::lock()");
  sharedMemory->lock();
}

void SharedMemory::unlock()
{
  Profiler::pushSignal("SharedMemory::unlock()");
  sharedMemory->unlock();
}

void SharedMemory::setProtection(unsigned int access)
{
  sharedMemory->setProtection(access);
}

void SharedMemory::synchronize(bool asynchronous)
{
  Profiler::IOFlushTask profile("SharedMemory::synchronize()");
  sharedMemory->synchronize(asynchronous);
}

void SharedMemory::clear() noexcept
{
  fill<uint8>(sharedMemory->getBytes(), sharedMemory->getSize(), 0);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
