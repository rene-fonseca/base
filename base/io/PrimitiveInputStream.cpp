/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveInputStream.h>
#include <base/Type.h>
#include <base/ByteOrder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

PrimitiveInputStream::PrimitiveInputStream(InputStream& in) throw(BindException) :
  FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() throw(IOException) {
  char value;
  read(getCharAddress(value), sizeof(value)); // TAG: problem if size > 1
  return value != 0;
}

char PrimitiveInputStream::readChar() throw(IOException) {
  char value;
  read(getCharAddress(value), sizeof(value)); // TAG: problem if size > 1
  return value;
}

short PrimitiveInputStream::readShortInteger() throw(IOException) {
  short value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian<unsigned short>(value);
}

unsigned short PrimitiveInputStream::readUnsignedShortInteger() throw(IOException) {
  unsigned short value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian(value);
}

int PrimitiveInputStream::readInteger() throw(IOException) {
  int value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian<unsigned int>(value);
}

unsigned int PrimitiveInputStream::readUnsignedInteger() throw(IOException) {
  unsigned int value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian(value);
}

long long PrimitiveInputStream::readLongInteger() throw(IOException) {
  long long value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian<unsigned long long>(value);
}

unsigned long long PrimitiveInputStream::readUnsignedLongInteger() throw(IOException) {
  unsigned long long value;
  read(getCharAddress(value), sizeof(value));
  return ByteOrder::fromBigEndian<unsigned long long>(value);
}

float PrimitiveInputStream::readFloat() throw(IOException) {
  float value;
  // convert value from IEEE_754_SinglePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

double PrimitiveInputStream::readDouble() throw(IOException) {
  double value;
  // convert value from IEEE_754_DoublePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

long double PrimitiveInputStream::readLongDouble() throw(IOException) {
  long double value;
  // convert value from IEEE_QuadruplePrecision (big endian) and return
  read(getCharAddress(value), sizeof(value));
  return value; // TAG: fix this
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
