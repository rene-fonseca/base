/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_OUTPUT_STREAM_H

/**
  Class representing an output stream of bytes.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class OutputStream {
public:

  /**
    Closes the output stream and releases any system resources associated with the stream.
  */
  virtual void close();

  /**
    Forces any buffered bytes to be written out.
  */
  virtual void flush();

  /**
    Writes a single value to the stream.

    @param value The value to be written.
  */
  virtual void write(char value) = 0;

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
  */
  virtual void write(char* buffer, unsigned int size);
};

#endif
