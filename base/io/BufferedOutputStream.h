/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_BUFFERED_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_BUFFERED_OUTPUT_STREAM_H

#include "FilterOutputStream.h"

/**
  A FilterOutputStream that adds buffer functionality to an OutputStream for added performance.

  @author René Møller Fonseca
  @version 1.0
*/

class BufferedOutputStream : public FilterOutputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
protected:

  /** The buffer. */
  char* buffer;
  /** The size of the buffer. */
  unsigned int size;
  /** The number of bytes in the buffer. */
  unsigned int count;
  /** Flushes the buffer. */
  void flushBuffer();
public:

  /**
    Initializes the buffered output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  BufferedOutputStream(OutputStream* out, unsigned int size = DEFAULT_BUFFER_SIZE);

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
  void write(char* buffer, unsigned int size);

  /**
    Destroys the buffered output stream.
  */
  ~BufferedOutputStream();
};

#endif
