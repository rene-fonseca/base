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
 
  Use capacity to avoid default construction.

  @short Allocator.
  @ingroup memory
  @see ReferenceCountedAllocator
  @version 1.2
*/

template<class TYPE>
class Allocator {
public:

  static constexpr bool useSafeDestroy = false;
private:

  /** The allocated memory block. */
  TYPE* elements = nullptr;
  /** The number of elements in the block. */
  MemorySize size = 0;
  /** Capacity. */
  MemorySize capacity = 0;
protected:

  /** Detaches buffer. */
  inline Span<TYPE> detach()
  {
    Span<TYPE> span(elements, size);
    elements = nullptr;
    size = 0;
    // capacity not impacted
    return span;
  }

  /** Attaches buffer. */
  inline void attach(TYPE* _buffer, MemorySize _size)
  {
    BASSERT(!elements && !size);
    elements = _buffer;
    size = _size;
    // capacity not impacted
  }

  /** Attaches buffer. */
  inline void attach(const Span<TYPE>& span)
  {
    BASSERT(!elements && !size);
    elements = span.buffer;
    size = span.size;
    // capacity not impacted
  }

  enum MemoryFill {
    INIT_MEMORY = 0xaa,
    DESTROY_OBJECT = 0xbb,
    RELEASE_MEMORY = 0xdd // 0xcc is used by Windows
  };

  /** Fills the memory with the given marker. */
  static inline void fill(TYPE* dest, MemorySize size, MemoryFill memoryFill)
  {
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    base::fill<uint8>(reinterpret_cast<uint8*>(dest), size * sizeof(TYPE), memoryFill);
#endif
  }
  
  /** Returns the actual size to allocate. Ie. compensated for capacity. */
  inline MemorySize getAdjustedSize(MemorySize size) const noexcept
  {
    return maximum(size, capacity); // granularity would be handled here too
  }

  /** Allocate new memory. */
  inline TYPE* allocate(const MemorySize size)
  {
    const auto adjustedNewSize = getAdjustedSize(size);
    auto result = Heap::allocate<TYPE>(adjustedNewSize);
    fill(result, adjustedNewSize, INIT_MEMORY);
    return result;
  }

  /** Resize memory buffer. */
  inline TYPE* resize(TYPE* buffer, const MemorySize newSize, const MemorySize originalSize)
  {
    // buffer can be nullptr
    BASSERT((!buffer && !originalSize) || (buffer && originalSize));
    if (!INLINE_ASSERT(newSize != originalSize)) {
      return buffer;
    }
    if (originalSize > newSize) {
      fill(buffer + newSize, originalSize - newSize, RELEASE_MEMORY);
    }
    const auto adjustedNewSize = getAdjustedSize(newSize);
    auto result = Heap::resize(buffer, adjustedNewSize); // reallocates - so do NOT have initialized objects in memory
    if (originalSize < adjustedNewSize) {
      fill(result + originalSize, adjustedNewSize - originalSize, INIT_MEMORY);
    }
    return result;
  }

  /** Inplace memory resize supported. */
  inline bool canResizeInplace() const noexcept
  {
    return Heap::canResizeInplace();
  }
  
  /** Try to inplace resize buffer. */
  inline TYPE* tryResize(TYPE* buffer, const MemorySize newSize, const MemorySize originalSize) noexcept
  {
    // buffer can be nullptr
    BASSERT((!buffer && !originalSize) || (buffer && originalSize));
    if (!INLINE_ASSERT(newSize != originalSize)) {
      return buffer;
    }
    if (originalSize > newSize) {
      fill(buffer + newSize, originalSize - newSize, RELEASE_MEMORY); // memory may not be released though
    }
    const auto adjustedNewSize = getAdjustedSize(newSize);
    auto result = Heap::tryResize<TYPE>(buffer, adjustedNewSize); // wont throw - elements pointer still good!
    BASSERT(!result || (result == buffer));
    if (result) {
      if (originalSize < adjustedNewSize) {
        fill(result + originalSize, adjustedNewSize - originalSize, INIT_MEMORY);
      }
    }
    return result;
  }

