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

#include <base/rmi/Encoder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: need low-level push pull interface

class _COM_AZURE_DEV__BASE__API LittleEndianEncoder : public Encoder {
private:
  
  // output stream
public:
  
  static const unsigned int BUFFER_SIZE = 4096;
  
  // TAG: need preallocated buffers (one for each primitive)
  // Allocator<LittleEndian<uint32> > buffer;
  
  /**
    Encodes the elements of an array of unsigned int.
  */
  inline void write(const unsigned int* buffer, unsigned int size) {
    LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
    while (size) {
      unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
      move<LittleEndian<uint32> >(encodedBuffer, buffer, partialSize);
      write(Cast::getAddress(buffer), partialSize * sizeof(LittleEndian<int>));
      size -= partialSize;
    }
  }

  inline Stream& operator<<(Stream& stream, const String& value) {
    LittleEndian<uint32> buffer = value.getLength();
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, const WideString& value) {
    LittleEndian<uint32> buffer = value.getLength();
    // TAG: use buffer if not native byte order
    stream.write(value.getBytes(), sizeof(uint32));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, char value) {
    LittleEndian<int8> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned short value) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }

  inline Stream& operator<<(Stream& stream, short value) {
    LittleEndian<int16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned short value) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, int value) {
    LittleEndian<int32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned int value) {
    LittleEndian<uint32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long value) {
    LittleEndian<int32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
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
