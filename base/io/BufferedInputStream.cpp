/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "BufferedInputStream.h"
#include <string.h>

template<class TYPE> inline TYPE min(TYPE left, TYPE right) {return (left <= right) ? left : right;};
template<class TYPE> inline TYPE max(TYPE left, TYPE right) {return (left >= right) ? left : right;};

BufferedInputStream::BufferedInputStream(InputStream& in, unsigned int size) throw(BindException) :
  FilterInputStream(in) {
  this->size = max(size, MINIMUM_BUFFER_SIZE);
  buffer = new char[this->size];
  if (buffer == NULL) {
    throw BindException();
  }
  count = 0;
  position = 0;
}

unsigned int BufferedInputStream::available() const throw(IOException) {
  return (count - position) + FilterInputStream::available();
}

unsigned int BufferedInputStream::read(char* buffer, unsigned int size) throw(IOException) {

  if (this->position >= this->count) { // is the internal buffer empty
    this->count = FilterInputStream::read(this->buffer, this->size); // checked for zero later
    this->position = 0;
  }

  unsigned int bytesInBuffer = this->count - this->position;

  if (size <= bytesInBuffer) { // does the internal buffer hold the requested bytes
    memcpy(buffer, this->buffer + this->count, size); // copy from internal to external buffer
    this->position += size;
    return size; // everything was read
  }

  unsigned int position = 0; // position in external buffer
  while (true) {
    unsigned int bytes = min(size - position, this->count - this->position); // bytes to copy

    if (bytes == 0) { // is internal buffer empty
      break; // prevent infinite loop - we have tried to fill the buffer prior to this
    }

    memcpy(buffer + position, this->buffer + this->position, bytes); // from internal to external
    position += bytes;
    this->position += bytes;

    if (position >= size) { // have we read all the bytes we were asked to read
      break;
    }

    if (this->position >= this->count) { // is the internal buffer empty
      this->count = FilterInputStream::read(this->buffer, this->size);
      this->position = 0;
    }
  }
  return position; // number of bytes that have been copied into the external buffer
}

unsigned int BufferedInputStream::skip(unsigned int count) throw(IOException) {
  if (position >= this->count) { // is buffer empty
    return FilterInputStream::skip(count);
  } else {
    unsigned int bytes = this->count - position;
    if (count > bytes) {
      // skip all bytes in buffer and more
      this->count = 0; // empty buffer
      return bytes + FilterInputStream::skip(bytes - count);
    } else {
      // skip bytes in buffer
      position += count;
      return count;
    }
  }
}

BufferedInputStream::~BufferedInputStream() {
  delete[] buffer;
}
