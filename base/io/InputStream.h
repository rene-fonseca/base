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

#include <base/io/Stream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class representing an input stream of bytes.

  @short Input stream.
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API InputStream : public virtual Stream {
public:

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking under normal circumstances. The number of returned bytes may be
    less than the true number of bytes available. Attempting to read the number
    of bytes returned by the function may block the calling thread if some
    process has acquired a lock on the object (e.g. file locking).

    @return Available number of bytes in stream.
  */
  virtual unsigned int available() const throw(IOException) = 0;

  /**
    Fills the specified buffer with bytes from the stream. In blocking mode the
    method does not return until all bytes have been read. In non-blocking mode
    the total bytes read may be any number below or equal to the requested
    number of bytes. Attempting to read beyond the end of a stream in blocking
    mode results in an EndOfFile exception.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @param nonblocking Specifies that the method may not block.
    @return The actual number of bytes read from the stream.
  */
  virtual unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking) throw(IOException) = 0;

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than
    available.

    @param count The number of bytes to skip.
    @return The actual number of bytes skipped.
  */
  virtual unsigned int skip(unsigned int count) throw(IOException) = 0;

  /**
    Blocking wait for input to become available.
  */
  virtual void wait() const throw(IOException) = 0;

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  virtual bool wait(unsigned int timeout) const throw(IOException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
