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

#include <base/mem/DynamicMemory.h>
#include <base/mem/MemoryCorruption.h>
#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides support for dynamic memory allocation/deallocation with
  debug support. This class should not be used in final application releases
  due to the added memory and processing requirements.
  
  @short Debug dynamic memory.
  @ingroup memory
  @see DynamicMemory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DebugDynamicMemory {
  friend class DebugDynamicMemoryImpl;
private:

  /** Holds the current number of allocated memory blocks. */
  static PreferredAtomicCounter currentAllocations;
  /** Global synchronization object. */
  static SpinLock spinLock;

  /* Memory block descriptor. */
  struct Descriptor {
    /** Magic number identifying the descriptor. */
    void* magic;
    /** The size of the memory block. */
    MemorySize size;
    /** Specifies whether or not the memory block has been allocated/deleted. */
    bool allocated;
  };
public:

  /** Specifies the size of the prefix region. */
  static const unsigned int PREFIX_WORDS = 128;
  /** Specifies the size of the suffix region. */
  static const unsigned int SUFFIX_WORDS = 128;

  /**
    Allocates the specified number of bytes on the heap. The behavior is
    unspecified if the desired size is 0.

    @param size The desired size of the memory block in number of bytes.

    @return 0 if not successful.
  */
  static void* allocate(MemorySize size) noexcept;

  /**
    Frees the specified memory block previously allocated by allocate(). Does
    nothing if the pointer is 0.
    
    @param memory The memory block.
  */
  static void release(void* memory);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
