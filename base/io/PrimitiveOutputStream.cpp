/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveOutputStream.h>
#include <base/ByteOrder.h>
#include <base/Primitives.h>
#include <base/FloatingPoint.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

PrimitiveOutputStream::PrimitiveOutputStream(
  OutputStream& out) throw(BindException)
  : FilterOutputStream(out) {
}

void PrimitiveOutputStream::writeBoolean(bool value) throw(IOException) {
  const uint8 temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeChar(char value) throw(IOException) {
  const uint8 temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeShortInteger(short value) throw(IOException) {
  BigEndian<int16> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedShortInteger(unsigned short value) throw(IOException) {
  BigEndian<uint16> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeInteger(int value) throw(IOException) {
  BigEndian<int32> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedInteger(unsigned int value) throw(IOException) {
  BigEndian<uint16> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongInteger(long long value) throw(IOException) {
  BigEndian<int64> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedLongInteger(unsigned long long value) throw(IOException) {
  BigEndian<uint64> temp;
  temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeFloat(float value) throw(IOException) {
  FloatingPoint::IEEE754SinglePrecision temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeDouble(double value) throw(IOException) {
  FloatingPoint::IEEE754DoublePrecision temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongDouble(long double value) throw(IOException) {
  FloatingPoint::IEEEQuadruplePrecision temp = value;
  write(Cast::getCharAddress(temp), sizeof(temp));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
