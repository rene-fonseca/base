/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/Heap.h>
#include <base/OperatingSystem.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <stdlib.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
namespace internal {
  namespace specific {
    extern OperatingSystem::Handle processHeap;
  };
};
#endif // flavour

void* HeapImpl::allocate(unsigned int size) throw(MemoryException) {
  void* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
#else // unix
  result = malloc(size);
#endif // flavour
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to allocate heap", Type::getType<HeapImpl>());
  }
  return result;
}

void* HeapImpl::resize(void* heap, unsigned int size) throw(MemoryException) {
  void* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // is serialization enabled for the heap object returned by GetProcessHeap
  if (heap) {
    if (size) {
      result = static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, 0, heap, size));
    } else {
      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw MemoryException("Unable to resize heap", Type::getType<HeapImpl>());
      }
      result = 0;
    }
  } else {
    result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  }
#else // unix
  result = realloc(heap, size);
#endif // flavour
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to resize heap", Type::getType<HeapImpl>());
  }
  return result;
}

void* HeapImpl::tryResize(void* heap, unsigned int size) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (heap) {
    if (size) {
      return static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, HEAP_REALLOC_IN_PLACE_ONLY, heap, size));
    } else {
      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw MemoryException("Unable to resize heap", Type::getType<HeapImpl>());
      }
      return 0;
    }
  } else {
    return 0;
  }
#else // unix
  return 0;
#endif // flavour
}

void HeapImpl::release(void* heap) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
    throw MemoryException("Unable to release heap", Type::getType<HeapImpl>());
  }
#else // unix
  free(heap);
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
