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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup memory Memory
*/

/**
  This class encapsulates the implementation used by the Heap class. Do not use
  this class directly.

  @short Heap implementation.
  @see Heap
  @ingroup memory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API HeapImpl {
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
    @return nullptr if unable to resize memory block or size was 0.
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
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Heap : private HeapImpl {
public:

  /**
    Allocates the specified number of bytes on the heap. Raises
    MemoryException if unable to allocate the memory block.

    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* allocate(unsigned int size) throw(MemoryException)
  {
    return static_cast<TYPE*>(HeapImpl::allocate(size * sizeof(TYPE)));
  }

  /**
    Resizes the specified memory block. Raises MemoryException if unable
    allocate memory block.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* resize(TYPE* heap, unsigned int size) throw(MemoryException)
  {
    return static_cast<TYPE*>(HeapImpl::resize(heap, size * sizeof(TYPE)));
  }

  /**
    Tries to resizes the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
    @return nullptr if unable to resize memory block or size was 0.
  */
  template<class TYPE>
  inline static TYPE* tryResize(void* heap, unsigned int size) throw(MemoryException)
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return static_cast<TYPE*>(HeapImpl::tryResize(heap, size * sizeof(TYPE)));
#else // Unix
    return nullptr; // not implemented
#endif
  }

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Raises MemoryException if unable to free the memory.

    @param heap The memory block.
  */
  template<class TYPE>
  inline static void release(TYPE* heap) throw(MemoryException)
  {
    HeapImpl::release(heap);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
