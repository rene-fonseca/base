/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveInputStream.h>
#include <base/Primitives.h>
#include <base/ByteOrder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

PrimitiveInputStream::PrimitiveInputStream(InputStream& in) throw(BindException) :
  FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() throw(IOException) {
  char value;
  read(getCharAddress(value), sizeof(value));
  return value != 0;
}

char PrimitiveInputStream::readChar() throw(IOException) {
  char value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

short PrimitiveInputStream::readShortInteger() throw(IOException) {
  BigEndian::SignedShort value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

unsigned short PrimitiveInputStream::readUnsignedShortInteger() throw(IOException) {
  BigEndian::UnsignedShort value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

int PrimitiveInputStream::readInteger() throw(IOException) {
  BigEndian::SignedInt value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() throw(IOException) {
  BigEndian::UnsignedInt value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

long long PrimitiveInputStream::readLongInteger() throw(IOException) {
  BigEndian::SignedLongLong value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

unsigned long long PrimitiveInputStream::readUnsignedLongInteger() throw(IOException) {
  BigEndian::UnsignedLongLong value;
  read(getCharAddress(value), sizeof(value));
  return value;
}

float PrimitiveInputStream::readFloat() throw(IOException) {
  float value;
  // TAG: convert value from IEEE_754_SinglePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

double PrimitiveInputStream::readDouble() throw(IOException) {
  double value;
  // TAG: convert value from IEEE_754_DoublePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

long double PrimitiveInputStream::readLongDouble() throw(IOException) {
  long double value;
  // TAG: convert value from IEEE_QuadruplePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
