/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILTER_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILTER_OUTPUT_STREAM_H

#include <base/io/OutputStream.h>
#include <base/io/BindException.h>

/**
  A FilterOutputStream redirects requests to an ordinary OutputStream possibly transforming data and providing additional functionality in the process. A FilterOutputStream is itself an OutputStream.

  @author René Møller Fonseca
  @version 1.0
*/

class FilterOutputStream : public virtual Object, public virtual OutputStream {
protected:

  /** Output stream to be filtered. */
  OutputStream& out;
public:

  /**
    Initializes the filter output stream.

    @param out Output stream to be filtered.
  */
  explicit FilterOutputStream(OutputStream& out) throw(BindException);

  /**
    Closes the output stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

  /**
    Writes bytes in the specified buffer to the stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Destroys the filtered output stream.
  */
  ~FilterOutputStream();
};

#endif
