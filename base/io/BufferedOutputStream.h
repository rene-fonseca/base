/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__BUFFERED_OUTPUT_STREAM_H

#include <base/io/FilterOutputStream.h>

/**
  A FilterOutputStream that adds buffer functionality to an OutputStream for added performance. MT-unsafe implementation.

  @author Ren� M�ller Fonseca
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
public:

  /**
    Initializes the buffered output stream.

    @param out The output stream.
    @param size The size of the buffer. Default is given by DEFAULT_BUFFER_SIZE.
  */
  BufferedOutputStream(OutputStream& out, unsigned int size = DEFAULT_BUFFER_SIZE) throw(BindException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException);

  /**
    Writes bytes in the specified buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Destroys the buffered output stream.
  */
  ~BufferedOutputStream();
};

#endif
