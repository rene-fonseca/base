/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H

#include <base/MemoryException.h>
#include <base/mem/AllocatorEnumeration.h>

/**
  This class encapsulates the implementation used by the Allocator class. Do not use directly.

  @see Allocator
  @author René Møller Fonseca
  @version 1.0
*/

class AllocatorImpl {
protected:

  /** The allocated memory block. */
  void* allocated;
  /** The size of the allocated memory block in bytes. */
  unsigned int size;
public:

  /**
    Initializes an empty allocator.
  */
  explicit AllocatorImpl() throw();

  /**
    Initializes an allocator.

    @param size The size in bytes of the allocated memory block.
  */
  explicit AllocatorImpl(unsigned int size) throw(MemoryException);

  /**
    Initializes allocator from other allocator.
  */
  AllocatorImpl(const AllocatorImpl& copy) throw(MemoryException);

  /**
    Assignment of allocator by allocator.
  */
  AllocatorImpl& operator=(const AllocatorImpl& eq) throw(MemoryException);

  /**
    Returns the allocated memory.
  */
  inline void* getElements() throw() {
    return allocated;
  }

  /**
    Returns the allocated memory.
  */
  inline const void* getElements() const throw() {
    return allocated;
  }

  /**
    Returns the size of the allocated memory.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Sets the size of the allocated memory.
  */
  void setSize(unsigned int size) throw(MemoryException);

  /**
    Destroys the allocator.
  */
  ~AllocatorImpl() throw();
};



/**
  Allocator of resizeable memory block.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Allocator : private AllocatorImpl {
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
private:

  /** The size of the memory block using the size of TYPE. */
  unsigned int elements;
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit Allocator() throw() {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate the requested
    number of elements.

    @param size Specifies the initial size of the allocator.
  */
  inline explicit Allocator(unsigned int size) throw(MemoryException) :
    AllocatorImpl(size * sizeof(TYPE)), elements(size) {}

  /**
    Initializes allocator from other allocator.
  */
  inline Allocator(const Allocator& copy) throw(MemoryException) : AllocatorImpl(copy) {}

  /**
    Assignment of allocator by allocator.
  */
  inline Allocator& operator=(const Allocator& eq) throw(MemoryException) {
    AllocatorImpl::operator=(eq);
    return *this;
  }

  /**
    Returns the elements of the allocator for modifying access.
  */
  inline TYPE* getElements() throw() {
    return static_cast<TYPE*>(AllocatorImpl::getElements());
  }

  /**
    Returns the elements of the allocator for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return static_cast<const TYPE*>(AllocatorImpl::getElements());
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return elements;
  }

  /**
    Sets the size of the allocated memory.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      AllocatorImpl::setSize(size * sizeof(TYPE));
    }
  }
};

#endif
