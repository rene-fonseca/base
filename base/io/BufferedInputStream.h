/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H

#include <base/io/FilterInputStream.h>
#include <base/mem/Allocator.h>
#include <base/OutOfDomain.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterInputStream that adds buffer functionality to an InputStream for added performance. MT-unsafe implementation.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
public:

  /**
    Initializes the buffered input stream.

    @param in The input stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE. The size cannot...
  */
  BufferedInputStream(InputStream& in, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException, MemoryException);

  /**
    Returns the unread bytes of the internal buffer as sequence. The size of
    the sequence is returned by getNumberOfBytes() or peek().
  */
  inline const char* getElements() const throw() {return buffer.getElements() + position;}

  /**
    Returns the number of bytes currently available in the internal buffer.
    This is not the same as available() which also includes buffered bytes of
    the linked input stream.
  */
  inline unsigned int getNumberOfBytes() const throw() {return count - position;}

  /**
    Returns true if the internal buffer is empty.
  */
  inline bool isEmpty() const throw() {return position >= count;}

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Fills the specified buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @param nonblocking Specifies that the method may not block. Default is false.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

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
    This function ensures that the requested number of bytes is available in
    the internal buffer. Use getElements() afterwards to examine the content of
    the buffer.

    @param count The number of bytes to peek ahead.
    @return The number of bytes in the internal buffer (only less than the
    requested number of bytes if EOF is hit).
  */
  unsigned int peek(unsigned int count) throw(OutOfDomain, IOException);

  /**
    Destroy buffered input stream.
  */
  ~BufferedInputStream() throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
