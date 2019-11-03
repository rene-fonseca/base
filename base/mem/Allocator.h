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
#include <base/Functor.h>
#include <base/mem/Heap.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/iterator/SequenceIterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Allocator of resizeable memory block. The implementation is not MT-safe.

  @short Allocator.
  @ingroup memory
  @see ReferenceCountedAllocator CapacityAllocator
  @version 1.2
*/

template<class TYPE>
class Allocator {
private:

  /** The allocated memory block. */
  TYPE* elements = nullptr;
  /** The number of elements in the block. */
  MemorySize size = 0;
public:

  typedef SequenceIterator<IteratorTraits<TYPE> > Iterator;
  typedef SequenceIterator<ReadIteratorTraits<TYPE> > ReadIterator;

  typedef AllocatorEnumerator<EnumeratorTraits<TYPE> > Enumerator;
  typedef AllocatorEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;

  /**
    Initializes the elements of the sequence using the default constructor.
    Uninitializeable objects are not initialized.
  */
  static void initialize(TYPE* dest, const TYPE* end)
  {
    ASSERT(dest <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      while (dest != end) {
        new(dest) TYPE(); // inplace initialization
        ++dest;
      }
    }
  }

  /**
    Initializes the elements of the sequence using the given value.
    Uninitializeable objects are not initialized.
  */
  static void initialize(TYPE* dest, const TYPE* end, const TYPE& value)
  {
    ASSERT(dest <= end);
    if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      while (dest != end) {
        *dest = value;
        ++dest;
      }
    } else {
      while (dest != end) {
        new(dest) TYPE(value); // inplace initialization
        ++dest;
      }
    }
  }

  /**
    Initializes the elements of the sequence by copying elements from other
    sequence. The memory image is copied directly for relocatable objects.
  */
  static void initializeByCopy(TYPE* restrict dest, const TYPE* restrict src, MemorySize count)
  {
    if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE || Relocateable<TYPE>::IS_RELOCATEABLE) {
      copy<TYPE>(dest, src, count); // blocks do not overlap
    } else {
      const TYPE* end = dest + count;
      while (dest != end) {
        new(dest) TYPE(*src); // copy object
        ++dest;
        ++src;
      }
    }
  }

  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end)
  {
    ASSERT(src <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE && !Relocateable<TYPE>::IS_RELOCATEABLE) {
      while (src != end) {
        new(dest) TYPE(*src); // copy object
        src->~TYPE(); // destroy old object
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
        fill<uint8>(reinterpret_cast<uint8*>(src), sizeof(TYPE), 0xaa);
#endif
        ++dest;
        ++src;
      }
    } else {
      while (src != end) {
        *dest = *src;
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
        fill<uint8>(reinterpret_cast<uint8*>(src), sizeof(TYPE), 0xaa);
#endif
        ++dest;
        ++src;
      }
    }
  }

  /**
    Destroys the elements of the sequence. Does nothing for uninitializeable objects.
  */
  static void destroy(TYPE* dest, const TYPE* end)
  {
    ASSERT(dest <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      while (dest != end) {
        dest->~TYPE();
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
        fill<uint8>(reinterpret_cast<uint8*>(dest), sizeof(TYPE), 0xaa);
#endif
        ++dest;
      }
    }
  }
  
