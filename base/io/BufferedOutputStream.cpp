/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "BufferedOutputStream.h"
#include <string.h>

void BufferedOutputStream::flushBuffer() {
  FilterOutputStream::write(buffer, count);
  count = 0;
}

BufferedOutputStream::BufferedOutputStream(OutputStream* out, unsigned int size) : FilterOutputStream(out) {
  this->size = size > MINIMUM_BUFFER_SIZE ? size : MINIMUM_BUFFER_SIZE;
  buffer = new char[this->size];
  count = 0;
}

void BufferedOutputStream::flush() throw(IOException) {
  flushBuffer();
  FilterOutputStream::flush();
}

void BufferedOutputStream::write(char value) throw(IOException) {
  if (count >= size) {
    flushBuffer();
  }
  buffer[count++] = value;
}

void BufferedOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
  unsigned int bytesToCopy;
  if (size > this->size - count) { // do we have enough space left in the buffer
    bytesToCopy = size % this->size;
  } else {
    bytesToCopy = size;
  }

  unsigned int bytesToWrite = size - bytesToCopy;
  if (bytesToWrite > 0) {
    flushBuffer();
    FilterOutputStream::write(buffer, bytesToWrite);
  }

  memcpy(&this->buffer[count], &buffer[bytesToWrite], bytesToCopy);
  count += bytesToCopy;
}

BufferedOutputStream::~BufferedOutputStream() {
  delete[] buffer;
}
