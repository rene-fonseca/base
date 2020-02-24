/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/UnsignedInteger.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int UnsignedInteger::parse(const char* src, const char* end, unsigned int flags)
{
  unsigned int base = 10; // default integer base
  
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }
  
  bassert(
    src != end,
    InvalidFormat("Not an integer.", Type::getType<UnsignedInteger>())
  ); // do not accept empty strings

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
    // char first = *i;
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
      InvalidFormat("Not an integer.", Type::getType<UnsignedInteger>())
    );
  }

  unsigned int highLimit = MAXIMUM/base;
  unsigned int lowLimit = MAXIMUM%base;
  unsigned int temp = 0;
  bassert( // make sure we have at least one digit
    ASCIITraits::isHexDigit(*src),
    InvalidFormat("Not an integer.", Type::getType<UnsignedInteger>())
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
      InvalidFormat("Not an integer.", Type::getType<UnsignedInteger>())
    );
    temp = temp * base + digitValue;
    ++src;
  }

  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }
  
  bassert(src == end, InvalidFormat("Not an integer.", Type::getType<UnsignedInteger>()));
  
  return temp;
}

Validified<unsigned int> UnsignedInteger::parseNoThrow(const char* src, const char* end, unsigned int flags) noexcept
{
  unsigned int base = 10; // default integer base
  
  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) {
      ++src; // eat space
    }
  }
  
  if (src == end) {
    return UNSPECIFIED;
  } // do not accept empty strings

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
    // char first = *i;
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
    if (!(((base == 2) && (flags & BIN)) ||
          ((base == 8) && (flags & OCT)) ||
          ((base == 10) && (flags & DEC)) ||
          ((base == 16) && (flags & HEX)))) {
      return UNSPECIFIED;
    }
  }

  unsigned int highLimit = MAXIMUM/base;
  unsigned int lowLimit = MAXIMUM%base;
  unsigned int temp = 0;
  if (!ASCIITraits::isHexDigit(*src)) { // make sure we have at least one digit
    return UNSPECIFIED;
  }
  while (src != end) {
    char ch = *src;
    if (!ASCIITraits::isHexDigit(ch)) {
      break;
    }
    unsigned char digitValue = ASCIITraits::digitToValue(ch); // unspecified for non-digit
    if (!(ASCIITraits::isHexDigit(ch) &&
          (digitValue < base) &&
          ((temp < highLimit) || ((temp == highLimit) && (digitValue <= lowLimit))))) {
      return UNSPECIFIED;
    }
    temp = temp * base + digitValue;
    ++src;
  }

  if (flags & FLAG_ALLOW_SPACES) {
    while ((src != end) && (*src == ' ')) { // rest must be spaces
      ++src;
    }
  }
  
  if (src != end) {
    return UNSPECIFIED;
  }
  
  return temp;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
