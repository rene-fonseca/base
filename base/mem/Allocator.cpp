/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <stdlib.h>
#include <string.h>

AllocatorImpl::AllocatorImpl() throw() : size(0), allocated(0) {
}

AllocatorImpl::AllocatorImpl(unsigned int size) throw(MemoryException) {
  this->size = size;
  allocated = malloc(size);
  if ((!allocated) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
}

AllocatorImpl::AllocatorImpl(const AllocatorImpl& copy) throw(MemoryException) : size(copy.size) {
  allocated = malloc(size);
  if ((!allocated) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  memcpy(allocated, copy.allocated, size); // memory blocks do not overlap
}

AllocatorImpl& AllocatorImpl::operator=(const AllocatorImpl& eq) throw(MemoryException) {
  if (&eq != this) {
    size = eq.size;
    allocated = malloc(size);
    if ((!allocated) && (size != 0)) { // was memory allocated
      throw MemoryException();
    }
    memcpy(allocated, eq.allocated, size); // memory blocks do not overlap
  }
  return *this;
}

void AllocatorImpl::setSize(unsigned int size) throw(MemoryException) {
  if (size != this->size) { // do we really have to
    this->size = size;
    void* result = realloc(allocated, size);
    if ((!result) && (size != 0)) { // was memory allocated
      throw MemoryException();
    }
    allocated = result;
  }
}

AllocatorImpl::~AllocatorImpl() throw() {
  free(allocated);
}
