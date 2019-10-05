/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H

#include <base/io/FilterOutputStream.h>
#include <base/mem/Allocator.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that adds buffer functionality to an OutputStream for
  added performance. MT-unsafe implementation.
  
  @short Buffered output stream.
  @ingroup io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
  Allocator<uint8> buffer;
  /** The write head. */
  unsigned int writeHead;
  /** The read head. */
  unsigned int readHead;
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
  ) throw(BindException, MemoryException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

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
    bool nonblocking = false) throw(IOException);

  /**
    Writes a single value multiple times to the stream.

    @param value The value to be written.
    @param count The number of bytes to be written.
  */
  void unfoldValue(char value, unsigned int count) throw(IOException);

  /**
    Flushes the buffer and releases any resources used by the output stream.
  */
  ~BufferedOutputStream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
