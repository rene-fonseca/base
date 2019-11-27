/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/UnsignedLongInteger.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned long long UnsignedLongInteger::parse(const char* src, const char* end, unsigned int flags) throw(InvalidFormat)
{
  unsigned int base = 10; // default integer base

  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }
  
  // do not accept empty strings
  bassert(
    src != end,
    InvalidFormat("String is empty", Type::getType<UnsignedLongInteger>())
  );
  
  switch (flags & (BIN|DEC|OCT|HEX)) {
  case BIN:
    base = 2;
    break;
  case OCT:
    base = 8;
    break;
  case DEC:
    base = 10;
    break;
  case HEX:
    base = 16;
    break;
  default:
    flags |= PREFIX; // ambiguous unless integer base is determined by prefix
  }

  if (flags & PREFIX) {
    // determine base by looking at prefix (0b: binary, 0x: hex, 0: octal, other: decimal)
    if (*src == '0') { // is binary, octal, or hex
      ++src;
      if (src != end) { // has second char
        char second = ASCIITraits::toLower(*src);
        if (second == 'b') {
          ++src;
          base = 2;
        } else if (second == 'x') {
          ++src;
          base = 16;
        } else {
          --src; // important 'cause we want to accept "0"
          base = (flags & OCT) ? 8 : 10; // default is octal within this content (but use DEC if disallowed)
        }
      } else {
        --src; // important 'cause we want to accept "0"
        base = (flags & OCT) ? 8 : 10; // default is octal within this content (but use DEC if disallowed)
      }
    }

    // check if base is allowed
    bassert(
      ((base == 2) && (flags & BIN)) ||
      ((base == 8) && (flags & OCT)) ||
      ((base == 10) && (flags & DEC)) ||
      ((base == 16) && (flags & HEX)),
      InvalidFormat("Invalid base", Type::getType<UnsignedLongInteger>())
    );
  }

  unsigned long long highLimit = MAXIMUM/base;
  unsigned long long lowLimit = MAXIMUM%base;
  unsigned long long temp = 0;
  bassert( // make sure we have at least one digit
    ASCIITraits::isHexDigit(*src),
    InvalidFormat("No digits", Type::getType<UnsignedLongInteger>())
  );
  while (src != end) {
    char ch = *src;
    if (!ASCIITraits::isHexDigit(ch)) {
      break;
    }
    unsigned char digitValue = ASCIITraits::digitToValue(ch); // unspecified for non-digit
    bassert(
      ASCIITraits::isHexDigit(ch) &&
      (digitValue < base) &&
      ((temp < highLimit) || ((temp == highLimit) && (digitValue <= lowLimit))),
      InvalidFormat("Out of range", Type::getType<UnsignedLongInteger>())
    );
    temp = temp * base + digitValue;
    ++src;
  }
  
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }
  
  bassert(src == end, InvalidFormat("Not a digit", Type::getType<UnsignedLongInteger>()));
  
  return temp;
}

FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long& value) throw(InvalidFormat, IOException)
{
  value = UnsignedLongInteger::parse(stream.getWord(), UnsignedLongInteger::ANY);
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
