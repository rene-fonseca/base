/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__INPUT_STREAM_H

#include "IOException.h"

/**
  Class representing an input stream of bytes.

  @author René Møller Fonseca
  @version 1.0
*/

class InputStream {
public:

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  virtual unsigned int available();

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  virtual void close() throw(IOException);

  /**
    Reads the next byte from the stream. Blocks if no bytes are available.

    @return The next byte from the stream.
  */
  virtual int read() throw(IOException) = 0;

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
  */
  virtual unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
  */
  virtual void skip(unsigned int count) throw(IOException);
};

#endif
