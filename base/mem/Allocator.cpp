/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <stdlib.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void* AllocatorImpl::setSize(void* buffer, unsigned int size) throw(MemoryException) {
  // Allocator::setSize() checks if we really have to set the size
  void* result;
#if defined(__win32__)
  // is serialization enabled for the heap object returned by GetProcessHeap
  if (buffer) {
    if (size) {
      result = (void*)HeapReAlloc(GetProcessHeap(), 0, buffer, size);
    } else {
      if (!HeapFree(GetProcessHeap(), 0, buffer)) {
        throw MemoryException("Unable to release memory");
      }
      result = 0;
    }
  } else {
    result = (void*)HeapAlloc(GetProcessHeap(), 0, size);
  }
  //  result = (void*)LocalReAlloc((HLOCAL)buffer, size, 0);
#else // __unix__
  result = realloc(buffer, size);
#endif
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to adjust allocated memory");
  }
  return result;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
