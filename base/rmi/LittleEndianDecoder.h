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

#include <base/rmi/Decoder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API LittleEndianDecoder : public Decoder {
private:

  // input stream
public:
  
  static const unsigned int BUFFER_SIZE = 4096;
  
  // TAG: need preallocated buffers (one for each primitive)
  // Allocator<LittleEndian<uint32> > buffer;
  
  /**
    Decodes the elements of an array of unsigned int.
  */
  inline void read(unsigned int* buffer, unsigned int size) {
    LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
    while (size) {
      unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
      read(Cast::getAddress(buffer), partialSize * sizeof(LittleEndian<int>));
      move<unsigned int>(buffer, encodedBuffer, partialSize);
      size -= partialSize;
    }
  }

  inline Stream& operator>>(Stream& stream, const String& value) {
    LittleEndian<uint32> buffer = value.getLength();
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator>>(Stream& stream, const WideString& value) {
    LittleEndian<uint32> buffer = value.getLength();
    // TAG: use buffer if not native byte order
    stream.write(value.getBytes(), sizeof(uint32));
    return stream;
  }
  
  inline Stream& operator>>(Stream& stream, char value) {
    LittleEndian<int8> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }
  
  inline Stream& operator>>(Stream& stream, unsigned short value) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }

  inline Stream& operator>>(Stream& stream, short value) {
    LittleEndian<int16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }
  
  inline Stream& operator>>(Stream& stream, unsigned short value) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }
  
  inline int read(int value) {
    LittleEndian<int32> buffer;
    stream.read(Cast::getCharAddress(buffer), sizeof(buffer));
    
  }
  
  inline Stream& operator<<(Stream& stream, unsigned int value) {
    LittleEndian<uint32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }
  
  inline Stream& operator<<(Stream& stream, long value) {
    LittleEndian<int32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
  }
  
  inline Stream& operator<<(Stream& stream, unsigned long value) {
    LittleEndian<uint32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long long value) {
    LittleEndian<int64> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned long long value) {
    LittleEndian<uint64> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, float value) {
    float buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }

  inline Stream& operator<<(Stream& stream, double value) {
    double buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long double value) {
    long double buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
