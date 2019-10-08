/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/LisaEncoding.h>
#include <base/FloatingPoint.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void LisaEncoding::LisaDecoder::read(OrbReference& value) throw(IOException) {
}
  
void LisaEncoding::LisaDecoder::read(String& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(WideString& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  char* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(char& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  short* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(short& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  unsigned short* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  unsigned short&  value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  int* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(int& value) throw(IOException) {
}
  
void LisaEncoding::LisaDecoder::read(
  unsigned int* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(unsigned int& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  long* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(long& value) throw(IOException) {
}
  
void LisaEncoding::LisaDecoder::read(
  unsigned long* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  unsigned long& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  long long* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(long long& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  unsigned long long* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  unsigned long long& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  float* buffer, unsigned int size) throw(IOException) {
}
  
void LisaEncoding::LisaDecoder::read(float& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  double* buffer, unsigned int size) throw(IOException) {
}
  
void LisaEncoding::LisaDecoder::read(double& value) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(
  long double* buffer, unsigned int size) throw(IOException) {
}

void LisaEncoding::LisaDecoder::read(long double& value) throw(IOException) {
}




void LisaEncoding::LisaEncoder::write(
  const OrbReference& value) throw(IOException) {
  pushValue<uint64, uint64>(value.getId());
}

void LisaEncoding::LisaEncoder::write(
  const String& value) throw(IOException) {
  pushValue<uint32, unsigned int>(value.getLength());
  OrbEncoder::push(
    Cast::pointer<const uint8*>(value.getElements()),
    value.getLength()
  );
}

void LisaEncoding::LisaEncoder::write(
  const WideString& value) throw(IOException) {
  pushValue<uint32, unsigned int>(value.getLength());
  pushArray<uint32, ucs4>(value.getElements(), value.getLength());
}

void LisaEncoding::LisaEncoder::write(
  const char* buffer, unsigned int size) throw(IOException) {
  OrbEncoder::push(Cast::pointer<const uint8*>(buffer), size);
}

void LisaEncoding::LisaEncoder::write(char value) throw(IOException) {
  OrbEncoder::push(Cast::getAddress(value), sizeof(uint8));
}

void LisaEncoding::LisaEncoder::write(
  const short* buffer, unsigned int size) throw(IOException) {
  pushArray<int16, short>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(short value) throw(IOException) {
  pushValue<int16, short>(value);
}

void LisaEncoding::LisaEncoder::write(
  const unsigned short* buffer, unsigned int size) throw(IOException) {
  pushArray<uint16, unsigned short>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(
  unsigned short value) throw(IOException) {
  pushValue<uint16, unsigned short>(value);
}

void LisaEncoding::LisaEncoder::write(
  const int* buffer, unsigned int size) throw(IOException) {
  pushArray<int32, int>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(int value) throw(IOException) {
  pushValue<int32, int>(value);
}

void LisaEncoding::LisaEncoder::write(
  const unsigned int* buffer, unsigned int size) throw(IOException) {
  pushArray<uint32, unsigned int>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(unsigned int value) throw(IOException) {
  pushValue<uint32, unsigned int>(value);
}

void LisaEncoding::LisaEncoder::write(
  const long* buffer, unsigned int size) throw(IOException) {
  pushArray<int32, long>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(long value) throw(IOException) {
  pushValue<int32, long>(value);
}

void LisaEncoding::LisaEncoder::write(
  const unsigned long* buffer, unsigned int size) throw(IOException) {
  pushArray<uint32, unsigned long>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(
  unsigned long value) throw(IOException) {
  pushValue<uint32, unsigned long>(value);
}

void LisaEncoding::LisaEncoder::write(
  const long long* buffer, unsigned int size) throw(IOException) {
  pushArray<int64, long long>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(long long value) throw(IOException) {
  pushValue<int64, long long>(value);
}

void LisaEncoding::LisaEncoder::write(
  const unsigned long long* buffer, unsigned int size) throw(IOException) {
  pushArray<uint64, unsigned long long>(buffer, size);
}

void LisaEncoding::LisaEncoder::write(
  unsigned long long value) throw(IOException) {
  pushValue<uint64, unsigned long long>(value);
}

void LisaEncoding::LisaEncoder::write(
  const float* buffer, unsigned int size) throw(IOException) {
  if (typeid(FloatingPoint::FloatRepresentation) ==
      typeid(FloatingPoint::Representation::IEEE754SinglePrecision)) {
    OrbEncoder::push(
      Cast::pointer<const uint8*>(buffer),
      size * sizeof(float)
    );
  } else {
    const float* end = buffer + size;
    while (buffer != end) {
      FloatingPoint::IEEE754SinglePrecision temp = *buffer++;
      OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
    }
  }
}
    
void LisaEncoding::LisaEncoder::write(float value) throw(IOException) {
  FloatingPoint::IEEE754SinglePrecision temp = value;
  OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
}

void LisaEncoding::LisaEncoder::write(
  const double* buffer, unsigned int size) throw(IOException) {
  if (typeid(FloatingPoint::DoubleRepresentation) ==
      typeid(FloatingPoint::Representation::IEEE754DoublePrecision)) {
    OrbEncoder::push(Cast::pointer<const uint8*>(buffer), size * sizeof(float));
  } else {
    const double* end = buffer + size;
    while (buffer != end) {
      FloatingPoint::IEEE754DoublePrecision temp = *buffer++;
      OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
    }
  }
}

void LisaEncoding::LisaEncoder::write(double value) throw(IOException) {
  FloatingPoint::IEEE754DoublePrecision temp = value;
  OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
}

void LisaEncoding::LisaEncoder::write(
  const long double* buffer, unsigned int size) throw(IOException) {
  if (typeid(FloatingPoint::LongDoubleRepresentation) ==
      typeid(FloatingPoint::Representation::IEEEExtendedDoublePrecision96)) {
    OrbEncoder::push(Cast::pointer<const uint8*>(buffer), size * sizeof(float));
  } else {
    const long double* end = buffer + size;
    while (buffer != end) {
      FloatingPoint::IEEEExtendedDoublePrecision96 temp = *buffer++;
      OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
    }
  }
}

void LisaEncoding::LisaEncoder::write(long double value) throw(IOException) {
  FloatingPoint::IEEEExtendedDoublePrecision96 temp = value;
  OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
}

Reference<OrbDecoder> LisaEncoding::getDecoder(
  ProtectedPointer<Orb> orb) const throw(MemoryException) {
  return new LisaDecoder(orb);
}

Reference<OrbEncoder> LisaEncoding::getEncoder(
  ProtectedPointer<Orb> orb) const throw(MemoryException) {
  return new LisaEncoder(orb);
}

String LisaEncoding::getUrn() const throw() {
  return Literal("urn:rmi:orb:encoding-scheme:Lisa");
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
