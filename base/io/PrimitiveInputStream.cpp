/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/PrimitiveInputStream.h>
#include <base/Primitives.h>
#include <base/ByteOrder.h>
#include <base/FloatingPoint.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

PrimitiveInputStream::PrimitiveInputStream(
  InputStream& in)
  : FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() {
  char value = 0;
  read(Cast::getAddress(value), sizeof(value));
  return value != 0;
}

char PrimitiveInputStream::readChar() {
  char value = 0;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

short PrimitiveInputStream::readShortInteger() {
  BigEndian<int16> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned short
  PrimitiveInputStream::readUnsignedShortInteger() {
  BigEndian<uint16> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

int PrimitiveInputStream::readInteger() {
  BigEndian<int32> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() {
  BigEndian<uint32> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

long long PrimitiveInputStream::readLongInteger() {
  BigEndian<int64> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned long long
  PrimitiveInputStream::readUnsignedLongInteger() {
  BigEndian<uint64> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

float PrimitiveInputStream::readFloat() {
  FloatingPoint::IEEE754SinglePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

double PrimitiveInputStream::readDouble() {
  FloatingPoint::IEEE754DoublePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

long double PrimitiveInputStream::readLongDouble() {
  FloatingPoint::IEEEQuadruplePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
