/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "BufferedInputStream.h"
#include <string.h>

void BufferedInputStream::fillBuffer() throw(IOException) {
  if (position >= count) {
    count = FilterInputStream::read(buffer, size);
    position = 0;
  }
}

BufferedInputStream::BufferedInputStream(InputStream* in, unsigned int size) :
  FilterInputStream(in) {
  this->size = size > MINIMUM_BUFFER_SIZE ? size : MINIMUM_BUFFER_SIZE;
  buffer = new char[this->size];
  count = 0;
  position = 0;
}

unsigned int BufferedInputStream::available() {
  return (count - position) + FilterInputStream::available();
}

int BufferedInputStream::read() throw(IOException) {
  if (position >= count) {
    fillBuffer();
  }
  return buffer[position++];
}

unsigned int BufferedInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  unsigned int position = 0; // position in destination buffer

  // need to prevent infinite loop!

  while (position < size) { // have we read all the bytes
    if (this->position >= this->count) {
      fillBuffer();
    }

    unsigned int bytes;
    if (size - position > this->count - this->position) { // bytes to copy
      bytes = this->count - this->position;
    } else {
      bytes = size - position;
    }

    memcpy(&buffer[position], &this->buffer[this->position], bytes);
    position += bytes;
    this->position += bytes;
  }
  return size;
}

void BufferedInputStream::skip(unsigned int count) throw(IOException) {
  // need to prevent infinite loop!
  while (count > 0) {
    if (this->position >= this->count) {
      fillBuffer();
    }
    unsigned int bytes;
    if (count > this->count - this->position) { // bytes to skip
      bytes = this->count - this->position;
    } else {
      bytes = count;
    }
    this->position += bytes;
    count -= bytes;
  }
}

BufferedInputStream::~BufferedInputStream() {
  delete[] buffer;
}
