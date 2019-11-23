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
#include <base/mem/Allocator.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe and the class is
  mainly used by other container classes.

  @short Reference counted allocator.
  @ingroup memory
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedAllocator : public ReferenceCountedObject, public Allocator<TYPE> {
private:

  /**
    Default assignment is prohibited.
  */
  ReferenceCountedAllocator& operator=(const ReferenceCountedAllocator& assign) throw();
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit ReferenceCountedAllocator() noexcept
  {
  }

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Raises MemoryException if unable to allocate enough memory to
    hold the requested number of elements.

    @param size Specifies the initial size of the allocator.
  */
  inline explicit ReferenceCountedAllocator(MemorySize size) throw(MemoryException)
    : Allocator<TYPE>(size)
  {
  }

#if 0
  inline explicit ReferenceCountedAllocator(MemorySize size, MemorySize capacity) throw(MemoryException)
    : Allocator<TYPE>(size, capacity)
  {
  }
#endif
  
  /**
    Initializes the allocator by copying from the specified allocator. Raises
    MemoryException if unable to allocate the required memory.
  */
  inline ReferenceCountedAllocator(const ReferenceCountedAllocator& copy) throw(MemoryException)
    : Allocator<TYPE>(copy)
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
