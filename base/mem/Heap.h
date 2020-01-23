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
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Memory span. */
template<class TYPE>
class Span {
public: // public for now

  TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  inline Span() noexcept
  {
  }

  inline Span(TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
    BASSERT((!buffer && !size) || (buffer && size));
  }

  inline Span(TYPE* _buffer, const TYPE* end) noexcept
    : buffer(_buffer), size(end - _buffer)
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

  inline TYPE& operator[](MemorySize index) noexcept
  {
    return buffer[index];
  }

  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }

  inline operator bool() const noexcept
  {
    return size != 0;
  }
};

/** Memory span. */
template<class TYPE>
class ConstSpan {
public: // public for now

  const TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  inline ConstSpan() noexcept
  {
  }

  inline ConstSpan(const TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
    BASSERT((!buffer && !size) || (buffer && size));
  }

  inline ConstSpan(const TYPE* _buffer, const TYPE* end) noexcept
    : buffer(_buffer), size(end - _buffer)
  {
    BASSERT((!buffer && !size) || (buffer && size));
  }

  inline ConstSpan(const Span<TYPE>& span) noexcept
    : buffer(span.buffer), size(span.size)
  {
    BASSERT((!buffer && !size) || (buffer && size));
  }

  inline const TYPE* begin() const noexcept
  {
    return buffer;
  }

