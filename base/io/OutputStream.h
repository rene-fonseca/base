/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H

#include <base/io/Stream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class representing an output stream of bytes.

  @short Output stream.
  @ingroup io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OutputStream : public virtual Stream {
public:

  /**
    Forces any buffered bytes to be written out.
  */
  virtual void flush() throw(IOException) = 0;

  /**
    Writes the specified number of bytes in the buffer to the stream. In
    blocking mode the method does not return until all bytes have been written.
    In non-blocking mode the total number of bytes written may be any number
    below or equal to the requested number of bytes.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Specifies that the method may not block.
    @return The actual number of bytes written to the stream.
  */
  virtual unsigned int write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
