/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_ALLOCATOR_H

#include <base/Base.h>
#include <base/MemoryException.h>
#include <base/mem/Allocator.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/string/FormatOutputStream.h>

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @short Reference counted allocator.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedAllocator : public ReferenceCountedObject, private Allocator<TYPE> {
public:

  /**
    Enumeration of all the elements of a ReferenceCountedAllocator.
  */
  class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified ReferenceCountedAllocator.

      @param allocator The ReferenceCountedAllocator being enumerated.
    */
    Enumeration(ReferenceCountedAllocator& allocator) throw() :
      AllocatorEnumeration<TYPE, TYPE&, TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };

  /**
    Non-modifying enumeration of all the elements of a ReferenceCountedAllocator.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified ReferenceCountedAllocator.

      @param allocator The ReferenceCountedAllocator being enumerated.
    */
    ReadOnlyEnumeration(const ReferenceCountedAllocator& allocator) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };
private:

  ReferenceCountedAllocator& operator=(const ReferenceCountedAllocator& eq); // prohibit assignment
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit ReferenceCountedAllocator() throw() {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate the requested
    number of elements.

    @param size Specifies the initial size of the allocator.
  */
  inline explicit ReferenceCountedAllocator(unsigned int size) throw(MemoryException) : Allocator<TYPE>(size) {}

  /**
    Initializes the allocator by copying from the specified allocator. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  ReferenceCountedAllocator(const ReferenceCountedAllocator& copy) throw(MemoryException) : Allocator<TYPE>(copy) {}

  /**
    Returns the elements of the allocator for modifying access.
  */
  inline TYPE* getElements() throw() {
    return Allocator<TYPE>::getElements();
  }

  /**
    Returns the elements of the allocator for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return Allocator<TYPE>::getElements();
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return Allocator<TYPE>::getSize();
  }

  /**
    Sets the number of elements of the allocator. The original elements are
    not modified when the size is increased. Throws 'MemoryException' if unable
    to adjust the size of the allocator.

    @param size The desired size.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    Allocator<TYPE>::setSize(size);
  }
};

#endif