//  /**
//    Enumeration of all the elements of an Allocator.
//  */
//  class Enumerator : public AllocatorEnumerator<EnumeratorTraits<TYPE> > {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified Allocator.
//
//      @param allocator The Allocator being enumerated.
//    */
//    Enumerator(Allocator& allocator) noexcept
//      : AllocatorEnumerator<Traits>(allocator.getElements(), allocator.getElements() + allocator.getSize())
//    {
//    }
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of an Allocator.
//  */
//  class ReadEnumerator : public AllocatorEnumerator<ReadEnumeratorTraits<TYPE> > {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the specified Allocator.
//
//      @param allocator The Allocator being enumerated.
//    */
//    ReadEnumerator(const Allocator& allocator) noexcept
//      : AllocatorEnumerator<Traits>(allocator.getElements(), allocator.getElements() + allocator.getSize())
//    {
//    }
//  };
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit Allocator() noexcept
  {
  }

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Raises MemoryException if unable to allocate enough memory to
    hold the requested number of elements.

    @param size Specifies the initial size of the allocator.
  */
  explicit Allocator(MemorySize _size) throw(MemoryException)
    : elements(Heap::allocate<TYPE>(_size)), size(_size)
  {
    initialize(elements, elements + size); // default initialization of elements
  }

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& copy) throw(MemoryException)
    : elements(Heap::allocate<TYPE>(copy.size)), size(copy.size)
  {
    // initialization of elements by copying
    initializeByCopy(elements, copy.elements, size);
  }

  Allocator(Allocator&& move)
    : elements(std::move(move.elements)), size(move.size)
  {
    move.elements = nullptr;
    move.size = 0;
  }

  /**
    Assignment of allocator by allocator.
  */
  Allocator& operator=(const Allocator& copy) throw(MemoryException)
  {
    if (&copy != this) { // protect against self assignment
      destroy(elements, elements + size);
      size = copy.size;
      elements = Heap::resize(elements, size);
      initializeByCopy(elements, copy.elements, size); // initialization of elements by copying
    }
    return *this;
  }

  Allocator& operator=(Allocator&& move) throw(MemoryException)
  {
    if (&move != this) { // protect against self assignment
      elements = std::move(move.elements);
      size = move.size;
      move.elements = nullptr;
      move.size = 0;
    }
    return *this;
  }

  /**
    Returns the elements of the allocator for modifying access.
  */
  inline TYPE* getElements() noexcept
  {
    return elements;
  }

  /**
    Returns the elements of the allocator for non-modifying access.
  */
  inline const TYPE* getElements() const noexcept
  {
    return elements;
  }

  /**
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator getBeginIterator() noexcept
  {
    return Iterator(getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator getEndIterator() noexcept
  {
    return Iterator(getElements() + getSize());
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const noexcept
  {
    return ReadIterator(getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const noexcept
  {
    return ReadIterator(getElements() + getSize());
  }

  /**
    Returns a modifying enumerator of the allocator.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(getElements(), getElements() + getSize());
  }

  /**
    Returns a non-modifying enumerator of the allocator.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(getElements(), getElements() + getSize());
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns the number of allocated bytes.
  */
  inline MemorySize getByteSize() const noexcept
  {
    return size * sizeof(TYPE);
  }

  /**
    Returns true if no elements are allocated.
  */
  inline bool isEmpty() const noexcept
  {
    return size == 0;
  }

  /**
    Sets the number of elements of the allocator. If the size is increased the
    original elements are not modified and the newly allocated elements are not
    initialized. Raises MemoryException if unable to allocate additional
    memory (does not throw an exception when the size is decreased or
    unchanged). If the size is reduced the elements up to the new size are
    unchanged.

    @param size The desired size.
  */
  void setSize(MemorySize size)
  {
    if (size != this->size) {
      if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
        // no need to destroy or initialize elements
        elements = Heap::resize(elements, size);
      } else {
        if (size < this->size) { // are we about to reduce the array
          destroy(elements + size, elements + this->size);
          elements = Heap::resize(elements, size);
        } else { // array is to be expanded
          TYPE* temp = Heap::allocate<TYPE>(size); // new array
          initializeByMove(temp, elements, elements + this->size);
          Heap::release(elements); // free previous array
          elements = temp;
          // default initialization of new objects
          initialize(elements + this->size, elements + size);
        }
      }
      this->size = size;
    }
  }

  /** Same as setSize() but fills new elements with the given value instead of using default initialization. */
  void setSize(MemorySize size, const TYPE& value)
  {
    if (size != this->size) {
      if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
        // no need to destroy or initialize elements
        elements = Heap::resize(elements, size);
        if (size > this->size) { // are have increased array size
          TYPE* dest = elements;
          const TYPE* end = dest + size;
          while (dest != end) {
            *dest = value;
            ++dest;
          }
        }
      } else {
        if (size < this->size) { // are we about to reduce the array
          destroy(elements + size, elements + this->size);
          elements = Heap::resize(elements, size);
        } else { // array is to be expanded
          TYPE* temp = Heap::allocate<TYPE>(size); // new array
          initializeByMove(temp, elements, elements + this->size);
          Heap::release(elements); // free previous array
          elements = temp;
          // initialization of new objects by value
          initialize(elements + this->size, elements + size, value);
        }
      }
      this->size = size;
    }
  }

  ~Allocator()
  {
    destroy(elements, elements + size);
    Heap::release(elements);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
