/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PRIMITIVE_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__PRIMITIVE_OUTPUT_STREAM_H

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
  explicit PrimitiveOutputStream(OutputStream& out) throw(BindException);

  /**
    Writes a boolean (8 bits) to the stream.
  */
  void writeBoolean(bool value) throw(IOException);

  /**
    Writes a byte/character (8 bits) to the stream.
  */
  void writeChar(char value) throw(IOException);

  /**
    Writes a short integer (16 bits) to the stream.
  */
  void writeShortInteger(short value) throw(IOException);

  /**
    Writes an unsigned short integer (16 bits) to the stream.
  */
  void writeUnsignedShortInteger(unsigned short value) throw(IOException);

  /**
    Writes an integer (32 bits) to the stream.
  */
  void writeInteger(int value) throw(IOException);

  /**
    Writes an unsigned integer (32 bits) to the stream.
  */
  void writeUnsignedInteger(unsigned int value) throw(IOException);

  /**
    Writes a long long integer (64 bits) to the stream.
  */
  void writeLongInteger(long long value) throw(IOException);

  /**
    Writes an unsigned long long (64 bits) integer to the stream.
  */
  void writeUnsignedLongInteger(unsigned long long value) throw(IOException);

  /**
    Writes a float (IEEE 32 bit format) to the stream.
  */
  void writeFloat(float value) throw(IOException);

  /**
    Writes a double (IEEE 64 bit format) to the stream.
  */
  void writeDouble(double value) throw(IOException);

  /**
    Writes a long double (Intel 386 style 80 bit format) to the stream.
  */
  void writeLongDouble(long double value) throw(IOException);
};

#endif
