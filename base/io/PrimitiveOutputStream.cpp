/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveOutputStream.h>
#include <base/ByteOrder.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

PrimitiveOutputStream::PrimitiveOutputStream(OutputStream& out) throw(BindException) :
  FilterOutputStream(out) {
}

void PrimitiveOutputStream::writeBoolean(bool value) throw(IOException) {
  write(getCharAddress(value), sizeof(char));
}

void PrimitiveOutputStream::writeChar(char value) throw(IOException) {
  write(getCharAddress(value), sizeof(char));
}

void PrimitiveOutputStream::writeShortInteger(short value) throw(IOException) {
  BigEndian::SignedShort temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedShortInteger(unsigned short value) throw(IOException) {
  BigEndian::UnsignedShort temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeInteger(int value) throw(IOException) {
  BigEndian::SignedInt temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedInteger(unsigned int value) throw(IOException) {
  BigEndian::UnsignedInt temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongInteger(long long value) throw(IOException) {
  BigEndian::SignedLongLong temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedLongInteger(unsigned long long value) throw(IOException) {
  BigEndian::UnsignedLongLong temp = value;
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeFloat(float value) throw(IOException) {
  float temp; // TAG: convert to IEEE_FloatPrecision (big endian)
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeDouble(double value) throw(IOException) {
  double temp; // TAG: convert to IEEE_DoublePrecision (big endian)
  write(getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongDouble(long double value) throw(IOException) {
  long double temp; // TAG: convert to IEEE_QuadruplePrecision (big endian)
  write(getCharAddress(temp), sizeof(temp));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
