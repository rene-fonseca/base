/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

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
  This class encapsulates the implementation used by the Heap class. Do not use
  this class directly.

  @see Heap
  @author René Møller Fonseca
  @version 1.0
*/

class HeapImpl {
protected:

  /**
    Allocates the specified number of bytes on the heap. Throws
    'MemoryException' if unable to allocate the memory block.

    @param size The desired size of the memory block in number of bytes.
  */
  static void* allocate(unsigned int size) throw(MemoryException);

  /**
    Resizes the specified memory block. Throws 'MemoryException' if unable
    allocate memory block.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
  */
  static void* resize(void* heap, unsigned int size) throw(MemoryException);

  /**
    Tries to resizes the specified memory block without movement. Throws
    'MemoryException' on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
    @return 0 if unable to resize memory block or size was 0.
  */
  static void* tryResize(void* heap, unsigned int size) throw(MemoryException);

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Throws 'MemoryException' if unable to free the memory.

    @param heap The memory block.
  */
  static void release(void* heap) throw(MemoryException);
};

/**
  Heap. Wrapper for memory allocation on the heap.

  @author René Møller Fonseca
  @version 1.0
*/

class Heap : private HeapImpl {
public:

  /**
    Allocates the specified number of bytes on the heap. Throws
    'MemoryException' if unable to allocate the memory block.

    @param size The desired size of the memory block in number of elements.
  */
  template<typename TYPE>
  inline static TYPE* allocate(unsigned int size) throw(MemoryException) {
    return static_cast<TYPE*>(HeapImpl::allocate(size * sizeof(TYPE)));
  }

  /**
    Resizes the specified memory block. Throws 'MemoryException' if unable
    allocate memory block.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
  */
  template<typename TYPE>
  inline static TYPE* resize(TYPE* heap, unsigned int size) throw(MemoryException) {
    return static_cast<TYPE*>(HeapImpl::resize(heap, size * sizeof(TYPE)));
  }

  /**
    Tries to resizes the specified memory block without movement. Throws
    'MemoryException' on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
    @return 0 if unable to resize memory block or size was 0.
  */
  template<typename TYPE>
  inline static TYPE* tryResize(void* heap, unsigned int size) throw(MemoryException) {
#if defined(__win32__)
    return static_cast<TYPE*>(HeapImpl::tryResize(heap, size * sizeof(TYPE)));
#else // __unix__
    return 0; // not implemented
#endif
  }

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Throws 'MemoryException' if unable to free the memory.

    @param heap The memory block.
  */
  template<typename TYPE>
  inline static void release(TYPE* heap) throw(MemoryException) {
    HeapImpl::release(heap);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
