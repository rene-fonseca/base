/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/string/WideString.h>
#include <base/Functor.h>
#include <base/Architecture.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <string.h>
  #include <stdlib.h>
#endif // flavor

#include <wchar.h>
#include <limits.h> // defines MB_LEN_MAX

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: add reference to RFC 2781
// TAG: need SCSU support see http://www.unicode.org/unicode/reports/tr6/
// TAG: need support for native unicode encoding (e.g. SYSTEM_UNICODE_ENCODING)?
// TAG: need input support for something like: "asdfd\u015fsdjfhd", "&#xXXXXX", and "&#DDDDD"

#if 0
FormatOutputStream& operator<<(FormatOutputStream& stream, UnicodeCharacter value) throw(IOException) {
  return stream;
}
#endif

StringLiteral WideString::toString(MultibyteEncoding encoding) throw() {
  switch (encoding) {
  case UTF7:
    return MESSAGE("UTF-7");
  case UTF8:
    return MESSAGE("UTF-8");
  case UTF16:
    return MESSAGE("UTF-16");
  case UTF16BE:
    return MESSAGE("UTF-32BE");
  case UTF16LE:
    return MESSAGE("UTF-32LE");
  case UTF32:
    return MESSAGE("UTF-32");
  case UTF32BE:
    return MESSAGE("UTF-32BE");
  case UTF32LE:
    return MESSAGE("UTF-32LE");
  }
}

unsigned int WideString::UCS2ToUTF8(uint8* dest, const ucs2* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs2* end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = 0xef;
      *dest++ = 0xbb;
      *dest++ = 0xbf;
    }
    while (src < end) {
      const ucs2 code = *src++;
      if (code <= 0x0000007fU) {
        *dest++ = code;
      } else if (code <= 0x000007ffU) {
        *dest++ = 0xc0 | (code >> 6); // 5 bit - most significant
        *dest++ = 0x80 | (code >> 0) & ((1 << 6) - 1); // 6 bit
      } else {
        assert(!((code >= 0xdc00) && (code <= 0xdfff)), WideStringException("Reserved code"));
        if ((code >= 0xd800) && (code <= 0xdbff)) {
          assert(src < end, WideStringException("Incomplete sequence"));
          const ucs2 lowSurrogate = *src++;
          assert((lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff), WideStringException("Invalid sequence"));
          const ucs4 ch = 0x00010000U +
            (static_cast<ucs4>(code) - 0xd800) * 0x0400 +
            (static_cast<ucs4>(lowSurrogate) - 0xdc00);
          *dest++ = 0xf0 | (ch >> 18); // 3 bit - most significant
          *dest++ = 0x80 | (ch >> 12) & ((1 << 6) - 1); // 6 bit
          *dest++ = 0x80 | (ch >> 6) & ((1 << 6) - 1); // 6 bit
          *dest++ = 0x80 | (ch >> 0) & ((1 << 6) - 1); // 6 bit
        } else {
          *dest++ = 0xe0 | (code >> 12); // 4 bit - most significant
          *dest++ = 0x80 | (code >> 6) & ((1 << 6) - 1); // 6 bit
          *dest++ = 0x80 | (code >> 0) & ((1 << 6) - 1); // 6 bit
        }
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 3; // <0xef> <0xbb> <0xbf>
    }
    while (src < end) {
      const ucs2 code = *src++;
      if (code <= 0x0000007fU) {
        ++length;
      } else if (code <= 0x000007ffU) {
        length += 2;
      } else {
        assert(!((code >= 0xdc00) && (code <= 0xdfff)), WideStringException("Reserved code"));
        if ((code >= 0xd800) && (code <= 0xdbff)) {
          assert(src < end, WideStringException("Incomplete sequence"));
          const ucs2 lowSurrogate = *src++;
          assert((lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff), WideStringException("Invalid sequence"));
          length += 4;
        } else {
          length += 3;
        }
      }
    }
    return length;
  }
}

unsigned int WideString::UCS4ToUTF8(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = 0xef;
      *dest++ = 0xbb;
      *dest++ = 0xbf;
    }
    while (src < end) {
      ucs4 value = *src++;
      if (value <= 0x0000007fU) {
        *dest++ = value;
      } else if (value <= 0x000007ffU) {
        *dest++ = 0xc0 | (value >> 6); // 5 bit - most significant
        *dest++ = 0x80 | (value >> 0) & ((1 << 6) - 1); // 6 bit
      } else if (value <= 0x0000ffffU) { // TAG: 0000fffe must never appear?
        *dest++ = 0xe0 | (value >> 12); // 4 bit - most significant
        *dest++ = 0x80 | (value >> 6) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 0) & ((1 << 6) - 1); // 6 bit
      } else if (value <= 0x001fffffU) {
        *dest++ = 0xf0 | (value >> 18); // 3 bit - most significant
        *dest++ = 0x80 | (value >> 12) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 6) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 0) & ((1 << 6) - 1); // 6 bit
      } else if (value <= 0x03ffffffU) {
        *dest++ = 0xf8 | (value >> 24); // 3 bit - most significant
        *dest++ = 0x80 | (value >> 18) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 12) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 6) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 0) & ((1 << 6) - 1); // 6 bit
      } else if (value <= 0x7fffffffU) {
        *dest++ = 0xfc | (value >> 30); // 1 bit - most significant
        *dest++ = 0x80 | (value >> 24) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 18) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 12) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 6) & ((1 << 6) - 1); // 6 bit
        *dest++ = 0x80 | (value >> 0) & ((1 << 6) - 1); // 6 bit
      } else {
        throw WideStringException("Invalid UCS-4 character", Type::getType<WideString>());
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 3; // <0xef> <0xbb> <0xbf>
    }
    while (src < end) {
      ucs4 value = *src++;
      if (value <= 0x0000007fU) {
        ++length;
      } else if (value <= 0x000007ffU) {
        length += 2;
      } else if (value <= 0x0000ffffU) { // TAG: 0000fffe must never appear?
        length += 3;
      } else if (value <= 0x001fffffU) {
        length += 4;
      } else if (value <= 0x03ffffffU) {
        length += 5;
      } else if (value <= 0x7fffffffU) {
        length += 6;
      } else {
        throw WideStringException("Invalid UCS-4 character", Type::getType<WideString>());
      }
    }
    return length;
  }
}

