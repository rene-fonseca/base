/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PRIMITIVE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__PRIMITIVE_INPUT_STREAM_H

#include <base/io/FilterInputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A FilterOutputStream that allows you to read primitive data types from an input stream in big-endian format. MT-Safe implementation if used with MT-safe input stream.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class PrimitiveInputStream : public FilterInputStream {
public:

  /**
    Initializes the data input stream.

    @param in Input stream to be filtered.
  */
  explicit PrimitiveInputStream(InputStream& in) throw(BindException);

  /**
    Reads a boolean (8 bits) from the stream.
  */
  bool readBoolean() throw(IOException);

  /**
    Reads a byte/character (8 bits) from the stream.
  */
  char readChar() throw(IOException);

  /**
    Reads a short integer (16 bits) from the stream.
  */
  short readShortInteger() throw(IOException);

  /**
    Reads an unsigned short integer (16 bits) from the stream.
  */
  unsigned short readUnsignedShortInteger() throw(IOException);

  /**
    Reads an integer (32 bits) from the stream.
  */
  int readInteger() throw(IOException);

  /**
    Reads an unsigned integer (32 bits) from the stream.
  */
  unsigned int readUnsignedInteger() throw(IOException);

  /**
    Reads a long integer (64 bits) from the stream.
  */
  long long readLongInteger() throw(IOException);

  /**
    Reads an unsigned long integer (64 bits) from the stream.
  */
  unsigned long long readUnsignedLongInteger() throw(IOException);

  /**
    Reads a float (IEEE 32 bit format) from the stream.
  */
  float readFloat() throw(IOException);

  /**
    Reads a double (IEEE 64 bit format) from the stream.
  */
  double readDouble() throw(IOException);

  /**
    Reads a long double (Intel 386 style 80 bit format) from the stream.
  */
  long double readLongDouble() throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
