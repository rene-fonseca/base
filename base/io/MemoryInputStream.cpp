/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/MemoryInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int MemoryInputStream::read(uint8* dest, unsigned int size, bool nonblocking) noexcept
{
  if (!dest) {
    return 0;
  }
  if (!src) {
    _throw IOException("Reading beyond eof.");
  }
  unsigned int count = minimum<unsigned int>(size, end - src);
  copy(dest, src, count);
  src += count;
  return count;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
