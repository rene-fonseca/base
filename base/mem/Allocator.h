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
#include <base/mem/Leaky.h>
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
public:

#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
  static constexpr bool clearMemory = true;
#else
  static constexpr bool clearMemory = false; // memory clear should be handled in destructor since object can be new'ed - but for primitives it makes sense to have option
#endif
  static constexpr bool useSafeDestroy = false;
private:

  /** The allocated memory block. */
  TYPE* elements = nullptr;
  /** The number of elements in the block. */
  MemorySize size = 0;
protected:

  /** Detaches buffer. */
  inline Span<TYPE> detach()
  {
    Span<TYPE> span(elements, size);
    elements = nullptr;
    size = 0;
    return span;
  }

  /** Attaches buffer. */
  inline void attach(TYPE* _buffer, MemorySize _size)
  {
    BASSERT(!elements && !size);
    elements = _buffer;
    size = _size;
  }

  /** Attaches buffer. */
  inline void attach(const Span<TYPE>& span)
  {
    BASSERT(!elements && !size);
    elements = span.buffer;
    size = span.size;
  }

  enum MemoryFill {
    INIT_MEMORY = 0xaa,
    DESTROY_OBJECT = 0xbb,
    RELEASE_MEMORY = 0xdd, // 0xcc is used by Windows
    SECURITY_MEMORY = 0xee // used instead of DESTROY_OBJECT/RELEASE_MEMORY for release builds when memory clear is enabled
  };

  /** Fills the memory. */
  static inline void fill(TYPE* dest, MemorySize size, MemoryFill memoryFill)
  {
    if (clearMemory) {
      base::fill<uint8>(reinterpret_cast<uint8*>(dest), size * sizeof(TYPE), memoryFill);
    }
  }

  inline TYPE* allocate(MemorySize size)
  {
    BASSERT(size > 0);
    auto result = Heap::allocate<TYPE>(size);
    fill(result, size, INIT_MEMORY);
    return result;
  }

  inline TYPE* resize(TYPE* buffer, MemorySize newSize, MemorySize originalSize)
  {
    // buffer can be nullptr
    BASSERT((!buffer && !originalSize) || (buffer && originalSize));
    if (!INLINE_ASSERT(newSize != originalSize)) {
      return buffer;
    }
    if (originalSize > newSize) {
      fill(buffer + newSize, originalSize - newSize, RELEASE_MEMORY);
    }
    auto result = Heap::resize(buffer, newSize); // reallocates - so do NOT have initialized objects in memory
    if (originalSize < newSize) {
      fill(result + originalSize, newSize - originalSize, INIT_MEMORY);
    }
    return result;
  }

  inline TYPE* tryResize(TYPE* buffer, MemorySize newSize, MemorySize originalSize) noexcept
  {
    // buffer can be nullptr
    BASSERT((!buffer && !originalSize) || (buffer && originalSize));
    if (!INLINE_ASSERT(newSize != originalSize)) {
      return buffer;
    }
    if (originalSize > newSize) {
      fill(buffer + newSize, originalSize - newSize, RELEASE_MEMORY); // memory may not be released though
    }
    auto result = Heap::tryResize<TYPE>(buffer, newSize); // wont throw - elements pointer still good!
    BASSERT((originalSize > newSize) ? result : true);
    BASSERT(!result || (result == buffer));
    if (result) {
      if (originalSize < newSize) {
        fill(result + originalSize, newSize - originalSize, INIT_MEMORY);
      }
    }
    return result;
  }

  inline void release(TYPE* buffer, MemorySize size)
  {
    BASSERT((!buffer && !size) || (buffer && size));
    if (buffer) {
      fill(buffer, size, RELEASE_MEMORY);
      Heap::release(buffer); // could throw - but would indicate heap corruption
    }
  }
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

    @return Returns the number of failed destroys.
  */
  static MemorySize destroyAll(TYPE* begin, const TYPE* _end) noexcept
  {
    MemorySize failed = 0;
    BASSERT(begin <= _end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      TYPE* end = (_end - begin) + begin;
      while (begin != end) { // we destroy from the end
        --end;
        try {
          end->~TYPE(); // this can throw - if only 1 exception object is unwound - std::terminate() could still be called
        } catch (...) {
          ++failed;
        }
        fill(end, 1, DESTROY_OBJECT);
      }
    } else {
      fill(begin, _end - begin, DESTROY_OBJECT);
    }
    return failed;
  }

  /**
    Destroys the elements of the sequence. Does nothing for uninitializeable objects.
  */
  static void destroy(TYPE* begin, const TYPE* _end)
  {
    if (useSafeDestroy) {
      auto failed = destroyAll(begin, _end); // TAG: needs testing - MSC behavior unknown
      return;
    }

    BASSERT(begin <= _end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE) {
      TYPE* end = (_end - begin) + begin;
      while (begin != end) { // we destroy from the end
        --end;
        end->~TYPE(); // this can throw
        // we cannot recover since object throwing is now in bad state - ie. partially destructed
        fill(end, 1, DESTROY_OBJECT);
      }
    } else {
      fill(begin, _end - begin, DESTROY_OBJECT);
    }
  }


  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end)
  {
    BASSERT(src <= end);
    if (INLINE_ASSERT(!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE && !Relocateable<TYPE>::IS_RELOCATEABLE)) {
      while (src != end) {
        new(dest++) TYPE(std::move(*src++)); // move objects - less likely to throw than destroy but we really dont know
      }
      // src still valid object so do NOT fill
    } else {
      fill(src, end - src, DESTROY_OBJECT);
    }
  }

  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end, bool& moved)
  {
    BASSERT(src <= end);
    if (!Uninitializeable<TYPE>::IS_UNINITIALIZEABLE && !Relocateable<TYPE>::IS_RELOCATEABLE) {
      initializeByMove(dest, src, end); // we move all first
      moved = true; // from here we can recover to some degree by leaking src
      // we cannot release heap since object can have indirect self references
      destroy(src, end); // now we destroy all
    } else { // just copy memory
      while (src != end) {
        *dest++ = *src++; // must not throw
      }
      moved = true;
      fill(src, end - src, DESTROY_OBJECT);
    }
  }
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
  {
    Span<TYPE> span(allocate(_size), _size);
    Leaky<TYPE> leaky(span);
    initialize(span.buffer, span.buffer + span.size); // default initialization of elements
    attach(span);
  }

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& copy)
  {
    Span<TYPE> span(allocate(copy.size), copy.size);
    Leaky<TYPE> leaky(span);
    initializeByCopy(span.buffer, copy.elements, copy.size);
    attach(span);
  }

  Allocator(Allocator&& move) noexcept
  {
    attach(move.detach());
  }

  /**
    Assignment of allocator by allocator.
  */
  Allocator& operator=(const Allocator& copy)
  {
    if (&copy != this) { // protect against self assignment
      // we do NOT use clear() since we need to reuse heap
      auto original = detach();
      Leaky<TYPE> leaky(original);
      destroy(original.buffer, original.buffer + original.size); // can throw
      auto buffer = resize(original.buffer, copy.size, original.size);
      initializeByCopy(buffer, copy.elements, copy.size); // initialization of elements by copying
      attach(buffer, copy.size);
    }
    return *this;
  }

  Allocator& operator=(Allocator&& move) noexcept
  {
    if (&move != this) { // protect against self assignment
      clear();
      attach(move.detach());
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
        elements = resize(elements, size, this->size); // ok if this throws
        if (value) { // fill new elements
          if (size > this->size) { // we are increased array size
            base::fill(elements + this->size, size - this->size, *value);
          }
        }
        this->size = size;
      } else { // non-primitive case
        TYPE* temp = nullptr;
        if (size > this->size) { // extend array
          temp = tryResize(elements, size, this->size); // wont throw - elements pointer still good!
          BASSERT(!temp || (temp == elements));
          if (!temp) {
            temp = allocate(size); // new array - ok if this throws here
          }
        }
        auto original = detach();
        Leaky<TYPE> leaky(original); // TAG: can we add callback to notify parent class to e.g. update cached size

        if (size < original.size) { // are we about to reduce the array
          destroy(original.buffer + size, original.buffer + original.size); // we cannot recover if this throws
          auto elements = resize(original.buffer, size, original.size);
          attach(elements, size);
        } else { // array is to be expanded
          if (temp != original.buffer) { // not if inplace resized
            initializeByMove(temp, original.buffer, original.buffer + original.size); // we still need to destroy
          }
          
          // if construction fails we cannot recover - we can set size to the last valid element and continue
          if (value) {
            initialize(temp + original.size, temp + size, *value); // copy construction
          } else {
            initialize(temp + original.size, temp + size); // default initialization of new objects
          }
          // see "fail construct" example - can we just set size of the successful initialized?
          attach(temp, size); // now we are free to use elements again

          if (temp != original.buffer) { // not if inplace resized
            destroy(original.buffer, original.buffer + original.size); // we cannot recover if this throws - we leak original buffer
            release(original.buffer, original.size); // free previous array
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
    auto original = detach();
    if (original) {
      Leaky<TYPE> leaky(original);
      destroy(original.buffer, original.buffer + original.size); // we cannot recover from throw
      release(original.buffer, original.size); // could throw - but would indicate heap corruption
    }
  }
  
  ~Allocator()
  {
    clear();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
