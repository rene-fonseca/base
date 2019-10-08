/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/FilterOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that allows you to write primitive data types to an
  output stream in big-endian format. MT-Safe implementation if used with
  MT-safe input stream.

  @short Primitive output stream.
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API PrimitiveOutputStream : public FilterOutputStream {
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
