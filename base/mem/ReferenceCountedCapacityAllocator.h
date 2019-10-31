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

#include <base/Base.h>
#include <base/MemoryException.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @short Reference counted capacity allocator.
  @ingroup memory
  @see CapacityAllocator
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedCapacityAllocator : public ReferenceCountedObject, public CapacityAllocator<TYPE> {
private:

  /**
    Default assignment is prohibited.
  */
  ReferenceCountedCapacityAllocator& operator=(const ReferenceCountedCapacityAllocator&) noexcept = delete;
  ReferenceCountedCapacityAllocator& operator=(const ReferenceCountedCapacityAllocator&&) noexcept = delete;
public:

  /**
    Initializes an empty allocator with the default granularity.
  */
  inline explicit ReferenceCountedCapacityAllocator() noexcept
    : CapacityAllocator<TYPE>()
  {
  }

  /**
    Initializes an empty allocator.

    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline explicit ReferenceCountedCapacityAllocator(MemorySize granularity) throw(OutOfRange)
    : CapacityAllocator<TYPE>(granularity)
  {
  }
  
  /**
    Initializes an allocator of the specified size without initializing the
    elements. Raises MemoryException if unable to allocate enough memory to
    hold the requested number of elements. Raises OutOfRange if granularity
    is less than MINIMUM_GRANULARITY.

    @param size Specifies the initial size of the allocator.
    @param granularity Specifies the number of elements to allocate at a time.
  */
  inline ReferenceCountedCapacityAllocator(
    MemorySize size,
    MemorySize granularity) throw(OutOfRange, MemoryException)
    : CapacityAllocator<TYPE>(size, granularity)
  {
  }

  inline ReferenceCountedCapacityAllocator(
    MemorySize size,
    MemorySize capacity,
    MemorySize granularity) throw(OutOfRange, MemoryException)
    : CapacityAllocator<TYPE>(size, capacity, granularity)
  {
  }

  /**
    Initializes the allocator by copying from the specified allocator. Raises
    MemoryException if unable to allocate the required memory.
  */
  ReferenceCountedCapacityAllocator(const ReferenceCountedCapacityAllocator& copy) throw(MemoryException)
    : CapacityAllocator<TYPE>(copy)
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
