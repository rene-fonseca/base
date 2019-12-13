/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveOutputStream.h>
#include <base/ByteOrder.h>
#include <base/Primitives.h>
#include <base/FloatingPoint.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

PrimitiveOutputStream::PrimitiveOutputStream(
  OutputStream& out)
  : FilterOutputStream(out) {
}

void PrimitiveOutputStream::writeBoolean(bool value) {
  const uint8 temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeChar(char value) {
  const uint8 temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeShortInteger(short value) {
  BigEndian<int16> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedShortInteger(
  unsigned short value) {
  BigEndian<uint16> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeInteger(int value) {
  BigEndian<int32> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedInteger(
  unsigned int value) {
  BigEndian<uint16> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongInteger(
  long long value) {
  BigEndian<int64> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeUnsignedLongInteger(
  unsigned long long value) {
  BigEndian<uint64> temp;
  temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeFloat(float value) {
  FloatingPoint::IEEE754SinglePrecision temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeDouble(double value) {
  FloatingPoint::IEEE754DoublePrecision temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

void PrimitiveOutputStream::writeLongDouble(
  long double value) {
  FloatingPoint::IEEEQuadruplePrecision temp = value;
  write(Cast::getAddress(temp), sizeof(temp));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
