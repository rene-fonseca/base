/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_DATA_INPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_DATA_INPUT_STREAM_H

#include "FilterInputStream.h"

/**
  A FilterOutputStream that allows you to read primitive data types from an input stream in little-endian format.

  @author René Møller Fonseca
  @version 1.0
*/

class PrimitiveInputStream : public FilterInputStream {
public:

  /**
    Initializes the data input stream.

    @param in Input stream to be filtered.
  */
  explicit PrimitiveInputStream(InputStream* in);

  /**
    Reads a boolean from the stream.
  */
  bool readBoolean();

  /**
    Reads a character from the stream.
  */
  char readChar();

  /**
    Reads a short integer from the stream.
  */
  short readShortInteger();

  /**
    Reads an unsigned short integer from the stream.
  */
  unsigned short readUnsignedShortInteger();

  /**
    Reads an integer from the stream.
  */
  int readInteger();

  /**
    Reads an unsigned integer from the stream.
  */
  unsigned int readUnsignedInteger();

  /**
    Reads a long integer from the stream.
  */
  long readLongInteger();

  /**
    Reads an unsigned long integer from the stream.
  */
  unsigned long readUnsignedLongInteger();

  /**
    Reads a long long integer from the stream.
  */
  long long readLongLongInteger();

  /**
    Reads an unsigned long long integer from the stream.
  */
  unsigned long long readUnsignedLongLongInteger();

  /**
    Reads a float from the stream.
  */
  float readFloat();

  /**
    Reads a double from the stream.
  */
  double readDouble();
};

#endif
