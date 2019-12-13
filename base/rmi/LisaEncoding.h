/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/rmi/OrbEncoding.h>
#include <base/mem/Allocator.h>
#include <base/ByteOrder.h>
#include <base/Architecture.h>
#include <base/mem/ProtectedPointer.h>
#include <base/rmi/Orb.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker encoding scheme. Integral types are encoded in little
  endian byte order and the floating-point types float, double, and long double
  are encoded in IEEE 754 single, IEEE 754 double, and IEEE extended double
  precision (96 bit), respectively.
  
  @short Object Request Broker Lisa encoding scheme.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LisaEncoding : public OrbEncoding {
public:
  
  class LisaDecoder : public OrbDecoder {
  public:

    inline LisaDecoder(ProtectedPointer<Orb> orb) noexcept : OrbDecoder(orb) {
    }
    
    /**
      Decodes a reference.
    */
    void read(OrbReference& value);
  
    /**
      Decodes a string.
    */
    void read(String& value);
  
    /**
      Decodes a wide string.
    */
    void read(WideString& value);
  
    /**
      Decodes an array of chars.
    */
    void read(char* buffer, unsigned int size);
  
    /**
      Decodes a char.
    */
    void read(char& value);
  
    /**
      Decodes an array of signed short integers.
    */
    void read(short* buffer, unsigned int size);
  
    /**
      Decodes a signed short integer.
    */
    void read(short& value);
  
    /**
      Decodes an array of unsigned short integers.
    */
    void read(unsigned short* buffer, unsigned int size);
  
    /**
      Decodes a unsigned short integer.
    */
    void read(unsigned short&  value);
  
    /**
      Decodes an array of signed integers.
    */
    void read(int* buffer, unsigned int size);

    /**
      Decodes a signed integer.
    */
    void read(int& value);
  
    /**
      Decodes an array of unsigned integers.
    */
    void read(unsigned int* buffer, unsigned int size);

    /**
      Decodes an unsigned integer.
    */
    void read(unsigned int& value);
  
    /**
      Decodes an array of signed long integers.
    */
    void read(long* buffer, unsigned int size);

    /**
      Decodes a signed long integer.
    */
    void read(long& value);
  
    /**
      Decodes an array of unsigned long integers.
    */
    void read(unsigned long* buffer, unsigned int size);

    /**
      Decodes an unsigned long integer.
    */
    void read(unsigned long& value);
  
    /**
      Decodes an array of signed long long integers.
    */
    void read(long long* buffer, unsigned int size);

    /**
      Decodes a signed long long integer.
    */
    void read(long long& value);
  
    /**
      Decodes an array of unsigned long long integers.
    */
    void read(unsigned long long* buffer, unsigned int size);
  
    /**
      Decodes an unsigned long long interger.
    */
    void read(unsigned long long& value);
  
    /**
      Decodes an array of float.
    */
    void read(float* buffer, unsigned int size);
  
    /**
      Decodes a float.
    */
    void read(float& value);
  
    /**
      Decodes an array of double.
    */
    void read(double* buffer, unsigned int size);
  
    /**
      Decodes a double.
    */
    void read(double& value);
  
    /**
      Decodes an array of long double.
    */
    void read(long double* buffer, unsigned int size);
  
    /**
      Decodes a long double.
    */
    void read(long double& value);
  };
  
  class LisaEncoder : public OrbEncoder {
  private:

    template<class DEST, class SRC>
    inline void pushValue(SRC value) {
      LittleEndian<DEST> temp = value;
      OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
    }
    
    template<class DEST, class SRC>
    inline void pushArray(
      const SRC* buffer, unsigned int size) {
      if (Architecture::isLittleEndian() && (sizeof(SRC) == sizeof(DEST))) {
        OrbEncoder::push(
          Cast::pointer<const uint8*>(buffer),
          size * sizeof(DEST)
        );
      } else {
        const SRC* end = buffer + size;
        while (buffer != end) {
          LittleEndian<DEST> temp = *buffer++;
          OrbEncoder::push(Cast::getAddress(temp), sizeof(temp));
        }
      }
    }
  public:
    
    /**
      Initializes the encoder.
    */
    inline LisaEncoder(ProtectedPointer<Orb> _orb) noexcept
      : OrbEncoder(orb) {
    }
    
    /**
      Encodes the specified reference.
    */
    void write(const OrbReference& value);
    
    /**
      Encodes the specified string.
    */
    void write(const String& value);
    
    /**
      Encodes the specified wide string.
    */
    void write(const WideString& value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const char* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(char value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const short* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(short value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(
      const unsigned short* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(unsigned short value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const int* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(int value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(
      const unsigned int* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(unsigned int value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const long* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(long value);
  
    /**
      Encodes the elements of the specified array.
    */
    void write(
      const unsigned long* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(unsigned long value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const long long* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(long long value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(
      const unsigned long long* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(unsigned long long value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const float* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(float value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(const double* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(double value);
    
    /**
      Encodes the elements of the specified array.
    */
    void write(
      const long double* buffer, unsigned int size);
    
    /**
      Encodes the specified value.
    */
    void write(long double value);
  };
  
  /**
    Returns a new decoder.
  */
  Reference<OrbDecoder> getDecoder(
    ProtectedPointer<Orb> orb) const;
  
  /**
    Returns a new encoder.
  */
  Reference<OrbEncoder> getEncoder(
    ProtectedPointer<Orb> orb) const;
  
  /**
     Returns the global identifier of the encoding scheme.
  */
  String getUrn() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
