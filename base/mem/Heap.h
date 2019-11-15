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
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Memory span. */
template<class TYPE>
class Span {
public:

  TYPE* buffer = nullptr;
  MemorySize size = 0;

  inline Span(TYPE* _buffer, MemorySize _size)
    : buffer(_buffer), size(_size)
  {
    BASSERT((!buffer && !size) || (buffer && size));
  }

  inline TYPE* begin() noexcept
  {
    return buffer;
  }

  inline TYPE* end() noexcept
  {
    return buffer + size;
  }

  inline const TYPE* cbegin() const noexcept
  {
    return buffer;
  }

  inline const TYPE* cend() const noexcept
  {
    return buffer + size;
  }

  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  inline operator bool() const noexcept
  {
    return buffer;
  }
};

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
  static void* allocate(MemorySize size) throw(MemoryException);

  /**
    Resizes the specified memory block. Raises MemoryException if unable
    allocate memory block. Memory block may be moved when memory is made smaller.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
  */
  static void* resize(void* heap, MemorySize size) throw(MemoryException);
  
  /**
    Tries to resize the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
    @return nullptr if unable to resize memory block or size was 0.
  */
  static void* tryResize(void* heap, MemorySize size) throw(MemoryException);

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Raises MemoryException if unable to free the memory.

    @param heap The memory block.
  */
  static void release(void* heap) throw(MemoryException);

  /** Returns the minimum block size. */
  static MemorySize getMinimumSize() noexcept;

  /** Returns the size of the memory block. */
  static MemorySize getSize(void* heap) noexcept;
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
  inline static TYPE* allocate(MemorySize size) throw(MemoryException)
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
  inline static TYPE* resize(TYPE* heap, MemorySize size) throw(MemoryException)
  {
    return static_cast<TYPE*>(HeapImpl::resize(heap, size * sizeof(TYPE)));
  }

  /** Returns true if inplace reallocation is supported. */
  static bool canResizeInplace() noexcept
  {
  #if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return true;
  #else
    return false;
  #endif
  }

  /**
    Tries to resizes the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
    @return nullptr if unable to resize memory block or size was 0.
  */
  template<class TYPE>
  inline static TYPE* tryResize(void* heap, MemorySize size) throw(MemoryException)
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

  /**
    Returns the size of the memory block.

    @param heap The memory block.
  */
  template<class TYPE>
  inline static MemorySize getSize(TYPE* heap) noexcept
  {
    return HeapImpl::getSize(heap)/sizeof(TYPE);
  }

  /**
    Called to register heap leaks.
  */
  static void onLeak(const Type* type, void* buffer, MemorySize size);

  /** Adjusts offset to 64 bytes alignment. */
  static inline MemorySize align64(MemorySize offset) noexcept
  {
    return (offset + (64 - 1)) & ~static_cast<MemorySize>(64 - 1);
  }

  /** Adjusts offset to 32 bytes alignment. */
  static inline MemorySize align32(MemorySize offset) noexcept
  {
    return (offset + (32 - 1)) & ~static_cast<MemorySize>(32 - 1);
  }

  /** Adjusts offset to 16 bytes alignment. */
  static inline MemorySize align16(MemorySize offset) noexcept
  {
    return (offset + 0xf) & ~static_cast<MemorySize>(0xf);
  }

  /** Adjusts offset to 16 bytes alignment. */
  static inline MemorySize align(MemorySize offset) noexcept
  {
    return (offset + 0xf) & ~static_cast<MemorySize>(0xf);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
