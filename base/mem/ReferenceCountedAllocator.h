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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @short Reference counted allocator.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedAllocator : public ReferenceCountedObject, public Allocator<TYPE> {
private:

  /**
    Default assignment is prohibited.
  */
  ReferenceCountedAllocator& operator=(const ReferenceCountedAllocator& eq);
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit ReferenceCountedAllocator() throw() {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements.

    @param size Specifies the initial size of the allocator.
  */
  inline explicit ReferenceCountedAllocator(unsigned int size) throw(MemoryException) : Allocator<TYPE>(size) {}

  /**
    Initializes the allocator by copying from the specified allocator. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  ReferenceCountedAllocator(const ReferenceCountedAllocator& copy) throw(MemoryException) : Allocator<TYPE>(copy) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
