/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/BufferedOutputStream.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

BufferedOutputStream::BufferedOutputStream(OutputStream& out, unsigned int size) throw(BindException, MemoryException) :
  FilterOutputStream(out), buffer(maximum(size, MINIMUM_BUFFER_SIZE)), writeHead(0), readHead(0) {
}

void BufferedOutputStream::flush() throw(IOException) {
  FilterOutputStream::write(getBuffer() + readHead, writeHead - readHead);
  readHead = 0;
  writeHead = 0;
  FilterOutputStream::flush();
}

unsigned int BufferedOutputStream::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  unsigned int bytesWritten = 0; // number of bytes that have been written
  while (true) {
    // copy from external to internal buffer - no overlap
    unsigned int bytesToCopy = minimum(size - bytesWritten, getSize() - writeHead);
    copy(getBuffer() + writeHead, buffer + bytesWritten, bytesToCopy);
    bytesWritten += bytesToCopy;
    writeHead += bytesToCopy;

    if (bytesWritten >= size) { // have we written all the bytes we were asked to write
      break;
    }

    unsigned int result = FilterOutputStream::write(getBuffer() + readHead, writeHead - readHead, nonblocking);
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

BufferedOutputStream::~BufferedOutputStream() throw(IOException) {
  TRACE_MEMBER();
  flush();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
