/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__SHARED_MEMORY_H
#define _DK_SDU_MIP__BASE_MEM__SHARED_MEMORY_H

#include <base/MemoryException.h>
#include <base/OperatingSystem.h>
#include <base/io/File.h>
#include <base/mem/Reference.h>
#include <base/NotSupported.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Shared memory support.
  
  @short Shared memory.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SharedMemory : public Object {
public:
  
  static inline OperatingSystem::Handle getHandle(const File& file) throw() {
    return file.getHandle();
  }
  
  class SharedMemoryImpl : public ReferenceCountedObject {
  private:

    /** The file object. */
    File file;
    /** The file region (i.e. size and offset). */
    FileRegion region;
    /** Handle to the shared memory object. */
    OperatingSystem::Handle handle;
    /** Specifies the access to the shared memory. */
    unsigned int access;
    /** The base address of the shared memory block. */
    char* address;
  public:

    SharedMemoryImpl(const File& file, const FileRegion& region, unsigned int access) throw(MemoryException);
    
    inline char* getBytes() const throw() {
      return address;
    }
    
    inline const File& getFile() const throw() {
      return file;
    }
    
    inline long long getOffset() const throw() {
      return region.getOffset();
    }
    
    inline unsigned int getSize() const throw() {
      return region.getSize();
    }
    
    inline const FileRegion& getRegion() const throw() {
      return region;
    }
    
    inline unsigned int getAccess() throw() {
      return access;
    }
    
    void lock() throw(MemoryException);
    
    void unlock() throw(MemoryException);
    
    void setProtection(unsigned int access) throw(MemoryException);
    
    void synchronize(bool asynchronous) throw(MemoryException);
    
    ~SharedMemoryImpl() throw();
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
  static unsigned int getGranularity() throw();
  
  /**
    Initializes shared memory object.

    @param path The path of the file.
    @param region The file region to map into memory.
    @param access The required access.
    @param options The options.
  */
  SharedMemory(const String& path, const FileRegion& region, unsigned int access, unsigned int options) throw(FileException, MemoryException);

  /**
    Initializes object by object.
  */
  SharedMemory(const SharedMemory& copy) throw();
  
  /**
    Assignment of object by object.
  */
  SharedMemory& operator=(const SharedMemory& eq) throw();
  
  /**
    Returns the offset within the file of the mapped region.
  */
  inline long long getOffset() const throw() {
    return sharedMemory->getOffset();
  }

  /**
    Returns the size of the map.
  */
  inline unsigned int getSize() const throw() {
    return sharedMemory->getSize();
  }
  
  /**
    Returns the region of the mapped.
  */
  inline FileRegion getRegion() const throw() {
    return sharedMemory->getRegion();
  }
  
  /**
    Returns the shared bytes for modifying access.
  */
  inline char* getBytes() throw() {
    return sharedMemory->getBytes();
  }
  
  /**
    Returns the shared bytes for non-modifying access.
  */
  inline const char* getBytes() const throw() {
    return sharedMemory->getBytes();
  }
  
  /**
    Locks the memory.
  */
  void lock() throw(MemoryException);
  
  /**
    Unlocks the memory.
  */
  void unlock() throw(MemoryException);
  
  /**
    Sets the desired protection of the shared memory.
  */
  void setProtection(unsigned int access) throw(MemoryException);

  /**
    Synchronize memory with file.

    @param asynchronous Allows the method to return before all data has been written. The default is false.
  */
  void synchronize(bool asynchronous = false) throw(MemoryException);

  /**
    Fills the memory with zeros.
  */
  void clear() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
