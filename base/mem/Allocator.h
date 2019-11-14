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

/** Helper class for detecting leaks when these cannot be avoided. */
template<class TYPE>
class Leaky {
private:
  
  TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  inline Leaky() noexcept
  {
  }
  
  inline Leaky(TYPE* object) noexcept
    : buffer(object), size(1)
  {
  }
  
  inline Leaky(TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
  }
  
  inline void clear()
  {
    buffer = nullptr;
    size = 0;
  }
  
  inline ~Leaky()
  {
    if (buffer) {
      if (Exception::isUnwinding()) {
        // Heap::onLeak(Type::getType<TYPE>(), buffer, size * sizeof(TYPE));
        // TAG: print warning - error if leaks are not allowed
        // TAG: show stats on app exit
      }
    }
  }
};

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
    BASSERT(dest <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      while (dest != end) {
        new(dest++) TYPE(); // inplace default initialization
      }
    }
  }

  /**
    Initializes the elements of the sequence using the given value.
    Uninitializeable objects are not initialized.
  */
  static void initialize(TYPE* dest, const TYPE* end, const TYPE& value)
  {
    BASSERT(dest <= end);
    if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      while (dest != end) {
        *dest++ = value;
      }
    } else {
      while (dest != end) {
        new(dest++) TYPE(value); // inplace initialization by copy construction
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
        new(dest++) TYPE(*src++); // copy object
      }
    }
  }

  /**
    Destroys the elements of the sequence. Does nothing for uninitializeable objects.
  */
  static void destroy(TYPE* begin, const TYPE* _end)
  {
    BASSERT(begin <= _end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      TYPE* end = (_end - begin) + begin;
      while (begin != end) { // we destroy from the end
        --end;
        end->~TYPE(); // this can throw
        // we cannot recover since object throwing is now in bad state - ie. partially destructed
  #if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
        fill<uint8>(reinterpret_cast<uint8*>(end), sizeof(TYPE), 0xaa);
  #endif
      }
    }
  }

  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end, bool& moved)
  {
    BASSERT(src <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE && !Relocateable<TYPE>::IS_RELOCATEABLE) {
      // we move all first
      const TYPE* moving = src;
      while (moving != end) {
        new(dest++) TYPE(std::move(*moving++)); // move objects - less likely to throw than destroy but we really dont know
      }
      moved = true; // from here we can recover to some degree by leaking src
      // we cannot release heap since object can have indirect self references
      destroy(src, end); // now we destroy all
    } else { // just copy memory
      while (src != end) {
        *dest = *src; // must not throw
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
        fill<uint8>(reinterpret_cast<uint8*>(src), sizeof(TYPE), 0xaa);
#endif
        ++dest;
        ++src;
      }
      moved = true;
    }
  }

  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end)
  {
    BASSERT(src <= end);
    if (INLINE_ASSERT(!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE && !Relocateable<TYPE>::IS_RELOCATEABLE)) {
      const TYPE* moving = src;
      while (moving != end) {
        new(dest++) TYPE(std::move(*moving++)); // move objects - less likely to throw than destroy but we really dont know
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
  explicit Allocator(MemorySize _size)
    : elements(Heap::allocate<TYPE>(_size)), size(_size)
  {
    initialize(elements, elements + size); // default initialization of elements
  }

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& copy)
    : elements(Heap::allocate<TYPE>(copy.size)), size(copy.size)
  {
    // initialization of elements by copying
    initializeByCopy(elements, copy.elements, size);
  }

  Allocator(Allocator&& move) noexcept
    : elements(std::move(move.elements)), size(move.size)
  {
    move.elements = nullptr;
    move.size = 0;
  }

  /**
    Assignment of allocator by allocator.
  */
  Allocator& operator=(const Allocator& copy)
  {
    if (&copy != this) { // protect against self assignment
      auto original = elements;
      auto originalSize = size;
      elements = nullptr;
      size = 0;
      destroy(original, original + originalSize); // can throw
      original = Heap::resize(original, copy.size);
      initializeByCopy(original, copy.elements, copy.size); // initialization of elements by copying
      elements = original;
      size = copy.size;
    }
    return *this;
  }

  Allocator& operator=(Allocator&& move) noexcept
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
  void setSizeImpl(MemorySize size, const TYPE* value)
  {
    if (size != this->size) {
      if (Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
        // no need to destroy or initialize elements
        elements = Heap::resize(elements, size); // ok if this throws
        if (value) { // fill new elements
          if (size > this->size) { // we are increased array size
            fill(elements + this->size, size - this->size, *value);
          }
        }
        this->size = size;
      } else { // non-primitive case
        TYPE* original = elements;
        const MemorySize originalSize = this->size;
        TYPE* temp = nullptr;
        if (size > originalSize) {
          temp = Heap::tryResize<TYPE>(original, size); // wont throw
          // TAG: get stats vs fails
          if (!temp) {
            temp = Heap::allocate<TYPE>(size); // new array - ok if this throws here
          }
        }
        elements = nullptr; // prevent use of data on exception
        this->size = 0;
        Leaky<TYPE> leaky(original, originalSize);

        if (size < originalSize) { // are we about to reduce the array
          destroy(original + size, original + originalSize); // we cannot recover if this throws
          elements = Heap::resize(original, size);
          BASSERT(elements == original); // reallocation is not allowed
          this->size = size;
          leaky.clear();
        } else { // array is to be expanded
          if (temp != original) { // not if inplace resized
            initializeByMove(temp, original, original + originalSize); // we still need to destroy
          }
          
          // if construction fails we cannot recover - we can set size to the last valid element and continue
          if (value) {
            initialize(temp + originalSize, temp + size, *value); // copy construction
          } else {
            initialize(temp + originalSize, temp + size); // default initialization of new objects
          }
          // see "fail construct" example - can we just set size of the successful initialized?
          elements = temp; // now we are free to use elements again
          this->size = size;
          leaky.clear();

          if (temp != original) { // not if inplace resized
            // TAG: add example for destruct throw in fail also
            destroy(original, original + originalSize); // we cannot recover if this throws - we leak original buffer
            Heap::release(original); // free previous array
          }
        }
      }
    }
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
  inline void setSize(MemorySize size)
  {
    setSizeImpl(size, nullptr);
  }
  
  /** Same as setSize() but fills new elements with the given value instead of using default initialization. */
  inline void setSize(MemorySize size, const TYPE& value)
  {
    setSizeImpl(size, &value);
  }

  void clear()
  {
    auto original = elements;
    auto originalSize = size;
    elements = nullptr;
    size = 0;

    destroy(original, original + originalSize); // we cannot recover
    Heap::release(original); // could throw
  }
  
  ~Allocator()
  {
    clear();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
