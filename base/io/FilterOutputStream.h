/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_FILTER_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_FILTER_OUTPUT_STREAM_H

#include "base/Object.h"
#include "OutputStream.h"

/**
  A FilterOutputStream redirects requests to an ordinary OutputStream possibly transforming data and providing additional functionality in the process. A FilterOutputStream is itself an OutputStream.

  @author René Møller Fonseca
  @version 1.0
*/

class FilterOutputStream : public Object, public OutputStream {
protected:

  /** Output stream to be filtered. */
  OutputStream* out;
public:

  /**
    Initializes the filter output stream.

    @param out Output stream to be filtered.
  */
  explicit FilterOutputStream(OutputStream* out);

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close();

  /**
    Forces any buffered bytes to be written out.
  */
  void flush();

  /**
    Writes a single value to the stream.

    @param value The value to be written.
  */
  void write(char value);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
  */
  void write(char* buffer, unsigned int size); // throw(StreamIsClose??)

  /**
    Destroys the filtered output stream.
  */
  ~FilterOutputStream();
};

#endif
