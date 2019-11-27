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

#include <base/Primitives.h>
#include <base/Base.h>
#include <base/MemoryException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/*
namespace alloc {

  void* operator new(MemorySize) throw(MemoryException);
  void operator delete(void*) throw(MemoryException);
  void* operator new[](MemorySize) throw(MemoryException);
  void operator delete[](void*) throw(MemoryException);
};
*/

/**
  This class provides support for dynamic memory allocation/deallocation.

  @short Dynamic memory.
  @ingroup memory
  @see DebugDynamicMemory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DynamicMemory {
/*
  friend void* alloc::operator new(MemorySize) throw(MemoryException);
  friend void alloc::operator delete(void*) throw(MemoryException);
  friend void* alloc::operator new[](MemorySize) throw(MemoryException);
  friend void alloc::operator delete[](void*) throw(MemoryException);
*/
private:

  /**
    Allocates the specified number of bytes on the heap. The behavior is
    unspecified if the desired size is 0.

    @param size The desired size of the memory block in number of bytes.

    @return nullptr if not successful.
  */
  static void* allocate(MemorySize size) noexcept;

  /**
    Frees the specified memory block previously allocated by allocate(). Does
    nothing if the pointer is 0.
    
    @param memory The memory block.
  */
  static void release(void* memory) noexcept;
};

#if 0
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
    bassert(size > 0, MemoryException()); // removed by compiler
    void* result = DynamicMemory::allocate(size);
    bassert(result != 0, MemoryException());
    return result;
  }
  
  /**
    Releases a dynamic memory block previously allocated by new. Raises
    MemoryException if memory is 0.
  */
  inline void operator delete(void* memory) throw(MemoryException) {
    bassert(memory && DynamicMemory::release(memory), MemoryException());
  }
  
  /**
    Allocates a block of dynamic memory for an array of the specified size in
    bytes. Raises MemoryException if the requested size is 0.
  */
  inline void* operator new[](MemorySize size) throw(MemoryException) {
    bassert(size > 0, MemoryException()); // removed by compiler
    void* result = DynamicMemory::allocate(size);
    bassert(result != 0, MemoryException());
    return result;
  }

  /**
    Releases dynamic memory previously allocated by new[]. Raises
    MemoryException is memory is 0.
  */
  inline void operator delete[](void* memory) throw(MemoryException) {
    bassert(memory && DynamicMemory::release(memory), MemoryException());
  }
}; // end of namespace - alloc
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#if (!defined(_COM_AZURE_DEV__BASE__DEFAULT_MEMORY) && \
      defined(_COM_AZURE_DEV__BASE__NAMESPACE))
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
