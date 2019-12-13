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

#include <base/mem/Allocator.h>
#include <base/OutOfRange.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: deprecated

template<class TYPE>
class CapacityAllocator : public Allocator<TYPE> {
public:
  
  /** Specifies the minimum block size in number of elements. */
  static constexpr MemorySize MINIMUM_GRANULARITY = 16;
  /** Specifies the default granularity in number of elements. Guaranteed to be
      greater than or equal to MINIMUM_GRANULARITY. */
  static constexpr MemorySize DEFAULT_GRANULARITY = MINIMUM_GRANULARITY;

  inline CapacityAllocator() noexcept
  {
  }
  
  inline CapacityAllocator(MemorySize size) throw(MemoryException)
  {
    Allocator<TYPE>::setSize(size);
  }
  
  inline CapacityAllocator(MemorySize size, MemorySize capacity) throw(MemoryException)
  {
    Allocator<TYPE>::ensureCapacity(capacity);
    Allocator<TYPE>::setSize(size);
  }
};

#if 0

/**
  Same as Allocator but allocates memory in larger blocks at a time. The
  implementation is not MT-safe.

  @short Capacity allocator.
  @ingroup memory
  @see Allocator.
  @version 1.0
*/

template<class TYPE>
class CapacityAllocator : public Allocator<TYPE> {
public:

  /** Specifies the minimum block size in number of elements. */
  static constexpr MemorySize MINIMUM_GRANULARITY = 16;
  /** Specifies the default granularity in number of elements. Guaranteed to be
      greater than or equal to MINIMUM_GRANULARITY. */
  static constexpr MemorySize DEFAULT_GRANULARITY = MINIMUM_GRANULARITY;
private:
  
  /** The number of elements in the block. */
  MemorySize size = 0;
  /** The minimum capacity. */
  MemorySize capacity = 0;
  /** The granularity of the allocated block memory. */
  MemorySize granularity = DEFAULT_GRANULARITY;
public:
  
  typedef typename Allocator<TYPE>::Iterator Iterator;
  typedef typename Allocator<TYPE>::ReadIterator ReadIterator;
  typedef typename Allocator<TYPE>::Enumerator Enumerator;
  typedef typename Allocator<TYPE>::ReadEnumerator ReadEnumerator;
public:

  /**
    Initializes an empty allocator with the default granularity.
  */
  inline explicit CapacityAllocator() noexcept
  {
  }

  /**
    Initializes an empty allocator. Raises OutOfRange if granularity is less
    than MINIMUM_GRANULARITY.

    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline explicit CapacityAllocator(MemorySize _granularity) throw(OutOfRange)
    : granularity(_granularity)
  {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange(this);
    }
  }

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Raises MemoryException if unable to allocate enough memory to
    hold the requested number of elements. Raises OutOfRange if granularity
    is less than MINIMUM_GRANULARITY.

    @param size Specifies the initial size of the allocator.
    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline CapacityAllocator(MemorySize size, MemorySize _granularity) throw(OutOfRange, MemoryException)
    : granularity(_granularity)
  {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange(this);
    }
    setSize(size);
  }

  inline CapacityAllocator(MemorySize size, MemorySize _capacity, MemorySize _granularity) throw(OutOfRange, MemoryException)
    : capacity(_capacity),
      granularity(_granularity)
  {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange(this);
    }
    setSize(size);
  }

  inline CapacityAllocator(MemorySize size, const TYPE& value, MemorySize _granularity) throw(OutOfRange, MemoryException)
    : granularity(_granularity)
  {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange(this);
    }
    setSize(size, value);
  }

  /**
    Initializes allocator from other allocator.
  */
  inline CapacityAllocator(const CapacityAllocator& copy) throw(MemoryException)
    : Allocator<TYPE>(copy),
      size(copy.size),
      capacity(copy.capacity),
      granularity(copy.granularity)
  {
  }
  
  /**
    Assignment of allocator by allocator.
  */
  inline CapacityAllocator& operator=(const CapacityAllocator& copy) throw(MemoryException)
  {
    if (&copy != this) { // protect against self assignment
      size = copy.size;
      capacity = copy.capacity;
      granularity = copy.granularity;
      Allocator<TYPE>::operator=(copy);
    }
    return *this;
  }
  