  inline const TYPE* end() const noexcept
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

  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }

  inline operator bool() const noexcept
  {
    return size != 0;
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

  /** Returns the total heap used. */
  static MemorySize getHeapUsed() noexcept;

  /**
    Allocates the specified number of bytes on the heap. Returns nullptr if
    unable to allocate the memory block. Returns nullptr for size 0.

    @param size The desired size of the memory block in number of bytes.
  */
  static void* allocateNoThrow(MemorySize size) noexcept;

  /**
    Allocates the specified number of bytes on the heap. Raises
    MemoryException if unable to allocate the memory block. Returns nullptr
    for size 0.

    @param size The desired size of the memory block in number of bytes.
  */
  static void* allocate(MemorySize size);

  /**
    Resizes the specified memory block. Raises MemoryException if unable
    allocate memory block. Memory block may be moved when memory is made smaller.
    Returns nullptr for size 0.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
  */
  static void* resize(void* heap, MemorySize size);
  
  /**
    Tries to resize the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of bytes.
    @return nullptr if unable to resize memory block or size was 0.
  */
  static void* tryResize(void* heap, MemorySize size);

  /**
    Frees the specified memory block previously allocated by 'allocate' or
    'reallocate'. Raises MemoryException if unable to free the memory.

    @param heap The memory block.
  */
  static void release(void* heap);

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

  /** Returns the minimum block size. */
  static inline MemorySize getMinimumSize() noexcept
  {
    return HeapImpl::getMinimumSize();
  }

  /**
    Allocates the specified number of bytes on the heap. Returns
    nullptr if unable to allocate the memory block.

    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* allocateNoThrow(MemorySize size) noexcept
  {
    return static_cast<TYPE*>(HeapImpl::allocateNoThrow(size * sizeof(TYPE)));
  }

  /**
    Allocates the specified number of bytes on the heap. Raises
    MemoryException if unable to allocate the memory block.

    @param size The desired size of the memory block in number of elements.
  */
  template<class TYPE>
  inline static TYPE* allocate(MemorySize size)
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
  inline static TYPE* resize(TYPE* heap, MemorySize size)
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
    Tries to resize the specified memory block without movement. Raises
    MemoryException on allocation failure.

    @param heap The memory block.
    @param size The desired size of the memory block in number of elements.
    @return nullptr if unable to resize memory block or size was 0.
  */
  template<class TYPE>
  inline static TYPE* tryResize(void* heap, MemorySize size)
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
  inline static void release(TYPE* heap)
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



/**
  Pointer to heap. Note destructor will not release heap! Likewise copy construction and
  assignment copy the pointer value. This is to behave like a normal
  pointer. You must call release explicitly.
*/
template<typename TYPE>
class HeapBlock {
protected:

  TYPE* heap = nullptr;
public:

  /** No memory. */
  inline HeapBlock() noexcept
  {
  }

  /** Allocates memory of given size. */
  inline HeapBlock(MemorySize size)
  {
    heap = Heap::allocate<TYPE>(size);
  }

  /** Copy of pointer value! */
  inline HeapBlock(const HeapBlock& copy) noexcept
    : heap(copy.heap) // like a normal pointer!
  {
  }

  /** Copy of pointer value! */
  inline HeapBlock(HeapBlock&& move) noexcept
    : heap(move.heap) // like a normal pointer!
  {
    move.heap = nullptr;
  }

  /** Assignment of pointer value! */
  inline HeapBlock& operator=(const HeapBlock& assign) noexcept
  {
    heap = assign.heap; // like a normal pointer!
    return *this;
  }

  /** Assignment of pointer value! */
  inline HeapBlock& operator=(NullPtr assign) noexcept
  {
    heap = assign; // like a normal pointer!
    return *this;
  }

  /** Assignment of pointer value! */
  inline HeapBlock& operator=(TYPE* assign) noexcept
  {
    heap = assign; // like a normal pointer!
    return *this;
  }

  /** Assignment of pointer value! */
  inline HeapBlock& operator=(HeapBlock&& assign) noexcept
  {
    if (this != &assign) {
      heap = assign.heap; // like a normal pointer!
      assign.heap = nullptr;
    }
    return *this;
  }

  /** Resizes block. */
  inline void resize(MemorySize size)
  {
    // TAG: should we release if size == 0? - can be done inside Heap::resize()
    heap = Heap::resize<TYPE>(heap, size);
    BASSERT((size == 0) || heap); // NOTE: some platforms allocate memory even when size == 0 is requested
  }

  /** Tries inplace resize. Returns true on success. */
  inline bool tryResize(MemorySize size) noexcept
  {
    auto result = Heap::tryResize<TYPE>(heap, size);
    BASSERT(!result || (result == heap));
    return result;
  }

  /** Returns the size of the block. Can return 0 if not supported. */
  inline MemorySize getSize() const noexcept
  {
    return Heap::getSize(heap);
  }

  /** Releases heap. Make sure to call explicitly for normal use cases! */
  inline void release()
  {
    Heap::release<TYPE>(heap);
    heap = nullptr;
  }

  /** Returns the native pointer. */
  inline operator TYPE*() noexcept
  {
    return heap;
  }

  /** Returns the native pointer. */
  inline operator const TYPE*() const noexcept
  {
    return heap;
  }

  /** Returns true if pointer is not nullptr. */
  inline operator bool() const noexcept
  {
    return heap;
  }

  /** Destruct. */
  inline ~HeapBlock()
  {
    heap = nullptr; // leak if not release explicitly
  }
};

template<typename TYPE>
class HeapBlockOwned : public HeapBlock<TYPE> {
private:

  HeapBlockOwned(const HeapBlockOwned& copy) noexcept;
  HeapBlockOwned& operator=(const HeapBlockOwned& assign) noexcept;
public:

  typedef HeapBlock<TYPE> Base;
  
  using Base::resize;
  using Base::tryResize;
  using Base::release;
  using Base::getSize;
  using Base::operator TYPE*;
  using Base::operator const TYPE*;
  using Base::operator bool;

  /** No memory. */
  inline HeapBlockOwned() noexcept
  {
  }

  /** Allocates memory of given size. */
  inline HeapBlockOwned(MemorySize size)
    : Base(size)
  {
  }

  /** Copy of pointer value! */
  inline HeapBlockOwned(Base&& move) noexcept
    : Base(moveObject(move))
  {
  }

  /** Assignment of pointer value! */
  inline HeapBlockOwned& operator=(HeapBlockOwned&& assign) noexcept
  {
    HeapBlock<TYPE>::operator=(moveObject(assign));
    return *this;
  }

  /** Destruct. Releases heap. */
  inline ~HeapBlockOwned()
  {
    release();
  }
};

/** Primitive array. */
template<class TYPE>
class PrimitiveArray {
private:
  
  HeapBlockOwned<TYPE> buffer;
  MemorySize count = 0;
public:

  PrimitiveArray(MemorySize size = 0)
  {
    if (size > 0) {
      buffer.resize(size);
      this->count = size;
    }
  }

  /** Resizes buffer. */
  void resize(MemorySize count)
  {
    if (count != this->count) {
      buffer.resize(count);
      this->count = count;
    }
  }

  /** Returns the native pointer. Returns nullptr if empty. */
  inline operator TYPE*() noexcept
  {
    return buffer;
  }

  /** Returns the native pointer. Returns nullptr if empty. */
  inline operator const TYPE*() const noexcept
  {
    return buffer;
  }

  /** Returns the size. */
  inline MemorySize size() const noexcept
  {
    return count;
  }

  /** Returns the begin iterator. */
  inline TYPE* begin() noexcept
  {
    return buffer;
  }

  /** Returns the end iterator. */
  inline TYPE* end() noexcept
  {
    return static_cast<TYPE*>(buffer) + count;
  }

  /** Returns the begin iterator. */
  inline const TYPE* cbegin() const noexcept
  {
    return buffer;
  }

  /** Returns the end iterator. */
  inline const TYPE* cend() const noexcept
  {
    return static_cast<const TYPE*>(buffer)+ count;
  }

  /** Returns the item at given index. */
  inline TYPE& operator[](MemorySize index) noexcept
  {
    return buffer[index];
  }

  /** Returns the item at given index. */
  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }
};

