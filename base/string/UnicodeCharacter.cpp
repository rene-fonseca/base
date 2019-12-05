/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/UnicodeCharacter.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, UnicodeCharacter character)
{
  char buffer[sizeof("&#4294967295;")]; // worst case length
  char* dest = buffer;
  unsigned int numberOfDigits = 1;
  unsigned int value = character.getCode();
  
  switch (character.getStyle()) {
  case UnicodeCharacter::STYLE_XML:
    if (stream.getFlags() & FormatOutputStream::Symbols::DECIMAL) {
      *dest++ = '&';
      *dest++ = '#';
      unsigned int temp = value/10;
      while (temp) {
        temp /= 10;
        ++numberOfDigits;
      }
      temp = value;
      for (int i = numberOfDigits - 1; i >= 0; --i) {
        dest[i] = ASCIITraits::valueToDigit(temp % 10);
        temp /= 10;
      }
      dest += numberOfDigits;
      *dest++ = ';';
    } else {
      *dest++ = '&';
      *dest++ = '#';
      *dest++ = 'x';
      for (unsigned int i = 8; i > 1; --i) {
        if (value >> ((i - 1) * 4)) {
          numberOfDigits = i;
          break;
        }
      }
      for (unsigned int j = numberOfDigits; j > 1; --j) {
        *dest++ = ASCIITraits::valueToDigit((value >> ((j - 1) * 4)) & 0xf);
      }
      *dest++ = ';';
    }
    break;
  case UnicodeCharacter::STYLE_PERL:
    *dest++ = '\\';
    *dest++ = 'x';
    *dest++ = '{';
    for (unsigned int i = 8; i > 1; --i) {
      if (value >> ((i - 1) * 4)) {
        numberOfDigits = i;
        break;
      }
    }
    for (unsigned int j = numberOfDigits; j > 1; --j) {
      *dest++ = ASCIITraits::valueToDigit((value >> ((j - 1) * 4)) & 0xf);
    }
    *dest++ = '}';
    break;
  case UnicodeCharacter::STYLE_CPP:
  default:
    *dest++ = '\\';
    *dest++ = 'u';
    for (unsigned int i = 8; i > 1; --i) {
      if (value >> ((i - 1) * 4)) {
        numberOfDigits = i;
        break;
      }
    }
    for (unsigned int j = numberOfDigits; j > 1; --j) {
      *dest++ = ASCIITraits::valueToDigit((value >> ((j - 1) * 4)) & 0xf);
    }
  }
  
  stream.addCharacterField(buffer, dest - buffer);
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
