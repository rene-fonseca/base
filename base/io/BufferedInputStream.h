/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_INPUT_STREAM_H

#include "FilterInputStream.h"

/**
  A FilterInputStream that adds buffer functionality to an InputStream for added performance. MT-unsafe implementation.

  @author René Møller Fonseca
  @version 1.0
*/

class BufferedInputStream : public FilterInputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
protected:

  /** The internal buffer used to store data temporarily. */
  char* buffer;
  /** The size fo the buffer. */
  unsigned int size;
  /** The current number of bytes in the buffer. */
  unsigned int count;
  /** The current position in the buffer. */
  unsigned int position;
  /** Fills the buffer with bytes from the input stream. */
  void fillBuffer() throw(IOException);
public:

  /**
    Initializes the buffered input stream.

    @param in The input stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE. The size cannot
  */
  BufferedInputStream(InputStream* in, unsigned int size = DEFAULT_BUFFER_SIZE);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available();

  /**
    Reads the next byte from the stream. Blocks if no bytes are available.

    @return The next byte from the stream.
  */
  int read() throw(IOException);

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer to fill.
    @param size The size of the buffer.
  */
  unsigned int read(char* buffer, unsigned int count) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
  */
  void skip(unsigned int count) throw(IOException);

  /**
    Destroys the buffered input stream.
  */
  ~BufferedInputStream();
};

#endif
