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

unsigned int BufferedInputStream::read(char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  unsigned int bytesRead = 0; // number of bytes that have been copied into external buffer
  while (true) {
    // copy from internal to external buffer - no overlap
    unsigned int bytesToCopy = minimum(size - bytesRead, count - position);
    copy(buffer + bytesRead, getBuffer() + this->position, bytesToCopy);
    bytesRead += bytesToCopy;
    position += bytesToCopy;

    if (bytesRead >= size) { // have we read all the bytes we were asked to read
      break;
    }

    position = 0;
    count = FilterInputStream::read(getBuffer(), getSize(), true); // refill of internal buffer with blocking
    if (count == 0) {
      if (!nonblocking) {
        count = FilterInputStream::read(getBuffer(), 1, false); // refill of internal buffer with blocking
      }
      if (count == 0) {
        break; // prevent infinite loop - we have tried to fill the buffer prior to this
      }
    }
  }
  return bytesRead;
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
