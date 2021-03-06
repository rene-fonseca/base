/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/io/InputStream.h>
#include <base/io/BindException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A FilterInputStream redirects requests to an ordinary InputStream possibly
  transforming data and providing additional functionality in the process. A
  FilterInputStream is itself an InputStream. MT-Safe implementation if used
  with MT-safe input stream.

  @short Filter input stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FilterInputStream : public virtual Object, public virtual InputStream {
protected:

  /** Input stream to be filtered. */
  InputStream& in;
public:

  /**
    Initializes the filter input stream.

    @param in Input stream to be filtered.
  */
  explicit FilterInputStream(InputStream& in);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const;

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close();

  /**
    Fills the specified buffer with bytes from the stream. In blocking mode the
    method does not return until all bytes have been read. In non-blocking mode
    the total bytes read may be any number below or equal to the requested
    number of bytes. Attempting to read beyond the end of a stream in blocking
    mode results in an EndOfFile exception.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @param nonblocking Specifies that the method may not block. The default is
    false.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than
    available.

    @param count The number of bytes to skip.
    @return The actual number of bytes skipped.
  */
  unsigned int skip(unsigned int count);

  /**
    Blocking wait for input to become available.
  */
  void wait() const;

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
