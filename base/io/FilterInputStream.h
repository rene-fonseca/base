/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILTER_INPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_FILTER_INPUT_STREAM_H

#include "base/Object.h"
#include "InputStream.h"

/**
  A FilterInputStream redirects requests to an ordinary InputStream possibly transforming data and providing additional functionality in the process. A FilterInputStream is itself an InputStream.

  @author René Møller Fonseca
  @version 1.0
*/

class FilterInputStream : public Object, public InputStream {
protected:

  /** Input stream to be filtered. */
  InputStream* in;
public:

  /**
    Initializes the filter input stream.

    @param in Input stream to be filtered.
  */
  explicit FilterInputStream(InputStream* in);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available();

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close();

  /**
    Reads the next byte from the stream. Blocks if no bytes are available.

    @return The next byte from the stream.
  */
  int read();

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
  */
  unsigned int read(char* buffer, unsigned int size);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
  */
  void skip(unsigned int count);

  /**
    Destroys the filter input stream.
  */
  ~FilterInputStream();
};

#endif
