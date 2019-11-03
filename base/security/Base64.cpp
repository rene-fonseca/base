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
#include <base/UnitTest.h>

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

String Base64::encode(const uint8* src, MemorySize size) noexcept
{
  // PADDING can be optional depending on use case
  const MemorySize length = (((size * 8 + (BITS_PER_CHARACTER - 1)) / BITS_PER_CHARACTER + 3)/4 * 4); // round up to 4 digits with padding
  String result;
  result.forceToLength(length);
  auto dest = result.getBeginIterator();
  // left to right for bits
  while (size >= 3) { // read all 3 octet chunks - 24 bits
    const uint32 bitBuffer = (static_cast<uint32>(src[0]) << 16) |
      (static_cast<uint32>(src[1]) << 8) |
      static_cast<uint32>(src[2]);
    src += 3;
    size -= 3;
    *dest++ = DIGITS[(bitBuffer >> (3 * BITS_PER_CHARACTER)) & 0x3f];
    *dest++ = DIGITS[(bitBuffer >> (2 * BITS_PER_CHARACTER)) & 0x3f];
    *dest++ = DIGITS[(bitBuffer >> (1 * BITS_PER_CHARACTER)) & 0x3f];
    *dest++ = DIGITS[(bitBuffer >> (0 * BITS_PER_CHARACTER)) & 0x3f];
  }

  BASSERT(size < 3);
  switch (size) {
  case 1: // in 8 bit => out 12 bit
    {
      const uint32 bitBuffer = static_cast<uint32>(src[0]) << 4; // 4 bits are zero
      *dest++ = DIGITS[(bitBuffer >> (1 * BITS_PER_CHARACTER)) & 0x3f];
      *dest++ = DIGITS[(bitBuffer >> (0 * BITS_PER_CHARACTER)) & 0x3f];
      *dest++ = Base64::PAD;
      *dest++ = Base64::PAD;
    }
    break;
  case 2: // in 16 bit => out 18 bit
    {
      const uint32 bitBuffer = ((static_cast<uint32>(src[0]) << 8) | static_cast<uint32>(src[1])) << 2;
      *dest++ = DIGITS[(bitBuffer >> (2 * BITS_PER_CHARACTER)) & 0x3f];
      *dest++ = DIGITS[(bitBuffer >> (1 * BITS_PER_CHARACTER)) & 0x3f];
      *dest++ = DIGITS[(bitBuffer >> (0 * BITS_PER_CHARACTER)) & 0x3f];
      *dest++ = Base64::PAD;
    }
    break;
  case 0: // no padding
    break;
  }
  BASSERT(dest == result.getEndIterator());
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

  unsigned int rest = size % BYTES_PER_LINE;
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
    switch (rest % 3) {
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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Base64) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/security");
  TEST_IMPACT(SECURITY);

  String getBase64(const uint8* buffer, MemorySize size)
  {
    return Base64::encode(buffer, size);
  }

  String getBase64(const char* text)
  {
    return getBase64(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
  }

  void run() override
  {
    TEST_EQUAL(getBase64(""), "");
    TEST_EQUAL(getBase64(" "), "IA==");
    TEST_EQUAL(getBase64("abc"), "YWJj");
    TEST_EQUAL(getBase64("abcdefghijklmnopqrstuvwxyz"), "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=");    
    TEST_EQUAL(getBase64("The quick brown fox jumps over the lazy dog"), "VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw==");
  }
};

TEST_REGISTER(Base64);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
