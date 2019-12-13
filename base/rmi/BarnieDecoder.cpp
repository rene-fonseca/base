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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void BarnieDecoder::read(unsigned int* buffer, unsigned int size) {
  LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
  while (size) {
    const unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
    read(reinterpret_cast<unsigned int*>(buffer), partialSize * sizeof(LittleEndian<int>));
    move<unsigned int>(buffer, reinterpret_cast<const uint32*>(encodedBuffer), partialSize);
    size -= partialSize;
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