unsigned int WideString::UTF8ToUCS2(ucs2* dest, const uint8* src, unsigned int size, unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }
  if ((size >= 3) && (src[0] == 0xef) && (src[0] == 0xbb) && (src[0] == 0xbf)) {
    if (flags & EAT_BOM) {
      src += 3;
    }
  } else {
    assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
  }
  const ucs2* const begin = dest;
  const uint8* const end = src + size;
  if (dest) {
    while (src < end) {
      uint8 value = *src++;
      if (value & 0x80 == 0x00) {
        *dest++ = value;
      } else if (value & 0xe0 == 0xc0) {
        ucs2 code = value & ~0xe0;
        assert(src < end, MultibyteException("Incomplete code"));
        uint8 temp = *src++;
        assert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
        code = (code << 6) | temp;
        *dest++ = code;
      } else if (value & 0xf0 == 0xe0) {
        ucs2 code = value & ~0xf0;
        for (int i = 0; i < 2; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if (value & 0xf8 == 0xf0) {
        ucs4 code = value & ~0xf8;
        for (int i = 0; i < 3; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
          code = (code << 6) | temp;
        }
        *dest++ = (code - 0x00010000U)/0x400 + 0xd800; // high surrogate
        *dest++ = (code - 0x00010000U)%0x400 + 0xdc00; // low surrogate
      } else if (value & 0xfc == 0xf8) {
        for (int i = 0; i < 4; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else if (value & 0xfe == 0xfc) {
        for (int i = 0; i < 5; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else {
        throw MultibyteException("Invalid code");
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src < end) {
      uint8 value = *src++;
      if (value & 0x80 == 0x00) {
        ++length;
      } else if (value & 0xe0 == 0xc0) {
        assert(src < end, MultibyteException("Incomplete code"));
        assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        ++length;
      } else if (value & 0xf0 == 0xe0) {
        for (int i = 0; i < 2; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        ++length;
      } else if (value & 0xf8 == 0xf0) {
        for (int i = 0; i < 3; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        length += 2;
      } else if (value & 0xfc == 0xf8) {
        for (int i = 0; i < 4; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else if (value & 0xfe == 0xfc) {
        for (int i = 0; i < 5; ++i) {
          assert(src < end, MultibyteException("Incomplete code"));
          assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else {
        throw MultibyteException("Invalid code");
      }
    }
    return length;
  }
}

unsigned int WideString::UTF8ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }
  if ((size >= 3) && (src[0] == 0xef) && (src[0] == 0xbb) && (src[0] == 0xbf)) {
    if (flags & EAT_BOM) {
      src += 3;
    }
  } else {
    assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
  }
  const ucs4* const begin = dest;
  const uint8* const end = src + size;
  if (dest) {
    while (src < end) {
      uint8 value = *src++;
      if ((value & 0x80) == 0x00) {
        *dest++ = value;
      } else if ((value & 0xe0) == 0xc0) {
        ucs4 code = value & ~0xe0;
        assert(src < end, MultibyteException("Incomplete character code"));
        uint8 temp = *src++;
        assert((temp & 0xc0) == 0x80, MultibyteException("Invalid character code"));
        code = (code << 6) | temp;
        *dest++ = code;
      } else if ((value & 0xf0) == 0xe0) {
        ucs4 code = value & ~0xf0;
        for (int i = 0; i < 2; ++i) {
          assert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid character code"));
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xf8) == 0xf0) {
        ucs4 code = value & ~0xf8;
        for (int i = 0; i < 3; ++i) {
          assert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid character code"));
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xfc) == 0xf8) {
        ucs4 code = value & ~0xfc;
        for (int i = 0; i < 4; ++i) {
          assert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid character code"));
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xfe) == 0xfc) {
        ucs4 code = value & ~0xfe;
        for (int i = 0; i < 5; ++i) {
          assert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          assert((temp & 0xc0) == 0x80, MultibyteException("Invalid character code"));
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else {
        throw MultibyteException();
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src < end) {
      unsigned int numberOfOctets;
      uint8 value = *src++;
      if ((value & 0x80) == 0x00) {
        numberOfOctets = 0;
      } else if ((value & 0xe0) == 0xc0) {
        numberOfOctets = 1;
      } else if ((value & 0xf0) == 0xe0) {
        numberOfOctets = 2;
      } else if ((value & 0xf8) == 0xf0) {
        numberOfOctets = 3;
      } else if ((value & 0xfc) == 0xf8) {
        numberOfOctets = 4;
      } else if ((value & 0xfe) == 0xfc) {
        numberOfOctets = 5;
      } else {
        throw MultibyteException("Invalid character code");
      }
      assert(src + numberOfOctets < end, MultibyteException("Incomplete character code"));
      for (int i = 0; i < numberOfOctets; ++i) {
        assert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid character code"));
      }
      ++length;
    }
    return length;
  }
}

// unsigned int UCS2ToUTF16(char* dest, const UCS2* src, unsigned int size) throw() {
//  if ((src == 0) || (size == 0)) {
//    return 0;
//  }
//   const char* const begin = dest;
  
//   return dest - begin;
// }

unsigned int WideString::UCS4ToUTF16BE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = BOM >> 8; // most significant
      *dest++ = BOM >> 0;
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        assert(!((value >= 0x0000d800) && (value <= 0x0000dfff)), WideStringException("Value reserved for UTF-16"));
        *dest++ = value >> 8; // most significant
        *dest++ = value >> 0;
      } else if (value <= 0x0010ffff) {
        uint16 a = (value - 0x00010000)/0x400 + 0xd800; // 10 bit - most significant
        *dest++ = a >> 8; // most significant
        *dest++ = a >> 0;
        uint16 b = (value - 0x00010000)%0x400 + 0xdc00; // 10 bit
        *dest++ = b >> 8;
        *dest++ = b >> 0;
      } else {
        throw WideStringException("Value incompatible with UTF-16", Type::getType<WideString>());     
      }
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 2;
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        assert(!((value >= 0x0000d800) && (value <= 0x0000dfff)), WideStringException("Value reserved for UTF-16"));
        length += 2;
      } else if (value <= 0x0010ffff) {
        assert((value & 0xf) < 0xe, WideStringException("Invalid UCS-4 character", Type::getType<WideString>()));
        length += 4;
      } else {
        throw WideStringException("Value incompatible with UTF-16", Type::getType<WideString>());
      }
    }
    return length;
  }
}

unsigned int WideString::UCS4ToUTF16LE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = BOM >> 8; // most significant
      *dest++ = BOM >> 0;
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        assert(!((value >= 0x0000d800) && (value <= 0x0000dfff)), WideStringException("Value reserved for UTF-16"));
        *dest++ = value >> 0; // least significant
        *dest++ = value >> 8;
      } else if (value <= 0x0010ffff) {
        uint16 a = (value - 0x00010000)/0x400 + 0xd800; // 10 bit - most significant
        *dest++ = a >> 0; // least significant
        *dest++ = a >> 8;
        uint16 b = (value - 0x00010000)%0x400 + 0xdc00; // 10 bit
        *dest++ = b >> 0;
        *dest++ = b >> 8;
      } else {
        throw WideStringException("Value incompatible with UTF-16", Type::getType<WideString>());     
      }
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 2;
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        assert(!((value >= 0x0000d800) && (value <= 0x0000dfff)), WideStringException("Value reserved for UTF-16"));
        length += 2;
      } else if (value <= 0x0010ffff) {
        assert((value & 0xf) < 0xe, WideStringException("Invalid UCS-4 character", Type::getType<WideString>()));
        length += 4;
      } else {
        throw WideStringException("Value incompatible with UTF-16", Type::getType<WideString>());
      }
    }
    return length;
  }
}

unsigned int WideString::UTF16ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }
  
  const ucs4* const begin = dest;
  const uint8* const end = src + size;
  
  assert(size % 2 == 0, MultibyteException("Invalid length of string"));

  if (size == 0) {
    assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
    return 0;
  }

  MultibyteEncoding encoding = UTF8; // invalid
  
  if (flags & ASSUME_NATIVE_BYTE_ORDER) {
    encoding = Architecture::isBigEndian() ? UTF16BE : UTF16LE;
  } else if (flags & ASSUME_BE) {
    encoding = UTF16BE;
  } else if (flags & ASSUME_LE) {
    encoding = UTF16LE;
  } else {
    if ((src[0] == 0xfe) && (src[1] == 0xff)) { // is big endian
      encoding = UTF16BE;
    } else if ((src[0] == 0xff) && (src[1] == 0xfe)) { // is little endian
      encoding = UTF16LE;
    } else { // guess byte order
      assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
      const uint8* s = src;
      while (s < end) {
        uint16 be = (static_cast<uint16>(s[0]) << 8) | (static_cast<uint16>(s[1]) << 0);
        uint16 le = (static_cast<uint16>(s[0]) << 0) | (static_cast<uint16>(s[1]) << 8);
        
        bool validBe = false;
        if (be <= 0xd7ff) {
          validBe = true;
        } else if (be <= 0xdbff) {
          if (src < end) {
            uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
            if ((temp >= 0xdc00) && (temp <= 0xdfff)) {
              be = (be - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
              if ((be <= 0x0000ffff) && ((be & 0xf) < 0xe)) {
                validBe = true;
              }
            }
          }
        } else if (be <= 0xdfff) {
        } else {
          validBe = true;
        }

        bool validLe = false;
        if (le <= 0xd7ff) {
          validLe = true;
        } else if (le <= 0xdbff) {
          if (src < end) {
            uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
            if ((temp >= 0xdc00) && (temp <= 0xdfff)) {
              le = (le - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
              if ((le <= 0x0000ffff) && ((le & 0xf) < 0xe)) {
                validLe = true;
              }
            }
          }
        } else if (le <= 0xdfff) {
        } else {
          validLe = true;
        }

        if (validBe && !validLe) {
          encoding = UTF16BE;
          break;
        } else if (!validBe && validLe) {
          encoding = UTF16LE;
          break;
        } else if (!validBe && !validLe) {
          throw MultibyteException("Invalid UTF-16 encoding");
        }
        
        s += 2;
      }
      if ((encoding == UTF8) && (dest == 0)) {
        return size/2; // only asking for length of string - no BOM is present
      }
      assert((encoding == UTF16BE) || (encoding == UTF16LE), MultibyteException("Ambiguous UTF-16 encoding"));
    }
  }

  if (flags & EAT_BOM) {
    if ((encoding == UTF16BE) && (src[0] == 0xfe) && (src[1] == 0xff)) { // is big endian
      src += 2;
    } else if ((encoding == UTF16LE) && (src[0] == 0xff) && (src[1] == 0xfe)) { // is little endian
      src += 2;
    }
  }
  
  switch (encoding) {
  case UTF16BE:
    if (dest != 0) {
      while (src < end) {
        unsigned int code = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
        if (code <= 0xd7ff) {
          *dest++ = code; // always valid
        } else if (code <= 0xdbff) {
          assert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
          assert((temp >= 0xdc00) && (temp <= 0xdfff), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          code = 0x00010000U + (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00);
          assert((code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          *dest++ = code;
        } else if (code <= 0xdfff) {
          throw MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>());
        } else {
          *dest++ = code;
        }
      }
      return dest - begin;
    } else {
      unsigned int length = 0;
      while (src < end) {
        unsigned int code = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
        if (code <= 0xd7ff) {
        } else if (code <= 0xdbff) {
          assert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
          assert((temp >= 0xdc00) && (temp <= 0xdfff), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          code = (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
          assert((code <= 0x0000ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
        } else if (code <= 0xdfff) {
          throw MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>());
        }
        ++length;
      }
      return length;
    }
    break;
  case UTF16LE:
    if (dest != 0) {
      while (src < end) {
        unsigned int code = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
        if (code <= 0xd7ff) {
          *dest++ = code; // always valid
        } else if (code <= 0xdbff) {
          assert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
          assert((temp >= 0xdc00) && (temp <= 0xdfff), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          code = 0x00010000U + (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00);
          assert((code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          *dest++ = code;
        } else if (code <= 0xdfff) {
          throw MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>());
        } else {
          *dest++ = code;
        }
      }
      return dest - begin;
    } else {
      unsigned int length = 0;
      while (src < end) {
        unsigned int code = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
        if (code <= 0xd7ff) {
        } else if (code <= 0xdbff) {
          assert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
          assert((temp >= 0xdc00) && (temp <= 0xdfff), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          code = (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
          assert((code <= 0x0000ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
        } else if (code <= 0xdfff) {
          throw MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>());
        }
        ++length;
      }
      return length;
    }
    break;
  default:
    throw MultibyteException("Invalid UTF-16 encoding");
  }
}

unsigned int WideString::UCS4ToUTF32BE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  
  const uint8* begin = dest;
  const ucs4* end = src + size;

  if (dest) {
    // TAG: do we need a flag to prevent/allow a redundant BOM (e.g. REDUNDANT_BOM) - also in other methods
    if (flags & ADD_BOM) {
      *dest++ = (BOM >> 24) & 0xff; // most significant
      *dest++ = (BOM >> 16) & 0xff;
      *dest++ = (BOM >> 8) & 0xff;
      *dest++ = (BOM >> 0) & 0xff;
    }
    
    while (src < end) {
      const ucs4 code = *src++;
      if (code <= 0x0010ffff) {
        *dest++ = (code >> 24) & 0xff; // most significant
        *dest++ = (code >> 16) & 0xff;
        *dest++ = (code >> 8) & 0xff;
        *dest++ = (code >> 0) & 0xff;
      } else {
        throw WideStringException("Invalid UCS-4 character code", Type::getType<WideString>());
      }
    }
  } else {
    while (src < end) {
      assert(*src++ <= 0x0010ffff, WideStringException("Invalid UCS-4 character code", Type::getType<WideString>()));
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
  return dest - begin;
}

unsigned int WideString::UCS4ToUTF32LE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;

  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = (BOM >> 0) & 0xff; // least significant
      *dest++ = (BOM >> 8) & 0xff;
      *dest++ = (BOM >> 16) & 0xff;
      *dest++ = (BOM >> 24) & 0xff;
    }
    
    while (src < end) {
      const ucs4 code = *src++;
      if (code <= 0x0010ffff) {
        *dest++ = (code >> 0) & 0xff; // least significant
        *dest++ = (code >> 8) & 0xff;
        *dest++ = (code >> 16) & 0xff;
        *dest++ = (code >> 24) & 0xff;
      } else {
        throw WideStringException("Invalid UCS-4 character code", Type::getType<WideString>());
      }
    }
    return dest - begin;
  } else {
    while (src < end) {
      assert(*src++ <= 0x0010ffff, WideStringException("Invalid UCS-4 character code", Type::getType<WideString>()));
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
}

WideString::MultibyteEncoding WideString::getMultibyteEncoding(const uint8* src, unsigned int size) throw() {
  if (size % 4 == 0) {
    if ((src[0] == 0x00) && (src[1] == 0x00) && (src[2] == 0xfe) && (src[3] == 0xff)) {
      return UTF32BE;
    } else if ((src[0] == 0xff) && (src[1] == 0xfe) && (src[2] == 0x00) && (src[3] == 0x00)) {
      return UTF32LE;
    } else { // no BOM
      const uint8* end = src + size;
      const uint8* s = src;
      while (s < end) {
        unsigned int be = (static_cast<unsigned int>(s[0]) << 24) |
          (static_cast<unsigned int>(s[1]) << 16) |
          (static_cast<unsigned int>(s[2]) << 8) |
          (static_cast<unsigned int>(s[3]) << 0);
        unsigned int le = (static_cast<unsigned int>(s[0]) << 0) |
          (static_cast<unsigned int>(s[1]) << 8) |
          (static_cast<unsigned int>(s[2]) << 16) |
          (static_cast<unsigned int>(s[3]) << 24);
        bool validBe = (be <= 0x0000ffff) || (be <= 0x0010ffff) && ((be & 0xf) < 0xe);
        bool validLe = (le <= 0x0000ffff) || (le <= 0x0010ffff) && ((le & 0xf) < 0xe);
        if (validBe && !validLe) {
          return UTF32BE;
        } else if (!validBe && validLe) {
          return UTF32LE;
        } else if (!validBe && !validLe) {
          break; // invalid UTF-32 code
        }
        s += 4;
      }
    }
  }
  if (size % 2 == 0) {
    if ((src[0] == 0xfe) && (src[1] == 0xff)) {
      return UTF16BE;
    } else if ((src[0] == 0xff) && (src[1] == 0xfe)) {
      return UTF16LE;
    } else {
      // TAG: fixme
    }
  }
  return UTF8;
}

unsigned int WideString::UTF32ToUCS4(ucs4* dest, const uint8* src, unsigned int size, unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }

  const ucs4* const begin = dest;
  const uint8* const end = src + size;
  
  assert(size % 4 == 0, MultibyteException("Invalid length of string"));

  if (size == 0) {
    assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
    return 0;
  }

  MultibyteEncoding encoding = UTF8; // invalid
  
  if (flags & ASSUME_NATIVE_BYTE_ORDER) {
    encoding = Architecture::isBigEndian() ? UTF32BE : UTF32LE;
  } else if (flags & ASSUME_BE) {
    encoding = UTF32BE;
  } else if (flags & ASSUME_LE) {
    encoding = UTF32LE;
  } else {
    if ((src[0] == 0x00) && (src[1] == 0x00) && (src[2] == 0xfe) && (src[3] == 0xff)) { // is big endian
      encoding = UTF32BE;
    } else if ((src[0] == 0xff) && (src[1] == 0xfe) && (src[2] == 0x00) && (src[3] == 0x00)) { // is little endian
      encoding = UTF32LE;
    } else { // guess byte order
      assert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
      const uint8* s = src;
      while (s < end) {
        unsigned int be = (static_cast<unsigned int>(s[0]) << 24) |
          (static_cast<unsigned int>(s[1]) << 16) |
          (static_cast<unsigned int>(s[2]) << 8) |
          (static_cast<unsigned int>(s[3]) << 0);
        unsigned int le = (static_cast<unsigned int>(s[0]) << 0) |
          (static_cast<unsigned int>(s[1]) << 8) |
          (static_cast<unsigned int>(s[2]) << 16) |
          (static_cast<unsigned int>(s[3]) << 24);

        bool validBe = (be <= 0x0000ffff) || (be <= 0x0010ffff) && ((be & 0xf) < 0xe);
        bool validLe = (le <= 0x0000ffff) || (le <= 0x0010ffff) && ((le & 0xf) < 0xe);
        
        if (validBe && !validLe) {
          encoding = UTF32BE;
          break;
        } else if (!validBe && validLe) {
          encoding = UTF32LE;
          break;
        } else if (!validBe && !validLe) {
          throw MultibyteException("Invalid UTF-32 encoding");
        }
        s += 4;
      }
      // string units are: 0x00xxyy00 where xx and yy are in the range [0x00; 0x10]
      if ((encoding == UTF8) && (dest == 0)) {
        return size/4; // only asking for length of string - no BOM is present
      }
      assert((encoding == UTF32BE) || (encoding == UTF32LE), MultibyteException("Ambiguous UTF-32 encoding"));
    }
  }
  
  switch (encoding) {
  case UTF32BE:
    if (dest != 0) {
      while (src < end) {
        unsigned int code = 0;
        code |= static_cast<unsigned int>(*src++) << 24; // most significant
        code |= static_cast<unsigned int>(*src++) << 16;
        code |= static_cast<unsigned int>(*src++) << 8;
        code |= static_cast<unsigned int>(*src++) << 0;
        assert((code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-32 encoding"));
        if (!((flags & EAT_BOM) && (code == BOM))) {
          *dest++ = code;
        }
      }
      return dest - begin;
    } else {
      unsigned int length = 0;
      while (src < end) {
        unsigned int code = 0;
        code |= static_cast<unsigned int>(*src++) << 24; // most significant
        code |= static_cast<unsigned int>(*src++) << 16;
        code |= static_cast<unsigned int>(*src++) << 8;
        code |= static_cast<unsigned int>(*src++) << 0;
        assert((code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-32 encoding"));
        if (!((flags & EAT_BOM) && (code == BOM))) {
          ++length;
        }
      }
      return length;
    }
    break;
  case UTF32LE:
    if (dest != 0) {
      while (src < end) {
        unsigned int code = 0;
        code |= static_cast<unsigned int>(*src++) << 0; // least significant
        code |= static_cast<unsigned int>(*src++) << 8;
        code |= static_cast<unsigned int>(*src++) << 16;
        code |= static_cast<unsigned int>(*src++) << 24;
        assert((code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-32 encoding"));
        if (!((flags & EAT_BOM) && (code == BOM))) {
          *dest++ = code;
        }
      }
      return dest - begin;
    } else {
      unsigned int length = 0;
      while (src < end) {
        unsigned int code = 0;
        code |= static_cast<unsigned int>(*src++) << 0; // least significant
        code |= static_cast<unsigned int>(*src++) << 8;
        code |= static_cast<unsigned int>(*src++) << 16;
        code |= static_cast<unsigned int>(*src++) << 24;
        assert((code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe), MultibyteException("Invalid UTF-32 encoding"));
        if (!((flags & EAT_BOM) && (code == BOM))) {
          ++length;
        }
      }
      return length;
    }
    break;
  default:
    throw MultibyteException("Invalid UTF-32 encoding");
  }
}



namespace isoc {

  inline size_t wcslen(const wchar_t* ws) throw() {
    const wchar_t* begin = ws;
    while (*ws) {
      ++ws;
    }
    return ws - begin;
  }
  
  inline size_t wcsnlen(const wchar_t* ws, size_t n) throw() {
    const wchar_t* begin = ws;
    while (*ws && n) {
      ++ws;
      --n;
    }
    return ws - begin;
  }

  // WARNING: not-compliant with standard which requires returning a non-const
  inline const wchar_t* wcschr(const wchar_t* ws, wchar_t wc) throw() {
    while (*ws) {
      if (*ws == wc) {
        return ws; // return pointer to the first occurrence
      }
      ++ws;
    }
    return 0; // not found
  }

  inline int wcscmp(const wchar_t* ws1, const wchar_t* ws2) throw() {
    while (*ws1 && *ws2) {
      if (*ws1 < *ws2) {
        return -1;
      } else if (*ws2 > *ws2) {
        return 1;
      }
      ++ws1;
      ++ws2;
    }
    if (*ws1) {
      return -1;
    } else if (*ws2) {
      return 1;
    } else {
      return 0;
    }
  }
  
  inline int wcsncmp(const wchar_t* ws1, const wchar_t* ws2, size_t n) throw() {
    while (*ws1 && *ws2 && n) {
      if (*ws1 < *ws2) {
        return -1;
      } else if (*ws2 > *ws2) {
        return 1;
      }
      ++ws1;
      ++ws2;
      --n;
    }
    if (n == 0) {
      return 0;
    }
    if (*ws1) {
      return -1;
    } else if (*ws2) {
      return 1;
    } else {
      return 0;
    }
  }

  // WARNING: not-compliant with standard which requires returning a non-const
  inline const wchar_t* wcsstr(const wchar_t* restrict ws1, const wchar_t* restrict ws2) throw() {
    if (*ws2) {
      return ws1;
    }
    while (true) {
      while (*ws2 && (*ws2 != *ws1)) { // skip to matching first character
        ++ws2;
      }
      if (!*ws2) {
        break;
      }
      const wchar_t* match = ws2;
      const wchar_t* substr = ws1;
      while (*substr && (*substr == *match)) {
        ++substr;
        ++match;
      }
      if (!*substr) {
        return ws2;
      }
      ++ws2;
    }
    return 0; // not found
  }
}; // end of namespace isoc

#if !defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
  #warning Assuming that mbstowcs is reentrant
#endif

String WideString::getMultibyteString(const wchar* string) throw(NullPointer, MultibyteException, WideStringException) {
  ASSERT((sizeof(wchar_t) == sizeof(ucs2)) || (sizeof(wchar_t) == sizeof(ucs4)));
  
  assert(string, NullPointer(Type::getType<WideString>()));
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, WideStringException(Type::getType<WideString>())); // maximum length exceeded
  const int numberOfCharacters = terminator - string;
  
  String result;
  if (sizeof(wchar) == sizeof(ucs2)) {
    int multibyteLength = UCS2ToUTF8(0, Cast::pointer<const ucs2*>(string), numberOfCharacters);
    assert((numberOfCharacters == 0) || (multibyteLength > 0), MultibyteException(Type::getType<WideString>()));
    result.forceToLength(multibyteLength);
    UCS2ToUTF8(Cast::pointer<uint8*>(result.getElements()), Cast::pointer<const ucs2*>(string), numberOfCharacters);
  } else {
    int multibyteLength = UCS4ToUTF8(0, Cast::pointer<const ucs4*>(string), numberOfCharacters);
    assert((numberOfCharacters == 0) || (multibyteLength > 0), MultibyteException(Type::getType<WideString>()));
    result.forceToLength(multibyteLength);
    UCS4ToUTF8(Cast::pointer<uint8*>(result.getElements()), Cast::pointer<const ucs4*>(string), numberOfCharacters);
  }
  return result;
}

// TAG: need getMultibyteString() with maximum length argument

WideString::WideString() throw() : elements(DEFAULT_STRING.elements) {
}

WideString::WideString(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<Character>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

WideString::WideString(const WideStringLiteral& str) throw(WideStringException, MemoryException) : elements(0) {
  unsigned int length = str.getLength();
  assert(length <= MAXIMUM_LENGTH, WideStringException(this)); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), str, length); // no overlap
}

WideString::WideString(const Character* string) throw(WideStringException, MemoryException) : elements(0) {
  assert(string, WideStringException(this)); // make sure string is proper (not empty)
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, WideStringException(this)); // maximum length exceeded
  int numberOfCharacters = terminator - string;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const Character* string, unsigned int maximum) throw(OutOfDomain, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  assert(string, WideStringException(this)); // make sure string is proper (not empty)
  const Character* terminator = find(string, maximum, Traits::TERMINATOR); // find terminator
  int numberOfCharacters = terminator ? (terminator - string) : maximum;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const String& string) throw(MultibyteException, MemoryException) {
  ASSERT((sizeof(wchar_t) == sizeof(ucs2)) || (sizeof(wchar_t) == sizeof(ucs4)));
  unsigned int multibyteLength = string.getLength();
  if (sizeof(wchar) == sizeof(ucs2)) {
    int numberOfCharacters = UTF8ToUCS2(0, Cast::pointer<const uint8*>(string.getElements()), multibyteLength);
    assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
    elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
    if (numberOfCharacters) {
      UTF8ToUCS2(Cast::pointer<ucs2*>(elements->getElements()), Cast::pointer<const uint8*>(string.getElements()), multibyteLength);
    }
  } else {
    int numberOfCharacters = UTF8ToUCS4(0, Cast::pointer<const uint8*>(string.getElements()), multibyteLength);
    assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
    elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
    if (numberOfCharacters) {
      UTF8ToUCS4(Cast::pointer<ucs4*>(elements->getElements()), Cast::pointer<const uint8*>(string.getElements()), multibyteLength);
    }
  }
}

WideString::WideString(const char* string) throw(MultibyteException, MemoryException) : elements(0) {
  ASSERT((sizeof(wchar_t) == sizeof(ucs2)) || (sizeof(wchar_t) == sizeof(ucs4)));

  if (!string) { // is string empty
    // TAG: fixme
    return;
  }
  
  const char* terminator = find<char>(string, MAXIMUM_LENGTH, 0); // find terminator
  assert(terminator, WideStringException(Type::getType<WideString>())); // maximum length exceeded
  const unsigned int multibyteLength = terminator - string;

  if (sizeof(wchar) == sizeof(ucs2)) {
    int numberOfCharacters = UTF8ToUCS2(0, Cast::pointer<const uint8*>(string), multibyteLength);
    assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
    elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
    if (numberOfCharacters) {
      UTF8ToUCS2(Cast::pointer<ucs2*>(elements->getElements()), Cast::pointer<const uint8*>(string), multibyteLength);
    }
  } else {
    int numberOfCharacters = UTF8ToUCS4(0, Cast::pointer<const uint8*>(string), multibyteLength);
    assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
    elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
    if (numberOfCharacters) {
      UTF8ToUCS4(Cast::pointer<ucs4*>(elements->getElements()), Cast::pointer<const uint8*>(string), multibyteLength);
    }
  }

  
//   int numberOfCharacters = 0;
//   if (string) { // is string proper (not empty)
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   numberOfCharacters = ::MultiByteToWideChar(CP_UTF8, 0, string, -1, 0, 0); // includes terminator
//   assert(numberOfCharacters > 0, MultibyteException(this));
//   --numberOfCharacters;
//   assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
//   elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
//   ::MultiByteToWideChar(CP_UTF8, 0, string, -1, elements->getElements(), numberOfCharacters + 1); // includes terminator
// #else // unix
// #if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
//     mbstate_t state;
//     clear(state); // initial state
//     const char* current = string;
//     size_t result = mbsrtowcs(0, &current, 0, &state);
// #else
//     size_t result = mbstowcs(0, string, 0);
// #endif
//     assert(result != size_t(-1), MultibyteException(this));
//     assert(result <= MAXIMUM_LENGTH, MemoryException(this)); // maximum length exceeded
//     numberOfCharacters = result;
//   }
//   elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
//   if (numberOfCharacters) {
// #if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
//     mbstate_t state;
//     clear(state); // initial state
//     const char* current = string;
//     size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
// #else
//     size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
// #endif
// #endif // flavor
//   }
}

WideString::WideString(const char* string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  int numberOfCharacters = 0;
  if (string) { // is string proper
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(0, &current, maximum, &state);
#else
    size_t result = mbstowcs(0, string, maximum);
#endif
    assert(result != size_t(-1), MultibyteException(this));
    assert(result <= MAXIMUM_LENGTH, MemoryException(this)); // maximum length exceeded
    numberOfCharacters = minimum(static_cast<unsigned int>(result), maximum);
  }
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  if (numberOfCharacters) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
#else
    size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
#endif
  }
}

WideString& WideString::operator=(const WideStringLiteral& string) throw(WideStringException, MemoryException) {
  unsigned int length = string.getLength();
  assert(length <= MAXIMUM_LENGTH, WideStringException(this)); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), string, length); // no overlap
  return *this;
}

bool WideString::isASCII() const throw() {
  const Character* i = getBuffer();
  const Character* end = i + getLength();
  while (i < end) {
    if (!ASCIITraits::isASCII(*i++)) {
      return false;
    }
  }
  return true;
}

void WideString::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  elements->ensureCapacity(capacity); // no need to do copyOnWrite
}

void WideString::optimizeCapacity() throw() {
  elements->optimizeCapacity(); // no need to do copyOnWrite
}

unsigned int WideString::getGranularity() const throw() {
  return elements->getGranularity();
}

void WideString::setGranularity(unsigned int granularity) throw() {
  elements->setGranularity(granularity);
}

WideString::Character WideString::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < getLength(), OutOfRange(this));
  return getBuffer()[index];
}

void WideString::setAt(unsigned int index, Character value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange(this));
  if (value != Traits::TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

WideString& WideString::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  int length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      Character* buffer = getBuffer(); // we are about to modify the buffer
      move(buffer + start, buffer + end, length - end); // move end of string
      elements->setSize(length - (end - start) + 1); // remember space for terminator
    }
  }
  return *this;
}

WideString& WideString::removeFrom(unsigned int start) throw(MemoryException) {
  if (start < getLength()) { // protect against some cases
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(start + 1); // remove section from end of string
  }
  return *this;
}

WideString& WideString::insert(unsigned int index, Character ch) throw(WideStringException, MemoryException) {
  int length = getLength();
  setLength(length + 1);
  Character* buffer = elements->getElements();
  if (index >= length) {
    buffer[length] = ch; // insert section at end of string
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, length - index);
    buffer[index] = ch;
  }
  return *this;
}

WideString& WideString::insert(unsigned int index, const WideString& str) throw(WideStringException, MemoryException) {
  int length = getLength();
  int strlength = str.getLength();
  setLength(length + strlength); // TAG: also protects against self insertion - but can this be circumvented
  Character* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, str.getBuffer(), strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, length - index); // move end of string
    copy(buffer + index, str.getBuffer(), strlength);
  }
  return *this;
}

WideString& WideString::insert(unsigned int index, const WideStringLiteral& str) throw(WideStringException, MemoryException) {
  int length = getLength();
  setLength(length + str.getLength());
  Character* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy<Character>(buffer + length, str, str.getLength());
  } else {
    // insert section in middle or beginning of string
    move<Character>(buffer + index + str.getLength(), buffer + index, length - index);
    copy<Character>(buffer + index, str, str.getLength());
  }
  return *this;
}

WideString& WideString::append(const WideStringLiteral& str) throw(WideStringException, MemoryException) {
  int length = getLength();
  setLength(length + str.getLength());
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, str.getLength());
  return *this;
}

WideString& WideString::append(const WideStringLiteral& str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  int length = getLength();
  setLength(length + minimum(str.getLength(), maximum));
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, minimum(str.getLength(), maximum));
  return *this;
}

WideString& WideString::append(const Character* str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  assert(str, WideStringException(this)); // make sure string is proper (not empty)
  int strlength = 0;
  const Character* terminator = find(str, maximum, Traits::TERMINATOR); // find terminator
  strlength = terminator ? (terminator - str) : maximum;
  int length = getLength();
  setLength(length + strlength);
  Character* buffer = elements->getElements();
  copy(buffer + length, str, strlength);
  return *this;
}

WideString& WideString::replace(unsigned int start, unsigned int end, const WideString& str) throw(WideStringException, MemoryException) {
  // need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

unsigned int WideString::replaceAll(const WideString& fromStr, const WideString& toStr) throw(WideStringException, MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength() - 1, toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

WideString WideString::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  int length = getLength();
  if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    // 0 <= start < end <= getLength()
    unsigned int lengthOfSubstring = end - start;
    WideString result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    return WideString(); // return empty string
  }
}

WideString& WideString::operator-=(const WideString& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

WideString& WideString::reverse() throw() {
  Character* p = getBuffer();
  Character* q = &p[getLength() - 1]; // last char - empty string => q < p
  Character temp;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

//WideString::Character* WideString::substring(Character* buffer, unsigned int start, unsigned int end) const throw() {
//  if (buffer) {
//    if ((start <= end) && (start < getLength())) {
//      if (end >= getLength()) {
//        end = getLength() - 1; // index of last char in this string
//      }
//      // 0 <= start <= end < getLength()
//      unsigned int lengthOfSubstring = end - start + 1;
//      copy(buffer, getBuffer() + start, lengthOfSubstring); // buffers do not overlap
//      buffer[lengthOfSubstring] = Traits::TERMINATOR;
//    } else {
//      *buffer = Traits::TERMINATOR;
//    }
//  }
//  return buffer;
//}

WideString& WideString::toLowerCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

WideString& WideString::toUpperCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int WideString::compareTo(const WideString& str) const throw() {
  return isoc::wcscmp(getElements(), str.getElements());
}

int WideString::compareTo(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException(this));
  return isoc::wcscmp(getElements(), str);
}

int WideString::compareToIgnoreCase(const Character* left, const Character* right) throw() {
  while (*left && *right) { // continue until end of any string has been reached
    if (*left != *right) { // not equal
      int result = Traits::toLower(*left) - Traits::toLower(*right);
      if (result != 0) { // not equal
        return result;
      }
    }
    ++left;
    ++right;
  }
  // possible cases: only end of 'left' (less than), only end of 'right' (greater than), end of both (equal)
  return (Traits::toLower(*left) - Traits::toLower(*right));
}

int WideString::compareToIgnoreCase(const WideString& str) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return compareToIgnoreCase(getElements(), str.getElements());
  //return _wcsicmp(getElements(), str.getElements());
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str.getElements());
#else // unix
  return compareToIgnoreCase(getElements(), str.getElements());
#endif
}

