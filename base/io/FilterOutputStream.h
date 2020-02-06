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
#include <base/io/OutputStream.h>
#include <base/io/BindException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream redirects requests to an ordinary OutputStream possibly
  transforming data and providing additional functionality in the process. A
  FilterOutputStream is itself an OutputStream.
  
  @short Filter output stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FilterOutputStream : public virtual Object, public virtual OutputStream {
protected:

  /** Output stream to be filtered. */
  OutputStream& out;
public:

  /**
    Initializes the filter output stream.

    @param out Output stream to be filtered.
  */
  explicit FilterOutputStream(OutputStream& out);

  /** Returns the output stream. */
  inline OutputStream& getOutputStream() noexcept
  {
    return out;
  }

  /**
    Closes the output stream and releases any system resources associated with the stream.
  */
  void close();

  /**
    Forces any buffered bytes to be written out.
  */
  void flush();

  /**
    Writes the specified number of bytes in the buffer to the stream. In
    blocking mode the method does not return until all bytes have been written.
    In non-blocking mode the total number of bytes written may be any number
    below or equal to the requested number of bytes.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Specifies that the method may not block. Default is false.
    @return The actual number of bytes written to the stream.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