  /**
    Returns the number of elements of the allocator.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns the granularity.
  */
  inline MemorySize getGranularity() const noexcept
  {
    return granularity;
  }

  /**
    Returns true if no elements are allocated.
  */
  inline bool isEmpty() const noexcept
  {
    return size == 0;
  }

  /**
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator getBeginIterator() noexcept
  {
    return Iterator(Allocator<TYPE>::getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator getEndIterator() noexcept
  {
    return Iterator(Allocator<TYPE>::getElements() + size);
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const noexcept
  {
    return ReadIterator(Allocator<TYPE>::getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const noexcept
  {
    return ReadIterator(Allocator<TYPE>::getElements() + size);
  }

  /**
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator begin() noexcept
  {
    return Iterator(Allocator<TYPE>::getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator end() noexcept
  {
    return Iterator(Allocator<TYPE>::getElements() + size);
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator begin() const noexcept
  {
    return ReadIterator(Allocator<TYPE>::getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator end() const noexcept
  {
    return ReadIterator(Allocator<TYPE>::getElements() + size);
  }

  /**
    Returns a modifying enumerator of the allocator.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(Allocator<TYPE>::getElements(), Allocator<TYPE>::getElements() + size);
  }

  /**
    Returns a non-modifying enumerator of the allocator.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(Allocator<TYPE>::getElements(), Allocator<TYPE>::getElements() + size);
  }


  /**
    Sets the number of elements of the allocator. If the size is increased the
    original elements are not modified and the newly allocated elements are not
    initialized. Raises MemoryException if unable to allocate additional
    memory (does not raise an exception when the size is decreased or
    unchanged). If the size is reduced the elements up to the new size are
    unchanged.
  */
  void setSize(MemorySize _size)
  {
    if (_size != size) {
      size = _size;
      Allocator<TYPE>::setSize(
        (maximum(size, capacity) + granularity - 1)/granularity * granularity
      );
    }
  }

  /**
    Resize allocator but use given value to fill any new elements instead of
    using default constructor.
  */
  void setSize(MemorySize _size, const TYPE& value)
  {
    if (_size != size) {
      const MemorySize actualSize = Allocator<TYPE>::getSize();
      Allocator<TYPE>::setSize(
        (maximum(_size, capacity) + granularity - 1)/granularity * granularity,
        value
      );
      if (_size > size) {
        auto dest = Allocator<TYPE>::getElements() + size;
        const auto end = Allocator<TYPE>::getElements() + minimum(_size, actualSize);
        for (; dest != end; ++dest) {
          *dest = value;
        }
      }
      size = _size;
    }
  }

  /**
    Sets the block size. This does, however, not influence the allocated amount
    of memory until the 'size' is adjusted. Raises OutOfRange if granularity
    is less than MINIMUM_GRANULARITY.
  */
  inline void setGranularity(MemorySize granularity) throw(OutOfRange)
  {
    if (granularity != this->granularity) {
      if (granularity < MINIMUM_GRANULARITY) {
        throw OutOfRange(this);
      }
      this->granularity = granularity;
    }
  }

  /**
    Returns the capacity of the allocator.
  */
  inline MemorySize getCapacity() const noexcept
  {
    return Allocator<TYPE>::getSize(); // the actual capacity
  }

  /**
    Ensures that the capacity of the allocator is at least equal to the
    specified minimum.

    @param capacity Specifies the minimum capacity of the allocator.
  */
  void ensureCapacity(MemorySize _capacity) throw(MemoryException)
  {
    if (_capacity != capacity) {
      capacity = _capacity;
      if (capacity > Allocator<TYPE>::getSize()) {
        Allocator<TYPE>::setSize(
          (capacity + granularity - 1)/granularity * granularity
        );
      }
    }
  }

  /**
    Returns the garbage collectable size.
  */
  inline MemorySize getGarbageCollectableSize() const noexcept
  {
    return Allocator<TYPE>::getSize() - getSize();
  }

  /**
    Releases any unused capacity of that is not absolutely required (i.e. the
    granularity is ignored). This member function is normally used when we know
    that the allocated memory is not going to be resized for a "long time".
  */
  void garbageCollect()
  {
    // knowledge: does not raise an exception 'cause we do not expand the buffer
    capacity = 0;
    Allocator<TYPE>::setSize(size); // ignore capacity here
  }
};

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
