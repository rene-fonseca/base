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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

unsigned long long UnsignedLongInteger::parse(
  const String& string, unsigned int accept) throw(InvalidFormat) {
  unsigned int base = 10; // default integer base
  String::ReadIterator i = string.getBeginReadIterator();
  const String::ReadIterator end = string.getEndReadIterator();

  while ((i < end) && (*i == ' ')) {
    ++i; // eat space
  }

  // do not accept empty strings
  bassert(
    i < end,
    InvalidFormat("String is empty", Type::getType<UnsignedLongInteger>())
  );
  
  switch (accept) {
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
    accept |= PREFIX; // ambiguous unless integer base is determined by prefix
  }

  if (accept & PREFIX) {
    // determine base by looking at prefix (0b: binary, 0x: hex, 0: octal, other: decimal)
    if (*i == '0') { // is binary, octal, or hex
      ++i;
      if (i < end) { // has second char
        char second = ASCIITraits::toLower(*i);
        if (second == 'b') {
          ++i;
          base = 2;
        } else if (second == 'x') {
          ++i;
          base = 16;
        } else {
          --i; // important 'cause we want to accept "0"
          base = (accept & OCT) ? 8 : 10; // default is octal within this content (but use DEC if disallowed)
        }
      } else {
        --i; // important 'cause we want to accept "0"
        base = (accept & OCT) ? 8 : 10; // default is octal within this content (but use DEC if disallowed)
      }
    }

    // check if base is allowed
    bassert(
      ((base == 2) && (accept & BIN)) ||
      ((base == 8) && (accept & OCT)) ||
      ((base == 10) && (accept & DEC)) ||
      ((base == 16) && (accept & HEX)),
      InvalidFormat("Invalid base", Type::getType<UnsignedLongInteger>())
    );
  }

  unsigned long long highLimit = MAXIMUM/base;
  unsigned long long lowLimit = MAXIMUM%base;
  unsigned long long temp = 0;
  bassert( // make sure we have at least one digit
    ASCIITraits::isHexDigit(*i),
    InvalidFormat("No digits", Type::getType<UnsignedLongInteger>())
  );
  while (i < end) {
    char ch = *i;
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
    ++i;
  }
  
  while ((i < end) && (*i == ' ')) { // rest must be spaces
    ++i;
  }
  
  bassert(i == end, InvalidFormat("Not a digit", Type::getType<UnsignedLongInteger>()));
  
  return temp;
}

FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long& value) throw(InvalidFormat, IOException) {
  value = UnsignedLongInteger::parse(stream.getWord(), UnsignedLongInteger::ANY);
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
