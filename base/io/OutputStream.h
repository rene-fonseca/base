/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H

#include <base/io/Stream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class representing an output stream of bytes.

  @author René Møller Fonseca
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
