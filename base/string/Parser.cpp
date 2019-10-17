/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Parser.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MemorySize Parser::getNumberOfUTF8Bytes() const noexcept
{
  return 0;
}

ucs4 Parser::readUCS4() {
  if (src == end) {
    throw ParseException("Unexpected end reached.");
  }
  const char result = *src;
  // TAG: impl
  ++src;
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
