/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_CAPACITY_ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_CAPACITY_ALLOCATOR_H

#include <base/Base.h>
#include <base/MemoryException.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @see CapacityAllocator
  @short Reference counted capacity allocator.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
    Initializes an empty allocator with the default granularity.
  */
  inline explicit ReferenceCountedCapacityAllocator() throw() : CapacityAllocator<TYPE>() {}

  /**
    Initializes an empty allocator.

    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline explicit ReferenceCountedCapacityAllocator(unsigned int granularity) throw(OutOfRange) :
    CapacityAllocator<TYPE>(granularity) {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements. Throws 'OutOfRange' if granularity
    is less than MINIMUM_GRANULARITY.

    @param size Specifies the initial size of the allocator.
    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline ReferenceCountedCapacityAllocator(unsigned int size, unsigned int granularity) throw(OutOfRange, MemoryException) :
    CapacityAllocator<TYPE>(size, granularity) {}

  /**
    Initializes the allocator by copying from the specified allocator. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  ReferenceCountedCapacityAllocator(const ReferenceCountedCapacityAllocator& copy) throw(MemoryException) :
    CapacityAllocator<TYPE>(copy) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
