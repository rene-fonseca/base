/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/BufferedOutputStream.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

BufferedOutputStream::BufferedOutputStream(OutputStream& out, unsigned int size) throw(BindException, MemoryException) :
  FilterOutputStream(out),
  buffer(maximum(size, MINIMUM_BUFFER_SIZE)),
  writeHead(0),
  readHead(0) {
}

void BufferedOutputStream::flush() throw(IOException) {
  FilterOutputStream::write(buffer.getElements() + readHead, writeHead - readHead);
  readHead = 0;
  writeHead = 0;
  FilterOutputStream::flush();
}

unsigned int BufferedOutputStream::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  unsigned int bytesWritten = 0; // number of bytes that have been written
  while (true) {
    // copy from external to internal buffer - no overlap
    unsigned int bytesToCopy = minimum(size - bytesWritten, this->buffer.getSize() - writeHead);
    copy(this->buffer.getElements() + writeHead, buffer + bytesWritten, bytesToCopy);
    bytesWritten += bytesToCopy;
    writeHead += bytesToCopy;

    if (bytesWritten >= size) { // have we written all the bytes we were asked to write
      break;
    }

    unsigned int result = FilterOutputStream::write(this->buffer.getElements() + readHead, writeHead - readHead, nonblocking);
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

void BufferedOutputStream::unfoldValue(char value, unsigned int size) throw(IOException) {
  while (true) {
    unsigned int bytesAvailable = minimum(size, this->buffer.getSize() - writeHead);
    fill(this->buffer.getElements() + writeHead, bytesAvailable, value);
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

BufferedOutputStream::~BufferedOutputStream() throw(IOException) {
  TRACE_MEMBER();
  flush();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
