/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_DATA_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_DATA_OUTPUT_STREAM_H

#include "FilterOutputStream.h"

/**
  A FilterOutputStream that allows you to write primitive data types to an output stream in little-endian format.

  @author René Møller Fonseca
  @version 1.0
*/

class PrimitiveOutputStream : public FilterOutputStream {
public:

  /**
    Initializes the data output stream.

    @param out Output stream to be filtered.
  */
  explicit PrimitiveOutputStream(OutputStream* out);

  /**
    Writes a boolean to the stream.
  */
  void writeBoolean(bool value);

  /**
    Writes a character to the stream.
  */
  void writeChar(char value);

  /**
    Writes a short integer to the stream.
  */
  void writeShortInteger(short value);

  /**
    Writes an unsigned short integer to the stream.
  */
  void writeUnsignedShortInteger(unsigned short value);

  /**
    Writes an integer to the stream.
  */
  void writeInteger(int value);

  /**
    Writes an unsigned integer to the stream.
  */
  void writeUnsignedInteger(unsigned int value);

  /**
    Writes a long integer to the stream.
  */
  void writeLongInteger(long value);

  /**
    Writes an unsigned long integer to the stream.
  */
  void writeUnsignedLongInteger(unsigned long value);

  /**
    Writes a long long integer to the stream.
  */
  void writeLongLongInteger(long long value);

  /**
    Writes an unsigned long long integer to the stream.
  */
  void writeUnsignedLongLongInteger(unsigned long long value);

  /**
    Writes a float to the stream.
  */
  void writeFloat(float value);

  /**
    Writes a double to the stream.
  */
  void writeDouble(double value);
};

#endif
