/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/BarnieDecoder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void BarnieDecoder::read(unsigned int* buffer, unsigned int size) throw(IOException) {
  LittleEndian<uint32> encodedBuffer[BUFFER_SIZE];
  while (size) {
    unsigned int partialSize = minimum<unsigned int>(size, getArraySize(encodedBuffer));
    read(Cast::getAddress(buffer), partialSize * sizeof(LittleEndian<int>));
    move<unsigned int>(buffer, encodedBuffer, partialSize);
    size -= partialSize;
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
