/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

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

  @see Allocator.
  @author René Møller Fonseca
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

  /**
    Enumeration of all the elements of an Allocator.
  */
  class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified Allocator.

      @param allocator The Allocator being enumerated.
    */
    Enumeration(CapacityAllocator& allocator) throw() :
      AllocatorEnumeration<TYPE, TYPE&, TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an Allocator.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified Allocator.

      @param allocator The Allocator being enumerated.
    */
    ReadOnlyEnumeration(const CapacityAllocator& allocator) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };
public:

  /**
    Initializes an empty allocator with the default granularity.
  */
  inline explicit CapacityAllocator() throw() : capacity(0), granularity(DEFAULT_GRANULARITY) {}

  /**
    Initializes an empty allocator. Throws 'OutOfRange' if granularity is less
    than MINIMUM_GRANULARITY.

    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline explicit CapacityAllocator(unsigned int granularity) throw() : capacity(0), granularity(granularity) {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange();
    }
  }

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements. Throws 'OutOfRange' if granularity
    is less than MINIMUM_GRANULARITY.

    @param size Specifies the initial size of the allocator.
    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline CapacityAllocator(unsigned int size, unsigned int granularity) throw(OutOfRange, MemoryException) : capacity(0), granularity(granularity) {
    if (granularity < MINIMUM_GRANULARITY) {
      throw OutOfRange();
    }
    setSize(size);
  }

  /**
    Initializes allocator from other allocator.
  */
  inline CapacityAllocator(const CapacityAllocator& cpy) throw(MemoryException) : capacity(0), granularity(0) {
    setGranularity(cpy.getGranularity());
    setSize(cpy.getSize());
      // we only want to copy the first 'capacity' number of elements!
    copy<TYPE>(getElements(), cpy.getElements(), capacity); // blocks do not overlap
  }

  /**
    Assignment of allocator by allocator.
  */
  inline CapacityAllocator& operator=(const CapacityAllocator& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      setGranularity(eq.getGranularity());
      setSize(eq.getSize());
      // we only want to copy the first 'capacity' number of elements!
      copy<TYPE>(getElements(), copy.getElements(), capacity); // blocks do not overlap
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
    Sets the number of elements of the allocator. If the size is increased the
    original elements are not modified and the newly allocated elements are not
    initialized. Throws 'MemoryException' if unable to allocate additional
    memory (does not throw an exception when the size is decreased or
    unchanged). If the size is reduced the elements up to the new size are
    unchanged.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != capacity) {
      capacity = size;
      Allocator<TYPE>::setSize((capacity + granularity - 1)/granularity * granularity);
    }
  }

  /**
    Sets the block size. This does, however, not influence the allocated amount
    of memory until the 'size' is adjusted. Throws 'OutOfRange' if granularity
    is less than MINIMUM_GRANULARITY.
  */
  inline void setGranularity(unsigned int granularity) throw(OutOfRange) {
    if (granularity != this->granularity) {
      if (granularity < MINIMUM_GRANULARITY) {
        throw OutOfRange();
      }
      this->granularity = granularity;
    }
  }

  /**
    Returns the capacity of the allocator.
  */
  inline unsigned int getCapacity() const throw() {return Allocator<TYPE>::getSize();}

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
    // internal knowledge: does not throw an exception 'cause we do not expand the buffer
    Allocator<TYPE>::setSize(capacity);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