int WideString::compareToIgnoreCase(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException(this));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return compareToIgnoreCase(getElements(), str);
  //return _wcsicmp(getElements(), str);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str);
#else // unix
  return compareToIgnoreCase(getElements(), str);
#endif
}

bool WideString::startsWith(const WideString& prefix) const throw() {
  return !prefix.isEmpty() && (isoc::wcsncmp(getBuffer(), prefix.getBuffer(), prefix.getLength()) == 0); // NULL-terminators are not required
}

bool WideString::startsWith(const WideStringLiteral& prefix) const throw() {
  return (prefix.getLength() > 0) && (isoc::wcsncmp(getBuffer(), prefix, prefix.getLength()) == 0); // NULL-terminator is not required
}

bool WideString::endsWith(const WideString& suffix) const throw() {
  return !suffix.isEmpty() && (isoc::wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix.getBuffer(), suffix.getLength()) == 0); // NULL-terminators are not required
}

bool WideString::endsWith(const WideStringLiteral& suffix) const throw() {
  return (suffix.getLength() > 0) && (isoc::wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix, suffix.getLength()) == 0); // NULL-terminator is not required
}

int WideString::indexOf(Character ch, unsigned int start) const throw() {
  int length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const Character* buffer = getElements();
  const Character* result = isoc::wcschr(buffer + start, ch);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

int WideString::indexOf(const WideString& str, unsigned int start) const throw() {
  if (start >= getLength()) {
    return -1; // not found
  }

  const Character* buffer = getElements();
  const Character* result = isoc::wcsstr(buffer + start, str.getElements());
  if (result) {
    return result - buffer;
  } else {
    return -1; // not found
  }
}

int WideString::lastIndexOf(Character ch, unsigned int start) const throw() {
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const Character* buffer = getBuffer();
  const Character* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return -1; // not found
}

int WideString::lastIndexOf(const WideString& str, unsigned int start) const throw() {
  if ((str.isEmpty()) || (str.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  // TAG: todo

  return -1; // not found
}

unsigned int WideString::count(Character ch, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

unsigned int WideString::count(const WideString& str, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(str, start)) >= 0) { // until not found - works for empty str
    ++count;
    start = result + str.getLength();
  }
  return count;
}

String WideString::getMultibyteString() const throw(MultibyteException, MemoryException) {
  const int length = getLength();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  const Character* buffer = getBuffer();
  int multibyteLength = ::WideCharToMultiByte(CP_UTF8, // code page
                                              0, // performance and mapping flags
                                              buffer, // wide-character string
                                              length, // number of chars in string
                                              0, // buffer for new string
                                              0, // size of buffer
                                              0, // default for unmappable chars
                                              0 // set when default char used
  );
  assert((length == 0) || (multibyteLength > 0), MultibyteException(this));
  String result;
  result.forceToLength(multibyteLength); // raises exception if maximum length exceeded
  ::WideCharToMultiByte(CP_UTF8, // code page
                        0, // performance and mapping flags
                        buffer, // wide-character string
                        length, // number of chars in string
                        result.getElements(), // buffer for new string
                        multibyteLength, // size of buffer
                        0, // default for unmappable chars
                        0 // set when default char used
  );
  return result;
#else // unix
  String result(MB_LEN_MAX * length); // greedy implementation
  size_t multibyteLength = ::wcstombs(result.getElements(), getElements(), MB_LEN_MAX * length + 1);
  assert(multibyteLength >= 0, MultibyteException(this));
  result.forceToLength(multibyteLength);
  return result;
#endif // flavor
}

template<>
int compare<WideString>(const WideString& left, const WideString& right) throw() {
  return isoc::wcscmp(left.getElements(), right.getElements());
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException) {
  // TAG: need support for \uXXXXX
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  const int length = value.getLength();
  Allocator<char> buffer(MB_LEN_MAX * length); // no terminator // TAG: what is the max. char length
  int result = ::WideCharToMultiByte(CP_UTF8, 0, value.getElements(), length, buffer.getElements(), buffer.getSize(), 0, 0);
  ASSERT((length == 0) || (result > 0)); // no errors expected
  stream.addCharacterField(buffer.getElements(), result);
#else // unix
  Allocator<char> buffer(MB_LEN_MAX * value.getLength() + 1); // remember terminator - greedy implementation
  size_t result = ::wcstombs(buffer.getElements(), value.getElements(), buffer.getSize());
  assert(result != size_t(-1), MultibyteException());
  stream.addCharacterField(buffer.getElements(), result);
#endif // flavor
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