/** Primitive array which uses stack for small buffer and otherwise heap. Uses 4096 bytes on stack by default but minimum 1 item on stack. */
template<class TYPE, MemorySize STACK_SIZE = (sizeof(TYPE) <= 4096) ? 4096/sizeof(TYPE) : 1>
class PrimitiveStackArray {
private:

  TYPE stack[STACK_SIZE];
  HeapBlockOwned<TYPE> heap;
  MemorySize count = 0;
  TYPE* buffer = nullptr;

  inline void update() noexcept
  {
    if (count == 0) {
      buffer = nullptr; // not required in many cases - sometimes buffer is checked against nullptr
      return;
    }
    buffer = (count <= STACK_SIZE) ? stack : heap;
  }
public:

  /** Constructs buffer of given size. */
  PrimitiveStackArray(MemorySize size = 0) // we could use STACK_SIZE - but makes it harder to refactor later
  {
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    // do not initialize buffer for release build
    clear(stack);
#endif
    BASSERT(size <= STACK_SIZE);
    if (size > 0) {
      if (size > STACK_SIZE) {
        heap.resize(size);
      }
      this->count = size;
      update();
    }
  }

  /** Returns true if only stack is used. */
  inline bool isUsingStack() const noexcept
  {
    return count <= STACK_SIZE;
  }

  /** Returns true if heap is used. */
  inline bool isUsingHeap() const noexcept
  {
    return count > STACK_SIZE;
  }

  /** Resizes buffer. */
  void resize(MemorySize count)
  {
    if (count != this->count) {
      // we never go back from heap to stack!
      if (!((this->count <= STACK_SIZE) && (count <= STACK_SIZE))) { // use heap
        heap.resize(count); // copies if already in use
        if (this->count <= STACK_SIZE) {
          copy<TYPE>(heap, stack, this->count);
        }
      }
      this->count = count;
      update();
    }
  }

  /** Returns the native pointer. Returns nullptr if empty. */
  inline operator TYPE*() noexcept
  {
    return buffer;
  }

  /** Returns the native pointer. Returns nullptr if empty. */
  inline operator const TYPE*() const noexcept
  {
    return buffer;
  }

  /** Returns the size. */
  inline MemorySize size() const noexcept
  {
    return count;
  }

  /** Returns the begin iterator. */
  inline TYPE* begin() noexcept
  {
    return buffer;
  }

  /** Returns the end iterator. */
  inline TYPE* end() noexcept
  {
    return buffer + count;
  }

  /** Returns the begin iterator. */
  inline const TYPE* cbegin() const noexcept
  {
    return buffer;
  }

  /** Returns the end iterator. */
  inline const TYPE* cend() const noexcept
  {
    return buffer + count;
  }

#if 0
  /** Returns the item at given index. */
  inline TYPE& operator[](MemorySize index) noexcept
  {
    return buffer[index];
  }

  /** Returns the item at given index. */
  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }
#endif
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
