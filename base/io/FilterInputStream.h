/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILTER_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILTER_INPUT_STREAM_H

#include <base/io/InputStream.h>
#include <base/io/BindException.h>

/**
  A FilterInputStream redirects requests to an ordinary InputStream possibly transforming data and providing additional functionality in the process. A FilterInputStream is itself an InputStream. MT-Safe implementation if used with MT-safe input stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FilterInputStream : public virtual Object, public virtual InputStream {
protected:

  /** Input stream to be filtered. */
  InputStream& in;
public:

  /**
    Initializes the filter input stream.

    @param in Input stream to be filtered.
  */
  explicit FilterInputStream(InputStream& in) throw(BindException);

  /**
    Returns true if the end of the stream has been reached.
  */
  bool atEnd() const throw(IOException);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Fills the specified buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
    @return The actual number of bytes skipped.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Destroys the filter input stream.
  */
  ~FilterInputStream();
};

#endif
