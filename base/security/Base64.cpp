/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Base64.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const char Base64::DIGITS[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
  'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
  'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', '+', '/'
};

const signed char Base64::VALUES[256] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, 62, -1, -1, -1, 63, // + and /
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // 0-9
  -1, -1, -1, -1, -1, -1, -1,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // A-J
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, // K-T
  20, 21, 22, 23, 24, 25, // U-Z
  -1, -1, -1, -1, -1, -1,
  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, // a-j
  36, 37, 38, 39, 40, 41, 42, 43, 44, 45, // k-t
  46, 47, 48, 49, 50, 51, // u-z
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1
};

// int Base64::valueToDigit(char digit) noexcept {
//   if (ASCIITraits::isUpper(digit)) {
//     return (digit - 'A') + 0;
//   } else if (ASCIITraits::isLower(digit)) {
//     return (digit - 'a') + 26;
//   } else if (ASCIITraits::isDigit(digit)) {
//     return (digit - '0') + 52;
//   } else if (digit == '+') {
//     return 62;
//   } else if (digit == '/') {
//     return 63;
//   } else {
//     return -1; // not valid
//   }
// }

String Base64::encode(const uint8* buffer, MemorySize size) noexcept
{
  unsigned int length = (size * 8 + 5) / 6;
  String result(length);
  result.forceToLength(length);
  String::Iterator i = result.getBeginIterator();
  const String::Iterator end = result.getEndIterator();

  unsigned int bitBuffer = 0;
  unsigned int bitsInBuffer = 0;

  while (i < end) {
    if (bitsInBuffer < 6) {
      switch (minimum<MemorySize>(size, 3U)) { // read as many octets as possible
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

FormatOutputStream& operator<<(FormatOutputStream& stream, const Base64::Descriptor& value) throw(IOException)
{
  const char* src = value.getBytes();
  const unsigned int size = value.getSize();

  const unsigned int CHARACTERS_PER_LINE = 76;
  const unsigned int BITS_PER_CHARACTER = 6;
  const unsigned int BITS_PER_LINE = CHARACTERS_PER_LINE * BITS_PER_CHARACTER;
  ASSERT(BITS_PER_LINE % 8 == 0);
  const unsigned int BYTES_PER_LINE = BITS_PER_LINE;
  
  unsigned int lines = size/BYTES_PER_LINE; // whole lines

  char line[CHARACTERS_PER_LINE];
  const char* end = line + CHARACTERS_PER_LINE;
  while (lines) {
    char* i = line;
    while (i < end) {
      // 24 bit quantum
      unsigned int bitBuffer =
        (static_cast<unsigned int>(src[2]) << 16) + (static_cast<unsigned int>(src[1]) << 8) + src[0];
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      bitBuffer >>= BITS_PER_CHARACTER;
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      bitBuffer >>= BITS_PER_CHARACTER;
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      src += 3;
    }
    stream.addCharacterField(line, CHARACTERS_PER_LINE);
    stream << CRLF;
    --lines;
  }

  unsigned int rest = size%BYTES_PER_LINE;
  if (rest) {
    char* i = line;
    const char* end = i + rest/3;
    while (i < end) {
      // 24 bit quantum
      unsigned int bitBuffer =
        (static_cast<unsigned int>(src[2]) << 16) + (static_cast<unsigned int>(src[1]) << 8) + src[0];
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      bitBuffer >>= BITS_PER_CHARACTER;
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      bitBuffer >>= BITS_PER_CHARACTER;
      *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
      src += 3;
    }

    // handle last quantum
    switch (rest%3) {
    case 1: // last 8 bit
      {
        unsigned int bitBuffer = src[0];
        *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
        *i++ = Base64::PAD;
        *i++ = Base64::PAD;
      }
      break;
    case 2: // last 16 bit
      {
        unsigned int bitBuffer = (static_cast<unsigned int>(src[1]) << 8) + src[0];
        *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
        bitBuffer >>= BITS_PER_CHARACTER;
        *i++ = Base64::valueToDigit(bitBuffer & ((1 << BITS_PER_CHARACTER) - 1));
        *i++ = Base64::PAD;
      }
      break;
    case 0: // no padding
      break;
    }
    
    stream.addCharacterField(line, rest);
    stream << CRLF;
  }
  
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
