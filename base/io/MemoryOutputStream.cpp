/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/MemoryOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int MemoryOutputStream::write(const uint8* src, unsigned int size, bool nonblocking)
{
  if (closed) {
    _throw IOException("Stream is closed.");
  }
  if (buffer.getSize() < (bytesRead + size)) {
    static constexpr MemorySize GRANULARITY = 16 * 1024;
    buffer.ensureCapacity(
      maximum(buffer.getSize() * 2, ((bytesRead + size) + GRANULARITY - 1)/GRANULARITY * GRANULARITY)
    );
  }
  buffer.setSize(bytesRead + size);
  uint8* dest = buffer.getElements();
  copy(dest + bytesRead, src, size);
  bytesRead += size;
  return size;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
