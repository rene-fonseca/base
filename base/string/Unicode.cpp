/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Unicode.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MemoryDiff Unicode::getUTF8StringLength(const uint8* src, const uint8* end) noexcept
{
  MemoryDiff length = 0;
  while (src != end) {
    ucs4 ch = 0;
    auto status = readUCS4(src, end, ch);
    if (status > 0) {
      src += status;
      ++length;
      continue;
    }
    return status;
  }
  return length;
}

MemoryDiff Unicode::getUTF8StringLength(const uint8* src) noexcept
{
  if (!src || !*src) {
    return 0;
  }
  MemoryDiff length = 0;
  while (*src) {
    ucs4 ch = 0;
    auto status = readUCS4(src, ch);
    if (status > 0) {
      src += status;
      ++length;
      continue;
    }
    return status;
  }
  return length;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
