/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/mem/Heap.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // Unix
  #include <stdlib.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void* HeapImpl::allocate(unsigned int size) throw(MemoryException) {
  void* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  result = static_cast<void*>(HeapAlloc(GetProcessHeap(), 0, size));
#else // Unix
  result = malloc(size);
#endif
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to allocate heap");
  }
  return result;
}

void* HeapImpl::resize(void* heap, unsigned int size) throw(MemoryException) {
  void* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  // is serialization enabled for the heap object returned by GetProcessHeap
  if (heap) {
    if (size) {
      result = static_cast<void*>(HeapReAlloc(GetProcessHeap(), 0, heap, size));
    } else {
      if (!HeapFree(GetProcessHeap(), 0, heap)) {
        throw MemoryException("Unable to resize heap");
      }
      result = 0;
    }
  } else {
    result = static_cast<void*>(HeapAlloc(GetProcessHeap(), 0, size));
  }
#else // Unix
  result = realloc(heap, size);
#endif
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to resize heap");
  }
  return result;
}

void* HeapImpl::tryResize(void* heap, unsigned int size) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (heap) {
    if (size) {
      return static_cast<void*>(HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY, heap, size));
    } else {
      if (!HeapFree(GetProcessHeap(), 0, heap)) {
        throw MemoryException("Unable to resize heap");
      }
      return 0;
    }
  } else {
    return 0;
  }
#else // Unix
  return 0;
#endif
}

void HeapImpl::release(void* heap) throw(MemoryException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!HeapFree(GetProcessHeap(), 0, heap)) {
    throw MemoryException("Unable to release heap");
  }
#else // Unix
  free(heap);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
