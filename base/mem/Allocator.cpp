/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <stdlib.h>

void* AllocatorImpl::setSize(void* buffer, unsigned int size) throw(MemoryException) {
  // Allocator::setSize() checks if we really have to set the size
  void* result = realloc(buffer, size);
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  return result;
}
