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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that allows you to write primitive data types to an
  output stream in big-endian format. MT-Safe implementation if used with
  MT-safe input stream.

  @short Primitive output stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API PrimitiveOutputStream : public FilterOutputStream {
public:

  /**
    Initializes the data output stream.

    @param out Output stream to be filtered.
  */
  explicit PrimitiveOutputStream(OutputStream& out);

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