  /** Release memory buffer. */
  inline void release(TYPE* buffer, const MemorySize size)
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
    if (!IsUninitializeable<TYPE>()) {
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
    if (IsUninitializeable<TYPE>()) {
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
    if (IsUninitializeable<TYPE>() || IsRelocateable<TYPE>()) {
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
  static void destroy(TYPE* begin, const TYPE* _end, MemorySize& destroyedCount)
  {
    BASSERT(begin <= _end);
    if (IsUninitializeable<TYPE>() || std::is_trivially_destructible<TYPE>()) {
      destroyedCount += (_end - begin);
      fill(begin, _end - begin, DESTROY_OBJECT);
      return;
    }
    
    TYPE* end = (_end - begin) + begin;
    if (useSafeDestroy && !std::is_nothrow_destructible<TYPE>()) {
      std::exception_ptr e;
      while (begin != end) { // we destroy from the end
        --end;
        try {
          ++destroyedCount; // before destroy!
          // TAG: needs testing - MSC behavior unknown
          end->~TYPE(); // this can throw - if only 1 exception object is unwound - std::terminate() could still be called
        } catch (...) {
          // we dont know which exception to rethrow so we just use first
          if (!e) {
            e = std::current_exception();
          }
        }
        fill(end, 1, DESTROY_OBJECT);
      }
      if (e) {
        destroyedCount = _end - begin;
        std::rethrow_exception(e);
      }
    } else {
      while (begin != end) { // we destroy from the end
        --end;
        ++destroyedCount; // before destroy!
        end->~TYPE(); // this can throw // what if noexcept still throws
        fill(end, 1, DESTROY_OBJECT);
      }
    }
  }

  static inline void destroy2(TYPE* begin, const TYPE* end)
  {
    MemorySize destroyedCount = 0;
    destroy(begin, end, destroyedCount);
  }
  
  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end)
  {
    BASSERT(src <= end);
    // TAG: IsRelocateable<TYPE>() - we can copy memory directly

    while (src != end) {
      new(dest++) TYPE(moveObject(*src++)); // move objects - less likely to throw than destroy but we really dont know
    }
    // src still valid object so do NOT fill
#if 0
    if (std::is_copy_constructible<TYPE>() || std::is_move_constructible<TYPE>()) {
      while (src != end) {
        new(dest++) TYPE(moveObject(*src++)); // move objects - less likely to throw than destroy but we really dont know
      }
      // src still valid object so do NOT fill
    } else if (std::is_trivially_constructible<TYPE>() && std::is_copy_assignable<TYPE>()) {
      while (src != end) {
        // no construction
        *dest++ = *src++; // move objects - less likely to throw than destroy but we really dont know
      }
      // src still valid object so do NOT fill
    } else if (std::is_default_constructible<TYPE>() && (std::is_copy_assignable<TYPE>() || std::is_move_assignable<TYPE>())) {
      while (src != end) {
        new(dest) TYPE();
        *dest++ = moveObject(*src++); // move objects - less likely to throw than destroy but we really dont know
      }
      // src still valid object so do NOT fill
    }
#endif
  }

  /**
    Initializes the elements of the sequence by moving elements from other sequence. Arrays must not overlap.
  */
  static void initializeByMove(TYPE* dest, TYPE* src, const TYPE* end, bool& moved)
  {
    BASSERT(src <= end);
    if (!IsUninitializeable<TYPE>() && !IsRelocateable<TYPE>()) {
      initializeByMove(dest, src, end); // we move all first
      moved = true; // from here we can recover to some degree by leaking src
      // we cannot release heap since object can have indirect self references
      destroy2(src, end); // now we destroy all
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

  explicit Allocator(MemorySize _size, MemorySize _capacity)
  {
    ensureCapacity(_capacity);
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
      if (copy.size == 0) {
        clear();
      } else {
        // we do NOT use clear() since we need to reuse heap
        auto original = detach();
        // clearImpl(original);
        Leaky<TYPE> leaky(original);
        if (original) {
          MemorySize destroyedCount = 0;
          try {
            destroy(original.buffer, original.buffer + original.size, destroyedCount);
          } catch (...) {
            if (destroyedCount == original.size) {
              release(original.buffer, original.size);
              leaky.clear();
            }
            throw;
          }
        }
        auto buffer = resize(original.buffer, copy.size, original.size);
        initializeByCopy(buffer, copy.elements, copy.size); // initialization of elements by copying
        attach(buffer, copy.size);
      }
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

  inline Span<TYPE> getSpan() noexcept
  {
    return Span<TYPE>(elements, size);
  }

  inline ConstSpan<TYPE> getSpan() const noexcept
  {
    return ConstSpan<TYPE>(elements, size);
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
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator begin() noexcept
  {
    return Iterator(getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator end() noexcept
  {
    return Iterator(getElements() + getSize());
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator begin() const noexcept
  {
    return ReadIterator(getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator end() const noexcept
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
  
  /** Sets the capacity. Only heap will be allocated so objects won't be initialized. Required when object do not have default initialization. */
  inline void ensureCapacity(MemorySize _capacity) noexcept
  {
    capacity = _capacity;
  }

  /** Sets the capacity to the current size. Only heap will be allocated so objects won't be initialized. Required when object do not have default initialization. */
  inline void ensureCapacity() noexcept
  {
    capacity = size;
  }

  /** Returns the capacity. */
  inline MemorySize getCapacity() const noexcept
  {
    return capacity;
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
      if (IsUninitializeable<TYPE>() && IsRelocateable<TYPE>()) {
        // no need to destroy or initialize elements
        elements = resize(elements, size, this->size); // ok if this throws
        if (value) { // fill new elements
          if (size > this->size) { // we are increased array size
            base::fill(elements + this->size, size - this->size, *value);
          }
        }
        this->size = size;
      } else { // non-primitive case
        TYPE* temp = nullptr; // new buffer
        if (size > this->size) { // extend array
          // TAG: check if reallocatable TYPE
          temp = tryResize(elements, size, this->size); // wont throw - elements pointer still good!
          BASSERT(!temp || (temp == elements));
          if (!temp) {
            temp = allocate(size); // new array - ok if this throws here
          }
        } else if (!canResizeInplace() && (size > 0)) {
          temp = allocate(size); // new array - ok if this throws here
        }
        
        auto original = detach();
        Leaky<TYPE> leaky(original); // TAG: can we add callback to notify parent class to e.g. update cached size

        if (size < original.size) { // are we about to reduce the array
          bool reduced = false;
          if (!temp) { // try inplace resize
            reduced = true;
            destroy2(original.buffer + size, original.buffer + original.size); // we cannot recover if this throws
            temp = tryResize(original.buffer, size, original.size); // wont throw
            BASSERT(!temp || (temp == original.buffer)); // reallocation NOT allowed - we still have objects initialized
            if (temp || (size == 0)) {
              attach(temp, size);
              return;
            }
          }
          if (!temp && (size > 0)) {
            try {
              temp = allocate(size); // new array
            } catch (...) {
              attach(original); // not modified
              leaky.clear();
              throw;
            }
          }
          initializeByMove(temp, original.buffer, original.buffer + size); // we still need to destroy
          attach(temp, size);
          destroy2(original.buffer, original.buffer + (reduced ? size : original.size)); // we cannot recover if this throws - we leak original buffer
          release(original.buffer, original.size); // free previous array
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
            destroy2(original.buffer, original.buffer + original.size); // we cannot recover if this throws - we leak original buffer
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

  /** Clear buffer. Capacity is ignored. */ // TAG: probably should enforce capacity
  void clear()
  {
    auto original = detach();
    if (original) {
      Leaky<TYPE> leaky(original);
      MemorySize destroyedCount = 0;
      if (std::is_nothrow_destructible<TYPE>()) {
        destroy(original.buffer, original.buffer + original.size, destroyedCount); // we cannot recover if any item hasn't been destroyed
        BASSERT(destroyedCount == original.size);
      } else {
        try {
          destroy(original.buffer, original.buffer + original.size, destroyedCount); // we cannot recover if any item hasn't been destroyed
        } catch (...) {
          if (destroyedCount == original.size) {
            release(original.buffer, original.size); // could throw - but would indicate heap corruption
            leaky.clear();
          }
          throw;
        }
      }
      release(original.buffer, original.size); // could throw - but would indicate heap corruption
    }
  }

  class NoThrow {
  public:
    
    inline ~NoThrow()
    {
      BASSERT(Exception::isUnwinding());
    }
  };
  
  static inline MemorySize align(MemorySize offset) noexcept
  {
    return Heap::align16(offset);
  }

  /**
    Releases unused memory. Requested capacity is ignored. Any object must not have a cached pointer/iterator to the
    elements!
   
    @return Returns the bytes released.
  */
  MemoryDiff garbageCollect()
  {
    if (!elements) {
      return 0;
    }
    const auto capacity = Heap::getSize(elements);
    if (size == 0) {
      auto original = detach();
      release(original.buffer, original.size); // ok if this throws
      return capacity * sizeof(TYPE);
    }
    if (align(capacity * sizeof(TYPE)) > align(size * sizeof(TYPE))) {
      if (canResizeInplace()) {
        auto _elements = tryResize(elements, size, capacity);
        if (_elements) {
          BASSERT(_elements == elements);
          return (capacity - Heap::getSize(elements)) * sizeof(TYPE);
        }
      }

      if (IsRelocateable<TYPE>()) {
        elements = resize(elements, size, capacity); // ok if this throws
        return (capacity - Heap::getSize(elements)) * sizeof(TYPE); // could be negative
      }

      // std::is_move_constructible<TYPE>() && std::is_nothrow_move_constructible<TYPE>();

      Span<TYPE> temp(allocate(size), size); // ok if throws
      auto original = detach();
      Leaky<TYPE> leaky(original);
      initializeByMove(temp.buffer, original.buffer, original.buffer + original.size);
      attach(temp);
      destroy2(original.buffer, original.buffer + original.size);
      release(original.buffer, original.size);
      return (capacity - Heap::getSize(temp.buffer)) * sizeof(TYPE);
    }
    return 0;
  }
  
  /**
    Returns the number of bytes that could be garbage collected.
  */
  inline MemorySize getGarbageByteSize() const noexcept
  {
    if (!elements) {
      return 0;
    }
    return align(Heap::getSize(elements) * sizeof(TYPE)) - align(size * sizeof(TYPE));
  }
  
  ~Allocator()
  {
    clear();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
