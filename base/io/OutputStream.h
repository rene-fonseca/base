/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__OUTPUT_STREAM_H

#include <base/io/Stream.h>

/**
  Class representing an output stream of bytes.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class OutputStream : public virtual Stream {
public:

  /**
    Forces any buffered bytes to be written out.
  */
  virtual void flush() throw(IOException) = 0;

  /**
    Writes bytes in the specified buffer to the stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  virtual unsigned int write(const char* buffer, unsigned int size) throw(IOException) = 0;
};

#endif
