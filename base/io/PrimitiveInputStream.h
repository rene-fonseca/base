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

#include <base/io/FilterInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that allows you to read primitive data types from an
  input stream in big-endian format. MT-Safe implementation if used with
  MT-safe input stream.

  @short Primitive input stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API PrimitiveInputStream : public FilterInputStream {
public:

  /**
    Initializes the data input stream.

    @param in Input stream to be filtered.
  */
  explicit PrimitiveInputStream(InputStream& in);

  /**
    Reads a boolean (8 bits) from the stream.
  */
  bool readBoolean();

  /**
    Reads a byte/character (8 bits) from the stream.
  */
  char readChar();

  /**
    Reads a short integer (16 bits) from the stream.
  */
  short readShortInteger();

  /**
    Reads an unsigned short integer (16 bits) from the stream.
  */
  unsigned short readUnsignedShortInteger();

  /**
    Reads an integer (32 bits) from the stream.
  */
  int readInteger();

  /**
    Reads an unsigned integer (32 bits) from the stream.
  */
  unsigned int readUnsignedInteger();

  /**
    Reads a long integer (64 bits) from the stream.
  */
  long long readLongInteger();

  /**
    Reads an unsigned long integer (64 bits) from the stream.
  */
  unsigned long long readUnsignedLongInteger();

  /**
    Reads a float (IEEE 32 bit format) from the stream.
  */
  float readFloat();

  /**
    Reads a double (IEEE 64 bit format) from the stream.
  */
  double readDouble();

  /**
    Reads a long double (Intel 386 style 80 bit format) from the stream.
  */
  long double readLongDouble();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
