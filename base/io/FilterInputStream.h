/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILTER_INPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_FILTER_INPUT_STREAM_H

#include "base/Object.h"
#include "InputStream.h"

/**
  A FilterInputStream redirects requests to an ordinary InputStream possibly transforming data and providing additional functionality in the process. A FilterInputStream is itself an InputStream. MT-Safe implementation if used with MT-safe input stream.

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
  void close() throw(IOException);

  /**
    Reads the next byte from the stream. Blocks if no bytes are available. Throws 'EndOfFile' if called when end of file has been reached.

    @return The next byte from the stream.
  */
  int read() throw(IOException);

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available. Throws 'EndOfFile' if called when end of file has been reached.

    @param buffer The buffer.
    @param size The size of the buffer.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available. Throws 'EndOfFile' if called when end of file has been reached.

    @param count The number of bytes to skip.
  */
  void skip(unsigned int count) throw(IOException);

  /**
    Destroys the filter input stream.
  */
  ~FilterInputStream();
};

#endif
