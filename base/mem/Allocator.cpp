/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <stdlib.h>
#include <string.h>

Allocator::Allocator(unsigned int size) throw(MemoryException) {
  this->size = size;
  memory = (char*)malloc(size);
  if ((memory == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
}

Allocator::Allocator(const Allocator& copy) throw(MemoryException) : size(copy.size) {
  memory = (char*)malloc(size);
  if ((memory == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  memcpy(memory, copy.memory, size);
}

void Allocator::setSize(unsigned int size) throw(MemoryException) {
  if (size != this->size) { // do we really have to
    this->size = size;
    char* result = (char*)realloc(memory, size);
    if ((result == NULL) && (size > 0)) { // was memory allocated
      throw MemoryException();
    }
    memory = result;
  }
}

Allocator::~Allocator() throw() {
  free(memory);
}
