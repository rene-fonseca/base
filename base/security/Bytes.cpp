/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Bytes.h>
#include <base/string/ASCIITraits.h>
#include <base/security/Base64.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Bytes::getAsHex(const uint8* src, MemorySize size)
{
  String result(size * 2);
  result.forceToLength(size * 2);
  String::Iterator dest = result.getBeginIterator();
  const uint8* end = src + size;
  while (src != end) {
    const uint8 byte = *src++;
    *dest++ = ASCIITraits::valueToDigit((byte >> 4) & 0x0f); // high-order digit
    *dest++ = ASCIITraits::valueToDigit((byte >> 0) & 0x0f); // low-order digit
  }
  return result;
}

String Bytes::getAsBase64(const uint8* src, MemorySize size)
{
  return Base64::encode(src, size);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
