/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/BarnieDecoder.h>
#include <base/ByteOrder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void BarnieDecoder::read(unsigned int* buffer, unsigned int size) throw(IOException) {
  LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
  while (size) {
    const unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
    read(reinterpret_cast<unsigned int*>(buffer), partialSize * sizeof(LittleEndian<int>));
    move<unsigned int>(buffer, reinterpret_cast<const uint32*>(encodedBuffer), partialSize);
    size -= partialSize;
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
