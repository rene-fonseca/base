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

void* AllocatorImpl::setSize(void* buffer, unsigned int size) throw(MemoryException) {
  // Allocator::setSize() checks if we really have to set the size
#if defined(__win32__)
  // use HeapCreate, HeapReAlloc, and HeapDestroy with serialization enabled
  void* result = (void*)LocalReAlloc((HLOCAL)buffer, size, 0);
#else // __unix__
  void* result = realloc(buffer, size);
#endif
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  return result;
}
