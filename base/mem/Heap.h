/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__HEAP_H
#define _DK_SDU_MIP__BASE_MEM__HEAP_H

#include <base/MemoryException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup memory Memory
*/

/**
  This class encapsulates the implementation used by the Heap class. Do not use
  this class directly.

  @short Heap implementation.
  @see Heap
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class HeapImpl {
protected:

  /**
    Allocates the specified number of bytes on the heap. Raises
    MemoryException if unable to allocate the memory block.

    @param size The desired size of the memory block in number of bytes.
  */
  static void* allocate(unsigned int size) throw(MemoryException);

  /**
    Resizes the specified memory block. Raises MemoryException if unable
    allocate memory block.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
  */
  static void* resize(void* heap, unsigned int size) throw(MemoryException);

  /**
    Tries to resize the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
    @return 0 if unable to resize memory block or size was 0.
  */
  static void* tryResize(void* heap, unsigned int size) throw(MemoryException);

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Raises MemoryException if unable to free the memory.

    @param heap The memory block.
  */
  static void release(void* heap) throw(MemoryException);
};

/**
  Heap. Wrapper for memory allocation on the heap.

  @short Heap provider.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Heap : private HeapImpl {
public:

  /**
    Allocates the specified number of bytes on the heap. Raises
    MemoryException if unable to allocate the memory block.

    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* allocate(unsigned int size) throw(MemoryException) {
    return static_cast<TYPE*>(HeapImpl::allocate(size * sizeof(TYPE)));
  }

  /**
    Resizes the specified memory block. Raises MemoryException if unable
    allocate memory block.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* resize(TYPE* heap, unsigned int size) throw(MemoryException) {
    return static_cast<TYPE*>(HeapImpl::resize(heap, size * sizeof(TYPE)));
  }

  /**
    Tries to resizes the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
    @return 0 if unable to resize memory block or size was 0.
  */
  template<class TYPE>
  inline static TYPE* tryResize(void* heap, unsigned int size) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    return static_cast<TYPE*>(HeapImpl::tryResize(heap, size * sizeof(TYPE)));
#else // Unix
    return 0; // not implemented
#endif
  }

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Raises MemoryException if unable to free the memory.

    @param heap The memory block.
  */
  template<class TYPE>
  inline static void release(TYPE* heap) throw(MemoryException) {
    HeapImpl::release(heap);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
