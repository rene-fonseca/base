/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/mem/Buffer.h>
#include <string.h>
#include <stdlib.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#define NUMBER_TO_GRAN(NUMBER, GRAN) ((((NUMBER) + (GRAN) - 1)/(GRAN)) * (GRAN))

Buffer::Buffer(unsigned int size, unsigned int granularity) throw(MemoryException) {
  this->granularity = (granularity > 0) ? granularity : 1;
//  this->size = NUMBER_TO_GRAN(size, granularity);
  bytes = static_cast<char*>(malloc(this->size));
  if ((bytes == 0) && (this->size != 0)) { // was memory allocated
    throw MemoryException(this);
  }
}

Buffer::Buffer(const Buffer& copy) throw(MemoryException) {
  granularity = copy.granularity;
  size = copy.size;
  bytes = static_cast<char*>(malloc(size));
  if ((bytes == 0) && (size != 0)) { // was memory allocated
    throw MemoryException(this);
  }
  memcpy(bytes, copy.bytes, size); // copy buffer
}

void Buffer::setSize(unsigned int size) throw(MemoryException) {
//  size = NUMBER_TO_GRAN(size, granularity);
  if (size != this->size) { // do we really have to
    this->size = size;
    char* result = static_cast<char*>(realloc(bytes, size));
    if ((result == 0) && (size > 0)) { // was memory allocated
      throw MemoryException(this);
    }
    bytes = result;
  }
}

Buffer::~Buffer() throw() {
  ::free(bytes);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
