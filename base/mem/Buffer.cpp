/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Buffer.h"
#include <string.h>
#include <stdlib.h>

Buffer::Buffer() throw() {
  managed = true;
  size = 0;
  bytes = NULL;
}

Buffer::Buffer(unsigned int size) throw(MemoryException) {
  managed = true;
  this->size = size;
  bytes = (char*)malloc(size);
  if ((bytes == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
}

Buffer::Buffer(unsigned int size, char value) throw(MemoryException) {
  managed = true;
  this->size = size;
  bytes = (char*)malloc(size);
  if ((bytes == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  fill(value, 0, size); // initialize all bytes
}

Buffer::Buffer(char* bytes, unsigned int size) throw(MemoryException) {
  managed = false;
  if ((bytes == NULL) && (size > 0)) { // do we have a conflict
    throw MemoryException();
  }
  this->size = size;
  this->bytes = bytes;
}

Buffer::Buffer(const Buffer& obj) throw(MemoryException) {
  managed = true;
  size = obj.getSize();
  bytes = (char*)malloc(size);
  if ((bytes == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  copy(obj, 0, size); // copy from obj to this buffer
}

void Buffer::setSize(unsigned int size) throw(MemoryException) {
  if (!managed) {
    throw MemoryException(); // cannot resize unmanaged memory
  }
  if (getSize() != size) { // do we really have to
    this->size = size;
    char* result = (char*)realloc(bytes, size);
    if ((result == NULL) && (size > 0)) { // was memory allocated
      throw MemoryException();
    }
    bytes = result;
  }
}

void Buffer::fill(char value, unsigned int offset, unsigned int count) throw(OutOfRange) {
  if ((offset >= getSize()) || ((unsigned long long)offset + count >= getSize())) {
    throw OutOfRange();
  }
  memset(&bytes[offset], value, count);
}

void Buffer::copy(const char* src, unsigned int offset, unsigned int count) throw(OutOfRange) {
  if ((offset >= getSize()) || ((unsigned long long)offset + count >= getSize())) {
    throw OutOfRange();
  }
  if (count == 0) {
    return; // nothing to do
  }
  if ((&src[count - 1] < &bytes[offset]) || (src > &bytes[offset + count - 1])) {
    memcpy(&bytes[offset], src, count); // memory blocks do not overlap
  } else {
    memmove(&bytes[offset], src, count); // memory blocks overlap
  }
}

Buffer::~Buffer() throw() {
  if (managed) {
    if (bytes) {
      free(bytes);
    }
  }
}
