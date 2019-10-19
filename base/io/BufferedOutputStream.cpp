/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/BufferedOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

BufferedOutputStream::BufferedOutputStream(
  OutputStream& out, unsigned int size) throw(BindException, MemoryException)
  : FilterOutputStream(out),
    buffer(maximum(size, MINIMUM_BUFFER_SIZE)),
    writeHead(0),
    readHead(0) {
}

void BufferedOutputStream::flush() throw(IOException) {
  auto size = writeHead - readHead;
  if (size > 0) {
    FilterOutputStream::write(buffer.getElements() + readHead, size);
  }
  readHead = 0;
  writeHead = 0;
  FilterOutputStream::flush();
}

unsigned int BufferedOutputStream::write(const uint8* buffer,
                                         unsigned int size,
                                         bool nonblocking) throw(IOException)
{
  unsigned int bytesWritten = 0; // number of bytes that have been written
  while (true) {
    // copy from external to internal buffer - no overlap
    unsigned int bytesToCopy = minimum<MemorySize>(size - bytesWritten, this->buffer.getSize() - writeHead);
    copy<uint8>(this->buffer.getElements() + writeHead, buffer, bytesToCopy);
    bytesWritten += bytesToCopy;
    writeHead += bytesToCopy;
    buffer += bytesToCopy;

    if (bytesWritten >= size) { // have we written all the bytes we were asked to write
      break;
    }

    unsigned int result = FilterOutputStream::write(
      this->buffer.getElements() + readHead,
      writeHead - readHead,
      nonblocking
    );
    readHead += result;
    if (readHead == writeHead) {
      readHead = 0;
      writeHead = 0;
    }
    if (result == 0) {
      break; // prevent infinite loop
    }
  }
  return bytesWritten;
}

void BufferedOutputStream::unfoldValue(
  char value,
  unsigned int size) throw(IOException) {
  while (true) {
    unsigned int bytesAvailable =
      minimum<MemorySize>(size, this->buffer.getSize() - writeHead);
    fill<uint8>(this->buffer.getElements() + writeHead, bytesAvailable, value);
    size -= bytesAvailable;
    writeHead += bytesAvailable;

    if (size == 0) { // have we written all the bytes we were asked to write
      break;
    }

    FilterOutputStream::write(this->buffer.getElements() + readHead, writeHead - readHead);
    readHead = 0;
    writeHead = 0;
  }
}

BufferedOutputStream::~BufferedOutputStream() {
  flush();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
