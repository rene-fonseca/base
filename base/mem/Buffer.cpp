/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Buffer.h"
#include <string.h>
#include <stdlib.h>

#define NUMBER_TO_GRAN(NUMBER, GRAN) ((((NUMBER) + (GRAN) - 1)/(GRAN)) * (GRAN))

Buffer::Buffer(unsigned int size, unsigned int granularity) throw(MemoryException) {
  this->granularity = (granularity > 0) ? granularity : 1;
  this->size = NUMBER_TO_GRAN(size, granularity);
  bytes = (char*)malloc(size);
  if ((bytes == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
}

Buffer::Buffer(const Buffer& copy) throw(MemoryException) {
  granularity = copy.granularity;
  size = copy.size;
  bytes = (char*)malloc(size);
  if ((bytes == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  memcpy(bytes, copy.bytes, size); // copy buffer
}

void Buffer::setSize(unsigned int size) throw(MemoryException) {
  size = NUMBER_TO_GRAN(size, granularity);
  if (size != this->size) { // do we really have to
    this->size = size;
    char* result = (char*)realloc(bytes, size);
    if ((result == NULL) && (size > 0)) { // was memory allocated
      throw MemoryException();
    }
    bytes = result;
  }
}

Buffer::~Buffer() throw() {
  ::free(bytes);
}
