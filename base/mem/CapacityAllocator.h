/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__CAPACITY_ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__CAPACITY_ALLOCATOR_H

#include <base/mem/Allocator.h>
#include <base/OutOfRange.h>

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
  static const unsigned int MINIMUM_BLOCK_SIZE = 16;
private:

  /** The number of elements in the block. */
  unsigned int capacity;
  /** The block size. */
  unsigned int blockSize;
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
    Initializes an empty allocator.

    @param blockSize The desired blockSize.
  */
  inline explicit CapacityAllocator(unsigned int blockSize) throw() : capacity(0), blockSize(blockSize) {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements. Throws 'OutOfRange' is blockSize is
    less than MINIMUM_BLOCK_SIZE.

    @param size Specifies the initial size of the allocator.
    @param blockSize Specifies the number of elements to allocate at a time.
  */
  inline CapacityAllocator(unsigned int size, unsigned int blockSize) throw(OutOfRange, MemoryException) : capacity(0), blockSize(blockSize) {
    if (blockSize < MINIMUM_BLOCK_SIZE) {
      throw OutOfRange();
    }
    setSize(size);
  }

  /**
    Initializes allocator from other allocator.
  */
  inline CapacityAllocator(const CapacityAllocator& cpy) throw(MemoryException) : capacity(0) {
    setBlockSize(eq.getBlockSize());
    setSize(cpy.getSize());
      // we only want to copy the first 'capacity' number of elements!
    copy<TYPE>(elements, copy.elements, size); // blocks do not overlap
  }

  /**
    Assignment of allocator by allocator.
  */
  inline CapacityAllocator& operator=(const CapacityAllocator& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      setBlockSize(eq.getBlockSize());
      setSize(eq.getSize());
      // we only want to copy the first 'capacity' number of elements!
      copy<TYPE>(elements, copy.elements, size); // blocks do not overlap
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
    Returns the block size.
  */
  inline unsigned int getBlockSize() const throw() {
    return blockSize;
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
      Allocator<TYPE>::setSize((capacity + blockSize - 1)/blockSize * blockSize);
    }
  }

  /**
    Sets the block size. This does, however, not influence the allocated amount
    of memory until the 'size' is adjusted. Throws 'OutOfRange' is blockSize is
    less than MINIMUM_BLOCK_SIZE.
  */
  inline void setBlockSize(unsigned int blockSize) throw(OutOfRange) {
    if (blockSize != this->blockSize) {
      if (blockSize < MINIMUM_BLOCK_SIZE) {
        throw OutOfRange();
      }
      this->blockSize = blockSize;
    }
  }

  /**
    Releases any memory that is not absolutely required (i.e. the block size is
    ignored). This member function is normally used when we know that the
    allocated memory is not going to be resized for a "long time".
  */
  inline void optimize() throw() {
    // internal knowledge: does not throw an exception 'cause we do not expand the buffer
    Allocator<TYPE>::setSize(capacity);
  }
};

#endif
