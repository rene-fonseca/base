/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__DEBUG_DYNAMIC_MEMORY_H
#define _DK_SDU_MIP__BASE_MEM__DEBUG_DYNAMIC_MEMORY_H

#include <base/mem/DynamicMemory.h>
#include <base/mem/MemoryCorruption.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides support for dynamic memory allocation/deallocation with
  debug support. This class should not be used in final application releases.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DebugDynamicMemory {
  friend void* operator new(unsigned int) throw(MemoryException);
  friend void operator delete(void*) throw(MemoryException);
  friend void* operator new[](unsigned int) throw(MemoryException);
  friend void operator delete[](void*) throw(MemoryException);
  friend class DebugDynamicMemoryImpl;
private:

  /** Holds the current number of allocated memory blocks. */
  static unsigned long currentAllocations;
  /** Global synchronization object. */
  static SpinLock spinLock;

  /** Memory block descriptor. */
  struct Descriptor {
    /** Magic number identifying the descriptor. */
    void* magic;
    /** The size of the memory block. */
    unsigned int size;
    /** Specifies whether or not the memory block has been allocated/deleted. */
    bool allocated;
  };
public:

  /** Specifies the size of the prefix region. */
  static const unsigned int PREFIX_WORDS = 128;
  /** Specifies the size of the suffix region. */
  static const unsigned int SUFFIX_WORDS = 128;

  /**
    Allocates the specified number of bytes on the heap. The behaviour is
    unspecified if the desired size is 0.

    @param size The desired size of the memory block in number of bytes.

    @return 0 if not successful.
  */
  static void* allocate(unsigned int size) throw();

  /**
    Frees the specified memory block previously allocated by allocate(). Does
    nothing if the pointer is 0.
    
    @param memory The memory block.

    @return True on success.
  */
  static bool release(void* memory) throw(MemoryCorruption);
};

#if 0
/**
  Allocates a block of dynamic memory.
  
  @return 0 if the requested size is 0.
*/
inline void* operator new(unsigned int size) throw(MemoryException) {
  if (size == 0) {
    return 0;
  }
  void* result = DebugDynamicMemory::allocate(size);
  if (result == 0) {
    throw MemoryException();
  }
  return result;
}

/**
  Releases a dynamic memory block previously allocated by new.
*/
inline void operator delete(void* memory) throw(MemoryException) {
  if (!DebugDynamicMemory::release(memory)) {
    throw MemoryException(); // TAG: is this allowed
  }
}

/**
  Allocates a block of dynamic memory for an array.

  @return 0 if the requested size is 0.
*/
inline void* operator new[](unsigned int size) throw(MemoryException) {
  if (size == 0) {
    return 0;
  }
  void* result = DebugDynamicMemory::allocate(size);
  if (result == 0) {
    throw MemoryException();
  }
  return result;
}

/**
  Releases dynamic memory previously allocated by new[].
*/
inline void operator delete[](void* memory) throw(MemoryException) {
  if (!DebugDynamicMemory::release(memory)) {
    throw MemoryException(); // TAG: is this allowed
  }
}
#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
