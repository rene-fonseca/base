/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H

#include <base/MemoryException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/Functor.h>

/**
  This class encapsulates the implementation used by the Allocator class. Do not use directly.

  @see Allocator
  @author René Møller Fonseca
  @version 1.01
*/

class AllocatorImpl {
public:

  /**
    Sets the size of the allocated memory.

    @param elements Pointer to the elements.
    @param size The size of the memory block in bytes.
  */
  void* setSize(void* elements, unsigned int size) throw(MemoryException);
};



/**
  Allocator of resizeable memory block.

  @author René Møller Fonseca
  @version 1.01
*/

template<class TYPE>
class Allocator : private AllocatorImpl {
private:

  /** The allocated memory block. */
  TYPE* elements;
  /** The number of elements in the block. */
  unsigned int size;
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
    Enumeration(Allocator& allocator) throw() :
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
    ReadOnlyEnumeration(const Allocator& allocator) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit Allocator() throw() : elements(0), size(0) {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate the requested
    number of elements.

    @param size Specifies the initial size of the allocator.
  */
  inline explicit Allocator(unsigned int size) throw(MemoryException) : elements(0), size(0) {
    setSize(size);
  }

  /**
    Initializes allocator from other allocator.
  */
  inline Allocator(const Allocator& cpy) throw(MemoryException) : elements(0), size(0) {
    setSize(cpy.size);
    copy<TYPE>(elements, cpy.elements, size); // blocks do not overlap
  }

  /**
    Assignment of allocator by allocator.
  */
  inline Allocator& operator=(const Allocator& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      setSize(eq.size);
      copy<TYPE>(elements, copy.elements, size); // blocks do not overlap
    }
    return *this;
  }

  /**
    Returns the elements of the allocator for modifying access.
  */
  inline TYPE* getElements() throw() {
    return elements;
  }

  /**
    Returns the elements of the allocator for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return elements;
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Returns true if no elements are allocated.
  */
  inline bool isEmpty() const throw() {
    return size == 0;
  }

  /**
    Sets the size of the allocated memory.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      elements = static_cast<TYPE*>(AllocatorImpl::setSize(elements, size * sizeof(TYPE)));
    }
  }

  inline ~Allocator() {
    setSize(0); // free memory
  }
};

#endif
