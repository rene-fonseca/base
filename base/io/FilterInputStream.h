/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILTER_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILTER_INPUT_STREAM_H

#include <base/io/InputStream.h>
#include <base/io/BindException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterInputStream redirects requests to an ordinary InputStream possibly transforming data and providing additional functionality in the process. A FilterInputStream is itself an InputStream. MT-Safe implementation if used with MT-safe input stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FilterInputStream : public virtual Object, public virtual InputStream {
protected:

  /** Input stream to be filtered. */
  InputStream& in;
public:

  /**
    Initializes the filter input stream.

    @param in Input stream to be filtered.
  */
  explicit FilterInputStream(InputStream& in) throw(BindException);

  /**
    Returns true if the end of the stream has been reached.
  */
  bool atEnd() const throw(IOException);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Fills the specified buffer with bytes from the stream. In blocking mode the
    method does not return until all bytes have been read. In non-blocking mode
    the total bytes read may be any number below or equal to the requested
    number of bytes. Attempting to read beyond the end of a stream in blocking
    mode results in an EndOfFile exception.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @param nonblocking Specifies that the method may not block. The default is false.
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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
