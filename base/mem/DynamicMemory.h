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

#include <base/Primitives.h>
#include <base/Base.h>
#include <base/MemoryException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides support for dynamic memory allocation/deallocation. Compatible with Heap class.

  @short Dynamic memory.
  @ingroup memory
  @see DebugDynamicMemory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DynamicMemory {
public:

  /**
    Allocates the specified number of bytes on the heap. The behavior is
    unspecified if the desired size is 0.

    @param size The desired size of the memory block in number of bytes.

    @return nullptr if not successful.
  */
  static void* allocate(MemorySize size) noexcept;

  /**
    Frees the specified memory block previously allocated by allocate(). Does
    nothing if the pointer is 0.
    
    @param memory The memory block.
  */
  static void release(void* memory) noexcept;

  /**
    Dynamic memory placement operator.
  */
  static inline void* operator new(MemorySize size, void* place) noexcept
  {
    BASSERT(size > 0);
    BASSERT(place);
    return place;
  }
  
  /**
    Dynamic memory placement operator.
  */
  static inline void operator delete(void* memory, void* place) noexcept
  {
    BASSERT(memory);
  }

  /**
    Dynamic memory placement operator for arrays.
  */
  static inline void* operator new[](MemorySize size, void* place) noexcept
  {
    BASSERT(size > 0);
    BASSERT(place);
    return place;
  }
  
  /**
    Releases dynamic memory previously allocated by new[].
  */
  static inline void operator delete[](void* memory, void* place) noexcept
  {
    BASSERT(memory);
  }

  /**
    Allocates a block of dynamic memory of the specified size in bytes. Raises
    MemoryException if the requested size is 0.
  */
  static void* operator new(MemorySize size);
  
  /**
    Releases a dynamic memory block previously allocated by new. Raises
    MemoryException if memory is 0.
  */
  static void operator delete(void* memory);

  /**
    Allocates a block of dynamic memory for an array of the specified size in
    bytes. Raises MemoryException if the requested size is 0.
  */
  static void* operator new[](MemorySize size);

  /**
    Releases dynamic memory previously allocated by new[]. Raises
    MemoryException if memory is 0.
  */
  static void operator delete[](void* memory);
};

#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD) // should also be set when building static
#  define _COM_AZURE_DEV__BASE__NO_ARRAY_ALLOCATION
#endif

/** Override memory allocation for a class. */
#if !defined(_COM_AZURE_DEV__BASE__NO_ARRAY_ALLOCATION)
#define _COM_AZURE_DEV__BASE__OVERRIDE_ALLOC() \
  static inline void* operator new(MemorySize size, void* place) noexcept \
    {return base::DynamicMemory::operator new(size, place);} \
  static inline void operator delete(void* memory, void* place) noexcept \
    {base::DynamicMemory::operator delete(memory, place); } \
  static inline void* operator new[](MemorySize size, void* place) noexcept \
    {return base::DynamicMemory::operator new[](size, place);} \
  static inline void operator delete[](void* memory, void* place) noexcept \
    {base::DynamicMemory::operator delete[](memory, place);} \
  static inline void* operator new(MemorySize size) \
    {return base::DynamicMemory::operator new(size);} \
  static inline void operator delete(void* memory) \
    {base::DynamicMemory::operator delete(memory);} \
  static inline void* operator new[](MemorySize size) \
    {return base::DynamicMemory::operator new[](size);} \
  static inline void operator delete[](void* memory) \
    {base::DynamicMemory::operator delete[](memory);}
#else
#define _COM_AZURE_DEV__BASE__OVERRIDE_ALLOC() \
  static inline void* operator new(MemorySize size, void* place) noexcept \
    {return base::DynamicMemory::operator new(size, place);} \
  static inline void operator delete(void* memory, void* place) noexcept \
    {base::DynamicMemory::operator delete(memory, place); } \
  static void* operator new[](MemorySize size, void* place) noexcept = delete; \
  static void operator delete[](void* memory, void* place) noexcept = delete; \
  static inline void* operator new(MemorySize size) \
    {return base::DynamicMemory::operator new(size);} \
  static inline void operator delete(void* memory) \
    {base::DynamicMemory::operator delete(memory);} \
  static void* operator new[](MemorySize size) = delete; \
  static void operator delete[](void* memory) = delete;
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

// global new and delete operators - using inline only to allow different implementations
#if (!defined(_COM_AZURE_DEV__BASE__DEFAULT_MEMORY) && \
      defined(_COM_AZURE_DEV__BASE__NAMESPACE))
inline void* operator new(base::MemorySize size, void* place) noexcept
{
  BASSERT(size > 0);
  return place;
}

inline void operator delete(void* memory, void* place) noexcept
{
  BASSERT(memory);
}

#if !defined(_COM_AZURE_DEV__BASE__NO_ARRAY_ALLOCATION)
inline void* operator new[](base::MemorySize size, void* place) noexcept
{
  BASSERT(size > 0);
  return place;
}

inline void operator delete[](void* memory, void* place) noexcept
{
  BASSERT(memory);
}
#else
void* operator new[](base::MemorySize size, void* place) noexcept = delete;
void operator delete[](void* memory, void* place) noexcept = delete;
#endif

inline void* operator new(base::MemorySize size)
{
  return base::DynamicMemory::operator new(size);
}

inline void operator delete(void* memory)
{
  base::DynamicMemory::operator delete(memory);
}

#if !defined(_COM_AZURE_DEV__BASE__NO_ARRAY_ALLOCATION)
inline void* operator new[](base::MemorySize size)
{
  return base::DynamicMemory::operator new[](size);
}

inline void operator delete[](void* memory)
{
  base::DynamicMemory::operator delete[](memory);
}
#else
void* operator new[](base::MemorySize size) = delete;
void operator delete[](void* memory) = delete;
#endif

// also add const std::nothrow_t&

#endif
