/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__INPUT_STREAM_H

#include "Stream.h"

/**
  Class representing an input stream of bytes.

  @author René Møller Fonseca
  @version 1.0
*/

class InputStream : public Stream {
public:

  /**
    Returns true if the end of the stream has been reached.
  */
  virtual bool atEnd() const throw(IOException) = 0;

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  virtual unsigned int available() const throw(IOException) = 0;

  /**
    Fills the specified buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @return The actual number of bytes read from the stream.
  */
  virtual unsigned int read(char* buffer, unsigned int size) throw(IOException) = 0;

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
    @return The actual number of bytes skipped.
  */
  virtual unsigned int skip(unsigned int count) throw(IOException) = 0;
};

#endif
