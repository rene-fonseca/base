/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Base64.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const char Base64::DIGITS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz"
                              "0123456789+/";

int Base64::valueToDigit(char digit) throw() {
  if (ASCIITraits::isUpper(digit)) {
    return (digit - 'A') + 0;
  } else if (ASCIITraits::isLower(digit)) {
    return (digit - 'a') + 26;
  } else if (ASCIITraits::isDigit(digit)) {
    return (digit - '0') + 52;
  } else if (digit == '+') {
    return 62;
  } else if (digit == '/') {
    return 63;
  } else {
    return -1; // not valid
  }
}

String Base64::encode(const byte* buffer, unsigned int size) throw() {
  unsigned int length = (size * 8 + 5) / 6;
  String result(length);
  result.forceToLength(length);
  String::Iterator i = result.getBeginIterator();
  const String::Iterator end = result.getEndIterator();

  unsigned int bitBuffer = 0;
  unsigned int bitsInBuffer = 0;

  while (i < end) {
    if (bitsInBuffer < 6) {
      switch (minimum(size, 3U)) { // read as many octets as possible
      case 3:
        bitBuffer |= *buffer++ << bitsInBuffer;
        bitsInBuffer += 8;
        --size; // yes no break here
      case 2:
        bitBuffer |= *buffer++ << bitsInBuffer;
        bitsInBuffer += 8;
        --size; // yes no break here
      case 1:
        bitBuffer |= *buffer++ << bitsInBuffer;
        bitsInBuffer += 8;
        --size;
      }
      // do nothing if no more octets - while loop knows when to stop
    }
    *i++ = valueToDigit(bitBuffer & 0x3f);
    bitBuffer >>= 6; // may not be valid for last digit
    bitsInBuffer -= 6; // may not be valid for last digit
  }
  return result;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
