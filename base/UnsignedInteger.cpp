/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/UnsignedInteger.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

unsigned int UnsignedInteger::parse(const String& str, unsigned int accept) throw(InvalidFormat) {
  unsigned int base = 10; // default integer base
  String::ReadIterator i = str.getBeginReadIterator();
  const String::ReadIterator end = str.getEndReadIterator();

  assert(i < end, InvalidFormat("Not an integer")); // do not accept empty strings

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
    char first = *i;
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
    assert(
      ((base == 2) && (accept & BIN)) ||
      ((base == 8) && (accept & OCT)) ||
      ((base == 10) && (accept & DEC)) ||
      ((base == 16) && (accept & HEX)),
      InvalidFormat("Not an integer")
    );
  }

  unsigned int highLimit = MAXIMUM/base;
  unsigned int lowLimit = MAXIMUM%base;
  unsigned int temp = 0;
  while (i < end) {
    char ch = *i++;
    unsigned char digitValue = ASCIITraits::digitToValue(ch); // unspecified for non-digit
    assert(
      ASCIITraits::isHexDigit(ch) &&
      (digitValue < base) &&
      ((temp < highLimit) || ((temp == highLimit) && (digitValue <= lowLimit))),
      InvalidFormat("Not an integer")
    );
    temp = temp * base + digitValue;
  }
  return temp;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
