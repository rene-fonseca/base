/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/BufferedOutputStream.h>
#include <base/Trace.h>
#include <string.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

BufferedOutputStream::BufferedOutputStream(OutputStream& out, unsigned int size) throw(BindException, MemoryException) :
  FilterOutputStream(out), buffer(maximum(size, MINIMUM_BUFFER_SIZE)) {
  count = 0;
}

void BufferedOutputStream::flush() throw(IOException) {
  FilterOutputStream::write(getBuffer(), count);
  count = 0;
  FilterOutputStream::flush();
}

unsigned int BufferedOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {

  unsigned int bytesLeft = getSize() - count; // bytes left in internal buffer

  if (size <= bytesLeft) { // do we have enough space left in the internal buffer
    memcpy(getBuffer() + count, buffer, size); // copy from external to internal
    count += size;
    return size;
  }

  unsigned int result; // used to hold the actual bytes written to the stream
  unsigned int position = 0; // specifies the position in the external buffer

  // fill internal buffer and write it to the stream
  if (bytesLeft > 0) {
    memcpy(getBuffer() + count, buffer, bytesLeft); // copy from external to internal
    count += bytesLeft;
    position += bytesLeft;
  }
  result = FilterOutputStream::write(getBuffer(), count); // write internal buffer
  count -= result;
  if (count > 0) { // did we empty the internal buffer
    if (result > 0) {
      memmove(getBuffer(), getBuffer() + result, count); // move bytes to beginning of buffer
    }
    return position; // # of bytes read from external buffer
  }

  unsigned int bytesToCopy = (size - position) % getSize(); // # of bytes to copy to internal buffer
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
  memcpy(getBuffer(), buffer + position, bytesToCopy); // copy external to internal
  count += bytesToCopy;
  return size; // # of bytes read from external buffer
}

BufferedOutputStream::~BufferedOutputStream() {
  TRACE_MEMBER();
  flush();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
