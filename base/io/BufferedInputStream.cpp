/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/BufferedInputStream.h>
#include <base/Base.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

BufferedInputStream::BufferedInputStream(InputStream& in, unsigned int size) throw(BindException, MemoryException) :
  FilterInputStream(in), buffer(maximum(size, MINIMUM_BUFFER_SIZE)) {
  count = 0;
  position = 0;
}

unsigned int BufferedInputStream::available() const throw(IOException) {
  return (count - position) + FilterInputStream::available();
}

unsigned int BufferedInputStream::read(char* buffer, unsigned int size) throw(IOException) {

  if (isEmpty()) {
    this->count = FilterInputStream::read(getBuffer(), getSize()); // checked for zero later
    this->position = 0;
  }

  unsigned int bytesInBuffer = this->count - this->position;

  if (size <= bytesInBuffer) { // does the internal buffer hold the requested bytes
    copy(buffer, getBuffer() + this->position, size); // copy from internal to external buffer - no overlap
    this->position += size;
    return size; // everything was read
  }

  unsigned int position = 0; // position in external buffer
  while (true) {
    unsigned int bytes = minimum(size - position, this->count - this->position); // bytes to copy

    if (bytes == 0) { // is internal buffer empty
      break; // prevent infinite loop - we have tried to fill the buffer prior to this
    }

    copy(buffer + position, getBuffer() + this->position, bytes); // from internal to external - no overlap
    position += bytes;
    this->position += bytes;

    if (position >= size) { // have we read all the bytes we were asked to read
      break;
    }

    if (isEmpty()) {
      this->count = FilterInputStream::read(getBuffer(), getSize());
      this->position = 0;
    }
  }
  return position; // number of bytes that have been copied into the external buffer
}

unsigned int BufferedInputStream::skip(unsigned int count) throw(IOException) {
  if (isEmpty()) {
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

void BufferedInputStream::wait() const throw(IOException) {
  if (isEmpty()) {
    FilterInputStream::wait();
  }
}

bool BufferedInputStream::wait(unsigned int timeout) const throw(IOException) {
  if (isEmpty()) {
    return FilterInputStream::wait(timeout);
  } else {
    return true; // data is available in buffer
  }
}

BufferedInputStream::~BufferedInputStream() {
  TRACE_MEMBER();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
