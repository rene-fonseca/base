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

#include <base/io/FilterOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that adds buffer functionality to an OutputStream for
  added performance. MT-unsafe implementation.
  
  @short Buffered output stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API BufferedOutputStream : public FilterOutputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
protected:

  /** The buffer. */
  Allocator<uint8> buffer;
  /** The write head. */
  unsigned int writeHead = 0;
  /** The read head. */
  unsigned int readHead = 0;
public:

  /**
    Initializes the buffered output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by
    DEFAULT_BUFFER_SIZE.
  */
  BufferedOutputStream(
    OutputStream& out,
    unsigned int size = DEFAULT_BUFFER_SIZE
  );

  /**
    Forces any buffered bytes to be written out.
  */
  void flush();

  /**
    Writes bytes in the specified buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Specifies that the method may not block.
    @return The actual number of bytes written. Always the requested number of
    bytes in blocking mode unless an error occured.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Writes a single value multiple times to the stream.

    @param value The value to be written.
    @param count The number of bytes to be written.
  */
  void unfoldValue(char value, unsigned int count);

  /**
    Flushes the buffer and releases any resources used by the output stream.
  */
  ~BufferedOutputStream();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
