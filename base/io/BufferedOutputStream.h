/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H

#include <base/io/FilterOutputStream.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that adds buffer functionality to an OutputStream for added performance. MT-unsafe implementation.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class BufferedOutputStream : public FilterOutputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
protected:

  /** The buffer. */
  Allocator<char> buffer;
  /** The write head. */
  unsigned int writeHead;
  /** The read head. */
  unsigned int readHead;

  /**
    Returns the buffer.
  */
  inline char* getBuffer() throw() {
    return buffer.getElements();
  }

  /**
    Returns the size of the buffer.
  */
  inline unsigned int getSize() const throw() {
    return buffer.getSize();
  }
public:

  /**
    Initializes the buffered output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  BufferedOutputStream(OutputStream& out, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException, MemoryException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

  /**
    Writes bytes in the specified buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Specifies that the method may not block.
    @return The actual number of bytes written. Always the requested number of bytes in blocking mode unless an error occured.
  */
  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

  /**
    Flushes the buffer and releases any resources used by the output stream.
  */
  ~BufferedOutputStream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
