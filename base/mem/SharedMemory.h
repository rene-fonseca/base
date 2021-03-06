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

#include <base/MemoryException.h>
#include <base/OperatingSystem.h>
#include <base/io/File.h>
#include <base/mem/Reference.h>
#include <base/NotSupported.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Shared memory support.
  
  @short Shared memory.
  @ingroup memory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SharedMemory : public Object {
public:
  
  class _COM_AZURE_DEV__BASE__API SharedMemoryImpl : public ReferenceCountedObject {
  private:

    /** The file object. */
    File file;
    /** The file region (i.e. size and offset). */
    FileRegion region;
    /** Handle to the shared memory object. */
    OperatingSystem::Handle handle;
    /** Specifies the access to the shared memory. */
    unsigned int access = 0;
    /** The base address of the shared memory block. */
    uint8* address = nullptr;
  public:

    SharedMemoryImpl(
      const File& file,
      const FileRegion& region,
      unsigned int access);
    
    inline uint8* getBytes() const noexcept
    {
      return address;
    }
    
    inline const File& getFile() const noexcept
    {
      return file;
    }
    
    inline long long getOffset() const noexcept
    {
      return region.getOffset();
    }
    
    inline unsigned int getSize() const noexcept
    {
      return region.getSize();
    }
    
    inline const FileRegion& getRegion() const noexcept
    {
      return region;
    }
    
    inline unsigned int getAccess() noexcept
    {
      return access;
    }
    
    void lock();
    
    void unlock();
    
    void setProtection(unsigned int access);
    
    void synchronize(bool asynchronous);
    
    ~SharedMemoryImpl() noexcept;
  };

  friend class SharedMemoryImpl;
  
  /** The internal shared memory representation. */
  Reference<SharedMemoryImpl> sharedMemory;
  
  /** Access to shared memory. */
  enum Access {
    READ = 1, /**< Read access. */
    WRITE = 2, /**< Write access. */
    READWRITE = READ|WRITE, /**< Read and write access. */
    EXECUTE = 4, /**< Execute access. */
    ANY = READWRITE|EXECUTE /**< Read, write, and execute access. */
  };

  /** Options. */
  enum Option {
    /** Specifies that the file should be extended to the specified file region. */
    PREPARE = 1,
    /** Specifies that the region should be initialized with zeros. */
    CLEAR = 2
  };
  
  /**
    Returns the granularity of addresses/offsets within the shared memory block
    and file object.
  */
  static MemorySize getGranularity() noexcept;
  
  /**
    Initializes shared memory object.

    @param path The path of the file.
    @param region The file region to map into memory.
    @param access The required access.
    @param options The options.
  */
  SharedMemory(const String& path, const FileRegion& region, unsigned int access, unsigned int options);

  /**
    Initializes object by object.
  */
  SharedMemory(const SharedMemory& copy) noexcept;
  
  /**
    Assignment of object by object.
  */
  SharedMemory& operator=(const SharedMemory& assign) noexcept;
  
  /**
    Returns the offset within the file of the mapped region.
  */
  inline long long getOffset() const noexcept {
    return sharedMemory->getOffset();
  }

  /**
    Returns the size of the map.
  */
  inline unsigned int getSize() const noexcept {
    return sharedMemory->getSize();
  }
  
  /**
    Returns the region of the mapped.
  */
  inline const FileRegion& getRegion() const noexcept {
    return sharedMemory->getRegion();
  }
  
  /**
    Returns the shared bytes for modifying access.
  */
  inline uint8* getBytes() noexcept {
    return sharedMemory->getBytes();
  }
  
  /**
    Returns the shared bytes for non-modifying access.
  */
  inline const uint8* getBytes() const noexcept {
    return sharedMemory->getBytes();
  }
  
  /**
    Locks the memory.
  */
  void lock();
  
  /**
    Unlocks the memory.
  */
  void unlock();
  
  /**
    Sets the desired protection of the shared memory.
  */
  void setProtection(unsigned int access);

  /**
    Synchronize memory with file.

    @param asynchronous Allows the method to return before all data has been
    written. The default is false.
  */
  void synchronize(bool asynchronous = false);

  /**
    Fills the memory with zeros.
  */
  void clear() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
