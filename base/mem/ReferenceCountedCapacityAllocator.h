/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_CAPACITY_ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_CAPACITY_ALLOCATOR_H

#include <base/Base.h>
#include <base/MemoryException.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedObject.h>

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @see CapacityAllocator
  @short Reference counted capacity allocator.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedCapacityAllocator : public ReferenceCountedObject, public CapacityAllocator<TYPE> {
private:

  /**
    Default assignment is prohibited.
  */
  ReferenceCountedCapacityAllocator& operator=(const ReferenceCountedCapacityAllocator& eq);
public:

  /**
    Initializes an empty allocator.

    @param blockSize The desired blockSize.
  */
  inline explicit ReferenceCountedCapacityAllocator(unsigned int blockSize) throw(OutOfRange) :
    CapacityAllocator<TYPE>(blockSize) {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements. Throws 'OutOfRange' is blockSize is
    less than MINIMUM_BLOCK_SIZE.

    @param size Specifies the initial size of the allocator.
    @param blockSize Specifies the number of elements to allocate at a time.
  */
  inline ReferenceCountedCapacityAllocator(unsigned int size, unsigned int blockSize) throw(OutOfRange, MemoryException) :
    CapacityAllocator<TYPE>(size, blockSize) {}

  /**
    Initializes the allocator by copying from the specified allocator. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  ReferenceCountedCapacityAllocator(const ReferenceCountedCapacityAllocator& copy) throw(MemoryException) :
    CapacityAllocator<TYPE>(copy) {}
};

#endif
