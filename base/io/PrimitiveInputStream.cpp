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
  InputStream& in) throw(BindException)
  : FilterInputStream(in) {
}

bool PrimitiveInputStream::readBoolean() throw(IOException) {
  char value = 0;
  read(Cast::getAddress(value), sizeof(value));
  return value != 0;
}

char PrimitiveInputStream::readChar() throw(IOException) {
  char value = 0;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

short PrimitiveInputStream::readShortInteger() throw(IOException) {
  BigEndian<int16> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned short
  PrimitiveInputStream::readUnsignedShortInteger() throw(IOException) {
  BigEndian<uint16> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

int PrimitiveInputStream::readInteger() throw(IOException) {
  BigEndian<int32> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned int PrimitiveInputStream::readUnsignedInteger() throw(IOException) {
  BigEndian<uint32> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

long long PrimitiveInputStream::readLongInteger() throw(IOException) {
  BigEndian<int64> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

unsigned long long
  PrimitiveInputStream::readUnsignedLongInteger() throw(IOException) {
  BigEndian<uint64> value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

float PrimitiveInputStream::readFloat() throw(IOException) {
  FloatingPoint::IEEE754SinglePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

double PrimitiveInputStream::readDouble() throw(IOException) {
  FloatingPoint::IEEE754DoublePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

long double PrimitiveInputStream::readLongDouble() throw(IOException) {
  FloatingPoint::IEEEQuadruplePrecision value;
  read(Cast::getAddress(value), sizeof(value));
  return value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
