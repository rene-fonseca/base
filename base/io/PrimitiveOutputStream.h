/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_PRIMITIVE_OUTPUT_STREAM_H
#define _DK_SDU_MIP_BASE_IO_PRIMITIVE_OUTPUT_STREAM_H

#include "FilterOutputStream.h"

/**
  A FilterOutputStream that allows you to write primitive data types to an output stream in big-endian format. MT-Safe implementation if used with MT-safe input stream.

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
    Writes a boolean (8 bits) to the stream.
  */
  void writeBoolean(bool value);

  /**
    Writes a byte/character (8 bits) to the stream.
  */
  void writeChar(char value);

  /**
    Writes a short integer (16 bits) to the stream.
  */
  void writeShortInteger(short value);

  /**
    Writes an unsigned short integer (16 bits) to the stream.
  */
  void writeUnsignedShortInteger(unsigned short value);

  /**
    Writes an integer (32 bits) to the stream.
  */
  void writeInteger(int value);

  /**
    Writes an unsigned integer (32 bits) to the stream.
  */
  void writeUnsignedInteger(unsigned int value);

  /**
    Writes a long long integer (64 bits) to the stream.
  */
  void writeLongInteger(long long value);

  /**
    Writes an unsigned long long (64 bits) integer to the stream.
  */
  void writeUnsignedLongInteger(unsigned long long value);

  /**
    Writes a float (IEEE 32 bit format) to the stream.
  */
  void writeFloat(float value);

  /**
    Writes a double (IEEE 64 bit format) to the stream.
  */
  void writeDouble(double value);

  /**
    Writes a long double (Intel 386 style 80 bit format) to the stream.
  */
  void writeLongDouble(long double value);
};

#endif
