/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Adler32.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Adler32::Adler32() throw() {
}

unsigned int Adler32::push(const uint8* buffer, unsigned int size) throw() {
  const uint8* const end = buffer + size;
  uint32 s1 = checksum & 0xffff;
  uint32 s2 = (checksum >> 16) & 0xffff;
  while (buffer < end) {
    s1 = (s1 + *buffer++) % BASE;
    s2 = (s2 + s1) % BASE;
  }
  checksum = (s2 << 16) + s1;
  totalSize += size;
  return size;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
