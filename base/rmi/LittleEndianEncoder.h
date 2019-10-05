/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__LITTLE_ENDIAN_ENCODER_H
#define _DK_SDU_MIP__BASE_RMI__LITTLE_ENDIAN_ENCODER_H

#include <base/rmi/Encoder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: need low-level push pull interface

class LittleEndianEncoder : public Encoder {
private:
  
  // output stream
public:
  
  static const unsigned int BUFFER_SIZE = 4096;
  
  // TAG: need preallocated buffers (one for each primitive)
  // Allocator<LittleEndian<uint32> > buffer;
  
  /**
    Encodes the elements of an array of unsigned int.
  */
  inline void write(const unsigned int* buffer, unsigned int size) throw(IOException) {
    LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
    while (size) {
      unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
      move<LittleEndian<uint32> >(encodedBuffer, buffer, partialSize);
      write(Cast::getAddress(buffer), partialSize * sizeof(LittleEndian<int>));
      size -= partialSize;
    }
  }

  inline Stream& operator<<(Stream& stream, const String& value) throw(IOException) {
    LittleEndian<uint32> buffer = value.getLength();
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, const WideString& value) throw(IOException) {
    LittleEndian<uint32> buffer = value.getLength();
    // TAG: use buffer if not native byte order
    stream.write(value.getBytes(), sizeof(uint32));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, char value) throw(IOException) {
    LittleEndian<int8> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned short value) throw(IOException) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }

  inline Stream& operator<<(Stream& stream, short value) throw(IOException) {
    LittleEndian<int16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned short value) throw(IOException) {
    LittleEndian<uint16> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, int value) throw(IOException) {
    LittleEndian<int32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned int value) throw(IOException) {
    LittleEndian<uint32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long value) throw(IOException) {
    LittleEndian<int32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned long value) throw(IOException) {
    LittleEndian<uint32> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long long value) throw(IOException) {
    LittleEndian<int64> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, unsigned long long value) throw(IOException) {
    LittleEndian<uint64> buffer = value;
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, float value) throw(IOException) {
    float buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }

  inline Stream& operator<<(Stream& stream, double value) throw(IOException) {
    double buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
  
  inline Stream& operator<<(Stream& stream, long double value) throw(IOException) {
    long double buffer = value; // TAG: fixme
    stream.write(Cast::getCharAddress(buffer), sizeof(buffer));
    return stream;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
