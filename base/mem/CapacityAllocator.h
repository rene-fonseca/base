/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__CAPACITY_ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__CAPACITY_ALLOCATOR_H

#include <base/mem/Allocator.h>
#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Same as Allocator but allocates memory in larger blocks at a time. The
  implementation is not MT-safe.

  @short Capacity allocator.
  @ingroup memory
  @see Allocator.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class CapacityAllocator : public Allocator<TYPE> {
public:

  /** Specifies the minimum block size in number of elements. */
  static const unsigned int MINIMUM_GRANULARITY = 16;
  /** Specifies the default granularity in number of elements. Guaranteed to be
      greater than or equal to MINIMUM_GRANULARITY. */
  static const unsigned int DEFAULT_GRANULARITY = MINIMUM_GRANULARITY;
private:
  
  /** The number of elements in the block. */
  unsigned int capacity;
  /** The granularity of the allocated block memory. */
  unsigned int granularity;
public:
  
  typedef typename Allocator<TYPE>::Iterator Iterator;
  typedef typename Allocator<TYPE>::ReadIterator ReadIterator;
  typedef typename Allocator<TYPE>::Enumerator Enumerator;
  typedef typename Allocator<TYPE>::ReadEnumerator ReadEnumerator;
public:

  /**
    Initializes an empty allocator with the default granularity.
  */
  inline explicit CapacityAllocator() throw()
    : capacity(0), granularity(DEFAULT_GRANULARITY) {
  }

  /**
    Initializes an empty allocator. Raises OutOfRange if granularity is less
    than MINIMUM_GRANULARITY.

    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline explicit CapacityAllocator(unsigned int _granularity) throw(OutOfRange)
    : capacity(0), granularity(_granularity) {
    assert(granularity >= MINIMUM_GRANULARITY, OutOfRange(this));
  }

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Raises MemoryException if unable to allocate enough memory to
    hold the requested number of elements. Raises OutOfRange if granularity
    is less than MINIMUM_GRANULARITY.

    @param size Specifies the initial size of the allocator.
    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline CapacityAllocator(unsigned int size, unsigned int _granularity) throw(OutOfRange, MemoryException)
    : capacity(0), granularity(_granularity) {
    assert(granularity >= MINIMUM_GRANULARITY, OutOfRange(this));
    setSize(size);
  }

  /**
    Initializes allocator from other allocator.
  */
  inline CapacityAllocator(const CapacityAllocator& copy) throw(MemoryException)
    : Allocator<TYPE>(copy),
      capacity(copy.capacity),
      granularity(copy.granularity) {
    // TAG: we only want to copy the first 'capacity' number of elements!
  }
  
  /**
    Assignment of allocator by allocator.
  */
  inline CapacityAllocator& operator=(const CapacityAllocator& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      capacity = eq.capacity;
      granularity = eq.granularity;
      Allocator<TYPE>::operator=(eq);
      // TAG: we only want to copy the first 'capacity' number of elements!
    }
    return *this;
  }
  
  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return capacity;
  }

  /**
    Returns the granularity.
  */
  inline unsigned int getGranularity() const throw() {
    return granularity;
  }

  /**
    Returns true if no elements are allocated.
  */
  inline bool isEmpty() const throw() {
    return capacity == 0;
  }

  /**
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator getBeginIterator() throw() {
    return Iterator(getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator getEndIterator() throw() {
    return Iterator(getElements() + getSize());
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const throw() {
    return ReadIterator(getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const throw() {
    return ReadIterator(getElements() + getSize());
  }

  /**
    Returns a modifying enumerator of the allocator.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(getElements(), getElements() + getSize());
  }

  /**
    Returns a non-modifying enumerator of the allocator.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(getElements(), getElements() + getSize());
  }

  /**
    Sets the number of elements of the allocator. If the size is increased the
    original elements are not modified and the newly allocated elements are not
    initialized. Raises MemoryException if unable to allocate additional
    memory (does not raise an exception when the size is decreased or
    unchanged). If the size is reduced the elements up to the new size are
    unchanged.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != capacity) {
      capacity = size;
      Allocator<TYPE>::setSize(
        (capacity + granularity - 1)/granularity * granularity
      );
    }
  }

  /**
    Sets the block size. This does, however, not influence the allocated amount
    of memory until the 'size' is adjusted. Raises OutOfRange if granularity
    is less than MINIMUM_GRANULARITY.
  */
  inline void setGranularity(unsigned int granularity) throw(OutOfRange) {
    if (granularity != this->granularity) {
      assert(granularity >= MINIMUM_GRANULARITY, OutOfRange(this));
      this->granularity = granularity;
    }
  }

  /**
    Returns the capacity of the allocator.
  */
  inline unsigned int getCapacity() const throw() {
    return Allocator<TYPE>::getSize();
  }

  /**
    Ensures that the capacity of the allocator is at least equal to the
    specified minimum.

    @param capacity Specifies the minimum capacity of the allocator.
  */
  inline void ensureCapacity(unsigned int capacity) throw(MemoryException) {
    if (capacity > Allocator<TYPE>::getSize()) {
      Allocator<TYPE>::setSize(capacity);
    }
  }

  /**
    Releases any unused capacity of that is not absolutely required (i.e. the
    granularity is ignored). This member function is normally used when we know
    that the allocated memory is not going to be resized for a "long time".
  */
  inline void optimizeCapacity() throw() {
    // knowledge: does not raise an exception 'cause we do not expand the buffer
    Allocator<TYPE>::setSize(capacity);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
