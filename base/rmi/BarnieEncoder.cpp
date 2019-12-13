/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/BarnieEncoder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0

void BarnieEncoder::write(const char* buffer, unsigned int size) {
}

void BarnieEncoder::write(char value) {
}

void BarnieEncoder::write(const ucs4* buffer, unsigned int size) {
}

void BarnieEncoder::write(ucs4 value) {
}

void BarnieEncoder::write(const uint8* buffer, unsigned int size) {
}

void BarnieEncoder::write(uint8 value) {
  write(Cast::getCharAddress(value), sizeof(value));
}

void BarnieEncoder::write(const short* buffer, unsigned int size) {
}

void BarnieEncoder::write(short value) {
  BigEndian<int16> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const unsigned short* buffer, unsigned int size) {
}

void BarnieEncoder::write(unsigned short value) {
  BigEndian<uint16> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const int* buffer, unsigned int size) {
}

void BarnieEncoder::write(int value) {
  BigEndian<int32> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const unsigned int* buffer, unsigned int size) {
}

void BarnieEncoder::write(unsigned int value) {
  BigEndian<uint32> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const long* buffer, unsigned int size) {
}

void BarnieEncoder::write(long value) {
}

void BarnieEncoder::write(const unsigned long* buffer, unsigned int size) {
}

void BarnieEncoder::write(unsigned long value) {
  BigEndian<int64> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const long long* buffer, unsigned int size) {
}

void BarnieEncoder::write(long long value) {
  BigEndian<int64> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const unsigned long long* buffer, unsigned int size) {
}

void BarnieEncoder::write(unsigned long long value) {
  BigEndian<int64> buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const float* buffer, unsigned int size) {
}

void BarnieEncoder::write(float value) {
  FloatingPoint::IEEE754SinglePrecision buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const double* buffer, unsigned int size) {
//   write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(double value) {
  FloatingPoint::IEEE754DoublePrecision buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

void BarnieEncoder::write(const long double* buffer, unsigned int size) {
//   FloatingPoint::IEEEQuadruplePrecision buffer[BUFFER_SIZE];
//   // = value;
//   while (size) {
//     unsigned int 
//     write(Cast::getCharAddress(buffer), );
//   }
}

void BarnieEncoder::write(long double value) {
  FloatingPoint::IEEEQuadruplePrecision buffer = value;
  write(Cast::getCharAddress(buffer), sizeof(buffer));
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
