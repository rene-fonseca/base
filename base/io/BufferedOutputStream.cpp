/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "BufferedOutputStream.h"
#include <string.h>

template<class TYPE> inline TYPE min(TYPE left, TYPE right) {return (left <= right) ? left : right;};
template<class TYPE> inline TYPE max(TYPE left, TYPE right) {return (left >= right) ? left : right;};

BufferedOutputStream::BufferedOutputStream(OutputStream& out, unsigned int size) throw(BindException) :
  FilterOutputStream(out) {
  this->size = max(size, MINIMUM_BUFFER_SIZE);
  buffer = new char[this->size];
  if (buffer == NULL) {
    throw BindException();
  }
  count = 0;
}

void BufferedOutputStream::flush() throw(IOException) {
  FilterOutputStream::write(buffer, count);
  count = 0;
  FilterOutputStream::flush();
}

unsigned int BufferedOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {

  unsigned int bytesLeft = this->size - count; // bytes left in internal buffer

  if (size <= bytesLeft) { // do we have enough space left in the internal buffer
    memcpy(this->buffer + count, buffer, size); // copy from external to internal
    return size;
  }

  unsigned int result; // used to hold the actual bytes written to the stream
  unsigned int position = 0; // specifies the position in the external buffer

  // fill internal buffer and write it to the stream
  if (bytesLeft > 0) {
    memcpy(this->buffer + count, buffer, bytesLeft); // copy from external to internal
    count += bytesLeft;
    position += bytesLeft;
  }
  result = FilterOutputStream::write(this->buffer, count); // write internal buffer
  count -= result;
  if (count > 0) { // did we empty the internal buffer
    if (result > 0) {
      memmove(this->buffer, this->buffer + result, count); // move bytes to beginning of buffer
    }
    return position; // # of bytes read from external buffer
  }

  unsigned int bytesToCopy = (size - position) % this->size; // # of bytes to copy to internal buffer
  unsigned int bytesToWrite = (size - position) - bytesToCopy; // # of bytes to write from external buffer

  // write directly from external buffer to stream
  if (bytesToWrite > 0) {
    result = FilterOutputStream::write(buffer + position, bytesToWrite);
    if (result < bytesToWrite) {
      return position + result; // # of bytes read from external buffer
    }
    position += bytesToWrite;
  }

  // copy rest from external buffer to internal buffer
  memcpy(this->buffer, buffer + position, bytesToCopy); // copy external to internal
  count += bytesToCopy;
  return size; // # of bytes read from external buffer
}

BufferedOutputStream::~BufferedOutputStream() {
  delete[] buffer;
}
