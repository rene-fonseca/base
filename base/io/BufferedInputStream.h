/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H

#include <base/io/FilterInputStream.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterInputStream that adds buffer functionality to an InputStream for added performance. MT-unsafe implementation.

  @author René Møller Fonseca
  @version 1.0
*/

class BufferedInputStream : public FilterInputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
protected:

  /** The internal buffer used to store data temporarily. */
  Allocator<char> buffer;
  /** The current number of bytes in the buffer. */
  unsigned int count;
  /** The current position in the buffer. */
  unsigned int position;

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

  /**
    Returns true if the internal buffer is empty.
  */
  inline bool isEmpty() const throw() {return position >= count;}
public:

  /**
    Initializes the buffered input stream.

    @param in The input stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE. The size cannot...
  */
  BufferedInputStream(InputStream& in, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException, MemoryException);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Fills the specified buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
    @return The actual number of bytes skipped.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Blocking wait for input to become available.
  */
  void wait() const throw(IOException);

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const throw(IOException);

  /**
    Returns buffer.
  */
//  const char* peek(unsigned int count) throw(IOException);

  /**
    Destroy buffered input stream.
  */
  ~BufferedInputStream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
