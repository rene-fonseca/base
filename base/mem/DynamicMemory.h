/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__DYNAMIC_MEMORY_H
#define _DK_SDU_MIP__BASE_MEM__DYNAMIC_MEMORY_H

#include <base/Primitives.h>
#include <base/Base.h>
#include <base/MemoryException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace alloc {
  void* operator new(MemorySize) throw(MemoryException);
  void operator delete(void*) throw(MemoryException);
  void* operator new[](MemorySize) throw(MemoryException);
  void operator delete[](void*) throw(MemoryException);
};

/**
  This class provides support for dynamic memory allocation/deallocation.

  @short Dynamic memory.
  @ingroup memory
  @see DebugDynamicMemory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DynamicMemory {
  friend void* alloc::operator new(MemorySize) throw(MemoryException);
  friend void alloc::operator delete(void*) throw(MemoryException);
  friend void* alloc::operator new[](MemorySize) throw(MemoryException);
  friend void alloc::operator delete[](void*) throw(MemoryException);
private:

  /**
    Allocates the specified number of bytes on the heap. The behavior is
    unspecified if the desired size is 0.

    @param size The desired size of the memory block in number of bytes.

    @return 0 if not successful.
  */
  static void* allocate(MemorySize size) throw();

  /**
    Frees the specified memory block previously allocated by allocate(). Does
    nothing if the pointer is 0.
    
    @param memory The memory block.

    @return True on success.
  */
  static bool release(void* memory) throw();
};

namespace alloc {
  
  /**
    Dynamic memory placement operator.
  */
  inline void* operator new(MemorySize, void* place) throw() {
    return place;
  }
  
  /**
    Dynamic memory placement operator for arrays.
  */
  inline void* operator new[](MemorySize, void* place) throw() {
    return place;
  }
  
  /**
    Allocates a block of dynamic memory of the specified size in bytes. Raises
    MemoryException if the requested size is 0.
  */
  inline void* operator new(MemorySize size) throw(MemoryException) {
    assert(size > 0, MemoryException()); // removed by compiler
    void* result = DynamicMemory::allocate(size);
    assert(result != 0, MemoryException());
    return result;
  }
  
  /**
    Releases a dynamic memory block previously allocated by new. Raises
    MemoryException if memory is 0.
  */
  inline void operator delete(void* memory) throw(MemoryException) {
    assert(memory && DynamicMemory::release(memory), MemoryException());
  }
  
  /**
    Allocates a block of dynamic memory for an array of the specified size in
    bytes. Raises MemoryException if the requested size is 0.
  */
  inline void* operator new[](MemorySize size) throw(MemoryException) {
    assert(size > 0, MemoryException()); // removed by compiler
    void* result = DynamicMemory::allocate(size);
    assert(result != 0, MemoryException());
    return result;
  }

  /**
    Releases dynamic memory previously allocated by new[]. Raises
    MemoryException is memory is 0.
  */
  inline void operator delete[](void* memory) throw(MemoryException) {
    assert(memory && DynamicMemory::release(memory), MemoryException());
  }
}; // end of namespace - alloc

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#if (!defined(_DK_SDU_MIP__BASE__DEFAULT_MEMORY) && \
      defined(_DK_SDU_MIP__BASE__NAMESPACE))
inline void* operator new(base::MemorySize, void* place) throw() {
  return place;
}

inline void* operator new[](base::MemorySize, void* place) throw() {
  return place;
}

inline void* operator new(base::MemorySize size) throw(base::MemoryException) {
  return base::alloc::operator new(size);
}

inline void operator delete(void* memory) throw(base::MemoryException) {
  base::alloc::operator delete(memory);
}

inline void* operator new[](base::MemorySize size) throw(base::MemoryException) {
  return base::alloc::operator new[](size);
}

inline void operator delete[](void* memory) throw(base::MemoryException) {
  base::alloc::operator delete[](memory);
}
#endif

#endif
