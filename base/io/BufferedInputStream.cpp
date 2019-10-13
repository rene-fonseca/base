/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/BufferedInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Base.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

BufferedInputStream::BufferedInputStream(
  InputStream& in, unsigned int size) throw(BindException, MemoryException)
  : FilterInputStream(in),
    buffer(maximum(size, MINIMUM_BUFFER_SIZE)),
    count(0),
    position(0) {
}

unsigned int BufferedInputStream::available() const throw(IOException) {
  return (count - position) + FilterInputStream::available();
}

unsigned int BufferedInputStream::read(
  uint8* buffer,
  unsigned int size,
  bool nonblocking) throw(IOException) {
  unsigned int bytesRead = 0; // number of bytes that have been copied into external buffer
  while (true) {
    // copy from internal to external buffer - no overlap
    unsigned int bytesToCopy = minimum(size - bytesRead, count - position);
    copy<uint8>(
      buffer,
      this->buffer.getElements() + this->position,
      bytesToCopy
    );
    bytesRead += bytesToCopy;
    position += bytesToCopy;
    buffer += bytesToCopy;

    if (bytesRead >= size) { // have we read all the bytes we were asked to read
      break;
    }

    position = 0;
    unsigned int available = FilterInputStream::available();
    if (available > 0) {
      count = FilterInputStream::read(
        this->buffer.getElements(),
        minimum<MemorySize>(this->buffer.getSize(), available)
      ); // refill of internal buffer without blocking
      ASSERT(count == available);
    } else if (nonblocking) {
      break;
    } else { // blocking
      FilterInputStream::wait(); // TAG: what about EOF
      unsigned int available = FilterInputStream::available();
      count = FilterInputStream::read(
        this->buffer.getElements(),
        minimum<MemorySize>(this->buffer.getSize(), available)
      ); // refill of internal buffer without blocking
      ASSERT((count > 0) && (count == available));
    }
  }
  return bytesRead;
}

unsigned int BufferedInputStream::peek(
  unsigned int count) throw(OutOfDomain, IOException) {
  unsigned int unreadBytes = this->count - position;
  if (count > unreadBytes) { // request for more than currently available in this buffer
    bassert(count <= buffer.getSize(), OutOfDomain());
    move(buffer.getElements(), buffer.getElements() + position, unreadBytes); // move unread elements to beginning of buffer
    position = 0;
    this->count -= position;
    // only read count bytes in blocking mode
    this->count += FilterInputStream::read(
      buffer.getElements() + this->count,
      buffer.getSize() - this->count,
      true
    ); // fill internal buffer (non-blocking)
    if (this->count < count) {
      this->count += FilterInputStream::read(
        buffer.getElements() + this->count,
        count - this->count,
        false
      ); // fill internal buffer (blocking)
    }
  }
  ASSERT(position <= this->count);
  return count;
}

unsigned int BufferedInputStream::skip(unsigned int count) throw(IOException) {
  unsigned int unreadBytes = this->count - position;
  if (count > unreadBytes) {
    position = this->count; // skip all bytes in buffer and more
    return unreadBytes + FilterInputStream::skip(count - unreadBytes);
  } else {
    position += count; // skip bytes in buffer
    return count;
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
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
