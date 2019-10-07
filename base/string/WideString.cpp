/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Primitives.h>
#include <base/string/WideString.h>
#include <base/Functor.h>
#include <base/Architecture.h>
#include <base/string/unicode/UnicodeLookup.cpp>
#include <base/string/unicode/UnicodeFolding.cpp>
#include <base/UnexpectedFailure.h>
#include <locale>
#include <codecvt>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: add reference to RFC 2781
// TAG: RFC 2044
// TAG: need SCSU support see http://www.unicode.org/unicode/reports/tr6/
// TAG: need support for native unicode encoding (e.g. SYSTEM_UNICODE_ENCODING)?
// TAG: need input support for something like: "asdfd\u015fsdjfhd", "&#xXXXXX", and "&#DDDDD"
// TAG: need support for width calculation. NBWSP=>0???

// TAG: GCC 3.2 bug: fails to compile
// const WideTraits::CharacterDescriptor WideTraits::characters[0]; // when 0 elements
// getArraySize(characters);

bool WideTraits::isControl(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(controlCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(controlCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < controlCharacters[index].first) {
        last = index - 1;
      } else if (character > controlCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isUpper(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(upperCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(upperCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < upperCharacters[index].first) {
        last = index - 1;
      } else if (character > upperCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isLower(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(lowerCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(lowerCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < lowerCharacters[index].first) {
        last = index - 1;
      } else if (character > lowerCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isTitle(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(titleCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(titleCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < titleCharacters[index].first) {
        last = index - 1;
      } else if (character > titleCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

unsigned int WideTraits::getFlags(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        return characters[index].flags; // found
      }
    }
  }
#endif
  return 0;
}

ucs4 WideTraits::toLower(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].lower != 0) {
          return characters[index].lower; // found and valid
        }
        break;
      }
    }
  }
#endif
  return character;
}

ucs4 WideTraits::toUpper(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].upper != 0) {
          return characters[index].upper; // found and valid
        }
        break;
      }
    }
  }
#endif
    return character;
}

ucs4 WideTraits::toTitle(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].title != 0) {
          return characters[index].title; // found and valid
        }
        break;
      }
    }
  }
#endif
  return character;
}

bool WideTraits::isSpace(ucs4 character) throw() {
  if (character == ' ') {
    return true;
  }
#if 0 // TAG: FIXME
  if (getArraySize(spaceCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(spaceCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < spaceCharacters[index].first) {
        last = index - 1;
      } else if (character > spaceCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isDigit(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(digitCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(digitCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < digitCharacters[index].first) {
        last = index - 1;
      } else if (character > digitCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

int WideTraits::digitToValue(ucs4 character) throw() {
#if 0 // TAG: FIXME
  if (getArraySize(digitCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(digitCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < digitCharacters[index].first) {
        last = index - 1;
      } else if (character > digitCharacters[index].last) {
        begin = index + 1;
      } else {
        return (character - digitCharacters[index].first) + digitCharacters[index].value; // found
      }
    }
  }
#endif
  return -1;
}

#if 0
bool isUCS4(unsigned int value) throw() {
  if (value < 0xd800) {
    return true;
  } else if (value < 0x10000) {
    return (value >= 0xe000) && (value < 0xffff); // TAG: 0xfffe is special
  } else {
    return true;
  }
}
#endif

void WideString::initialize(
  const wchar* string, unsigned int nativeLength) throw(MemoryException) {
  
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(string),
      nativeLength
    );
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      length + 1,
      GRANULARITY
    );
    UCS2ToUCS4(
      elements->getElements(),
      Cast::pointer<const ucs2*>(string),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      nativeLength + 1,
      GRANULARITY
    );
    copy<ucs4>(
      elements->getElements(),
      Cast::pointer<const ucs4*>(string),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

void WideString::initialize(
  const char* string, unsigned int length) throw(MemoryException) {
  
  const unsigned int numberOfCharacters =
    UTF8ToUCS4(0, Cast::pointer<const uint8*>(string), length);
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedCapacityAllocator<ucs4>(
    numberOfCharacters + 1,
    GRANULARITY
  );
  if (numberOfCharacters) {
    UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string),
      length
    );
  }
}

Literal WideString::toString(MultibyteEncoding encoding) throw() {
  switch (encoding) {
  case UTF7:
    return Literal("UTF-7");
  case UTF8:
    return Literal("UTF-8");
  case UTF16:
    return Literal("UTF-16");
  case UTF16BE:
    return Literal("UTF-32BE");
  case UTF16LE:
    return Literal("UTF-32LE");
  case UTF32:
    return Literal("UTF-32");
  case UTF32BE:
    return Literal("UTF-32BE");
  case UTF32LE:
  default: // avoid warning
    return Literal("UTF-32LE");
  }
}

unsigned int WideString::UCS2ToUTF8(
  uint8* dest,
  const ucs2* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
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
        bassert(
          !((code >= 0xdc00) && (code <= 0xdfff)),
          WideStringException("Reserved code")
        );
        if ((code >= 0xd800) && (code <= 0xdbff)) {
          bassert(src < end, WideStringException("Incomplete sequence"));
          const ucs2 lowSurrogate = *src++;
          bassert(
            (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
            WideStringException("Invalid sequence")
          );
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
        bassert(
          !((code >= 0xdc00) && (code <= 0xdfff)),
          WideStringException("Reserved code")
        );
        if ((code >= 0xd800) && (code <= 0xdbff)) {
          bassert(src < end, WideStringException("Incomplete sequence"));
          const ucs2 lowSurrogate = *src++;
          bassert(
            (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
            WideStringException("Invalid sequence")
          );
          length += 4;
        } else {
          length += 3;
        }
      }
    }
    return length;
  }
}

unsigned int WideString::UCS4ToUTF8(
  uint8* dest,
  const ucs4* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
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
        throw WideStringException(
          "Invalid UCS-4 character",
          Type::getType<WideString>()
        );
      }
    }
    return length;
  }
}

unsigned int WideString::UTF8ToUCS2(
  ucs2* dest,
  const uint8* src,
  unsigned int size,
  unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }
  if ((size >= 3) &&
      (src[0] == 0xef) && (src[0] == 0xbb) && (src[0] == 0xbf)) {
    if (flags & EAT_BOM) {
      src += 3;
    }
  } else {
    bassert(
      (flags & EXPECT_BOM) == 0,
      MultibyteException("BOM is excepted")
    );
  }
  const ucs2* const begin = dest;
  const uint8* const end = src + size;
  if (dest) {
    while (src < end) {
      uint8 value = *src++;
      if ((value & 0x80) == 0x00) {
        *dest++ = value;
      } else if ((value & 0xe0) == 0xc0) {
        ucs2 code = value & ~0xe0;
        bassert(src < end, MultibyteException("Incomplete code"));
        uint8 temp = *src++;
        bassert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
        code = (code << 6) | temp;
        *dest++ = code;
      } else if ((value & 0xf0) == 0xe0) {
        ucs2 code = value & ~0xf0;
        for (int i = 0; i < 2; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          bassert(
            (temp & 0xc0) == 0x80,
            MultibyteException("Invalid code")
          );
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xf8) == 0xf0) {
        ucs4 code = value & ~0xf8;
        for (int i = 0; i < 3; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          bassert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
          code = (code << 6) | temp;
        }
        *dest++ = (code - 0x00010000U)/0x400 + 0xd800; // high surrogate
        *dest++ = (code - 0x00010000U)%0x400 + 0xdc00; // low surrogate
      } else if ((value & 0xfc) == 0xf8) {
        for (int i = 0; i < 4; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          bassert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else if ((value & 0xfe) == 0xfc) {
        for (int i = 0; i < 5; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          uint8 temp = *src++;
          bassert((temp & 0xc0) == 0x80, MultibyteException("Invalid code"));
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
      if ((value & 0x80) == 0x00) {
        ++length;
      } else if ((value & 0xe0) == 0xc0) {
        bassert(src < end, MultibyteException("Incomplete code"));
        bassert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        ++length;
      } else if ((value & 0xf0) == 0xe0) {
        for (int i = 0; i < 2; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          bassert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        ++length;
      } else if ((value & 0xf8) == 0xf0) {
        for (int i = 0; i < 3; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          bassert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        length += 2;
      } else if ((value & 0xfc) == 0xf8) {
        for (int i = 0; i < 4; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          bassert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else if ((value & 0xfe) == 0xfc) {
        for (int i = 0; i < 5; ++i) {
          bassert(src < end, MultibyteException("Incomplete code"));
          bassert((*src++ & 0xc0) == 0x80, MultibyteException("Invalid code"));
        }
        throw WideStringException("No mapping");
      } else {
        throw MultibyteException("Invalid code");
      }
    }
    return length;
  }
}

#if 0 // TAG: fixme
inline /*???*/
void WideString::invalidCode(
  int octetIndex, int suboctetIndex) throw(MultibyteException) {
  MultibyteException e(
    "Invalid character code",
    Type::getType<WideString>()
  );
  e.setCause(MultibyteException::INVALID_CHARACTER_CODE);
  e.setOctetIndex(octetIndex);
  e.setSuboctetIndex(suboctetIndex);
  throw e;
}

inline /*???*/
void WideString::incompleteCode(
  int octetIndex, int suboctetIndex) throw(MultibyteException) {
  MultibyteException e(
    "Incomplete character code",
    Type::getType<WideString>()
  );
  e.setCause(MultibyteException::INCOMPLETE_CHARACTER_CODE);
  e.setOctetIndex(octetIndex);
  e.setSuboctetIndex(suboctetIndex);
  throw e;
}
#endif

unsigned int WideString::UTF8ToUCS4(
  ucs4* dest,
  const uint8* src,
  unsigned int size,
  unsigned int flags) throw(MultibyteException) {
  if (src == 0) {
    return 0;
  }
  if ((size >= 3) && (src[0] == 0xef) && (src[0] == 0xbb) && (src[0] == 0xbf)) {
    if (flags & EAT_BOM) {
      src += 3;
    }
  } else {
    bassert(
      (flags & EXPECT_BOM) == 0,
      MultibyteException("BOM is excepted", Type::getType<WideString>())
    );
  }
  
  const uint8* const srcBegin = src;
  const ucs4* const begin = dest;
  const uint8* const end = src + size;
  
  if (dest) {
    while (src < end) {
      uint8 value = *src++;
      if ((value & 0x80) == 0x00) {
        *dest++ = value;
      } else if ((value & 0xe0) == 0xc0) {
        ucs4 code = value & ~0xe0;
        bassert(
          src < end,
          MultibyteException(
            "Incomplete character code",
            Type::getType<WideString>()
          )
        );
        uint8 temp = *src++;
        bassert(
          (temp & 0xc0) == 0x80,
          MultibyteException(
            "Invalid character code",
            Type::getType<WideString>()
          )
        );
        code = (code << 6) | temp;
        *dest++ = code;
      } else if ((value & 0xf0) == 0xe0) {
        ucs4 code = value & ~0xf0;
        for (int i = 0; i < 2; ++i) {
          bassert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          bassert(
            (temp & 0xc0) == 0x80,
            MultibyteException("Invalid character code")
          );
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xf8) == 0xf0) {
        ucs4 code = value & ~0xf8;
        for (int i = 0; i < 3; ++i) {
          bassert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          bassert(
            (temp & 0xc0) == 0x80,
            MultibyteException("Invalid character code")
          );
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xfc) == 0xf8) {
        ucs4 code = value & ~0xfc;
        for (int i = 0; i < 4; ++i) {
          bassert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          bassert(
            (temp & 0xc0) == 0x80,
            MultibyteException("Invalid character code")
          );
          code = (code << 6) | temp;
        }
        *dest++ = code;
      } else if ((value & 0xfe) == 0xfc) {
        ucs4 code = value & ~0xfe;
        for (int i = 0; i < 5; ++i) {
          bassert(src < end, MultibyteException("Incomplete character code"));
          uint8 temp = *src++;
          bassert(
            (temp & 0xc0) == 0x80,
            MultibyteException("Invalid character code")
          );
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
        MultibyteException e(
          "Invalid character code",
          Type::getType<WideString>()
        );
        e.setCause(MultibyteException::INVALID_CHARACTER_CODE);
        e.setOctetIndex(src - srcBegin);
        e.setSuboctetIndex(0);
        throw e;
      }
      bassert(
        src + numberOfOctets <= end,
        bindCause(
          MultibyteException(
            "Incomplete character code",
            Type::getType<WideString>()
          ),
          MultibyteException::INCOMPLETE_CHARACTER_CODE
        )
      );
      for (unsigned int i = 0; i < numberOfOctets; ++i) {
        bassert(
          (*src++ & 0xc0) == 0x80,
          MultibyteException(
            "Invalid character code",
            Type::getType<WideString>()
          )
        );
      }
      ++length;
    }
    return length;
  }
}

unsigned int WideString::UCS2ToUTF16BE(
  uint8* dest,
  const ucs2* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  // TAG: fixme
  return 0;
}

unsigned int WideString::UCS2ToUTF16LE(
  uint8* dest,
  const ucs2* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  // TAG: fixme
  return 0;
}

unsigned int WideString::UCS4ToUTF16BE(uint8* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = static_cast<uint8>((BOM >> 8) & 0xff); // most significant
      *dest++ = static_cast<uint8>((BOM >> 0) & 0xff);
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        bassert(
          !((value >= 0x0000d800) && (value <= 0x0000dfff)),
          WideStringException("Value reserved for UTF-16")
        );
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
        bassert(!((value >= 0x0000d800) && (value <= 0x0000dfff)), WideStringException("Value reserved for UTF-16"));
        length += 2;
      } else if (value <= 0x0010ffff) {
        bassert((value & 0xf) < 0xe, WideStringException("Invalid UCS-4 character", Type::getType<WideString>()));
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
      *dest++ = static_cast<uint8>((BOM >> 8) & 0xff); // most significant
      *dest++ = static_cast<uint8>((BOM >> 0) & 0xff);
    }
    while (src < end) {
      unsigned int value = *src++;
      if (value <= 0x0000ffff) {
        bassert(
          !((value >= 0x0000d800) && (value <= 0x0000dfff)),
          WideStringException("Value reserved for UTF-16")
        );
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
        bassert(
          !((value >= 0x0000d800) && (value <= 0x0000dfff)),
          WideStringException("Value reserved for UTF-16")
        );
        length += 2;
      } else if (value <= 0x0010ffff) {
        bassert(
          (value & 0xf) < 0xe,
          WideStringException(
            "Invalid UCS-4 character",
            Type::getType<WideString>()
          )
        );
        length += 4;
      } else {
        throw WideStringException("Value incompatible with UTF-16", Type::getType<WideString>());
      }
    }
    return length;
  }
}

unsigned int WideString::UCS2ToUCS4(ucs4* dest, const ucs2* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const ucs4* const begin = dest;
  const ucs2* end = src + size;
  if (dest) {
    while (src < end) {
      const ucs2 code = *src++;
      bassert(
        !((code >= 0xdc00) && (code <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((code >= 0xd800) && (code <= 0xdbff)) {
        bassert(src < end, WideStringException("Incomplete sequence"));
        const ucs2 lowSurrogate = *src++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
        *dest++ = 0x00010000U +
          (static_cast<ucs4>(code) - 0xd800) * 0x0400 +
          (static_cast<ucs4>(lowSurrogate) - 0xdc00);
      } else {
        *dest++ = code;
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src < end) {
      const ucs2 code = *src++;
      bassert(
        !((code >= 0xdc00) && (code <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((code >= 0xd800) && (code <= 0xdbff)) {
        bassert(src < end, WideStringException("Incomplete sequence"));
        const ucs2 lowSurrogate = *src++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
      }
      ++length;
    }
    return length;
  }
}

unsigned int WideString::UCS4ToUCS2(ucs2* dest, const ucs4* src, unsigned int size, unsigned int flags) throw(WideStringException) {
  if (src == 0) {
    return 0;
  }
  const ucs2* const begin = dest;
  const ucs4* end = src + size;
  if (dest) {
    while (src < end) {
      const ucs4 value = *src++;
      if (value <= 0x0000ffff) {
        bassert(
          !((value >= 0x0000d800) && (value <= 0x0000dfff)),
          WideStringException("Value reserved for UCS-2 encoding")
        );
        *dest++ = value;
      } else if (value <= 0x0010ffff) {
        *dest++ = (value - 0x00010000)/0x400 + 0xd800; // 10 bit - most significant
        *dest++ = (value - 0x00010000)%0x400 + 0xdc00; // 10 bit
      } else {
        throw WideStringException("Value incompatible with UCS-2", Type::getType<WideString>());
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src < end) {
      const ucs4 value = *src++;
      if (value <= 0x0000ffff) {
        bassert(
          !((value >= 0x0000d800) && (value <= 0x0000dfff)),
          WideStringException("Value reserved for UCS-2 encoding")
        );
        ++length;
      } else if (value <= 0x0010ffff) {
        bassert(
          (value & 0xf) < 0xe,
          WideStringException(
            "Invalid UCS-4 character",
            Type::getType<WideString>()
          )
        );
        length += 2;
      } else {
        throw WideStringException("Value incompatible with UCS-2", Type::getType<WideString>());
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
  
  bassert(size % 2 == 0, MultibyteException("Invalid length of string"));
  
  if (size == 0) {
    bassert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
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
      bassert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
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
      bassert((encoding == UTF16BE) || (encoding == UTF16LE), MultibyteException("Ambiguous UTF-16 encoding"));
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
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
          bassert(
            (temp >= 0xdc00) && (temp <= 0xdfff),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
          code = 0x00010000U + (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00);
          bassert(
            (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
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
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 8) | (static_cast<unsigned int>(*src++) << 0);
          bassert(
            (temp >= 0xdc00) && (temp <= 0xdfff),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
          code = (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
          bassert(
            (code <= 0x0000ffff) && ((code & 0xf) < 0xe),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
        } else if (code <= 0xdfff) {
          throw MultibyteException(
            "Invalid UTF-16 encoding",
            Type::getType<WideString>()
          );
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
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
          bassert(
            (temp >= 0xdc00) && (temp <= 0xdfff),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
          code = 0x00010000U + (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00);
          bassert(
            (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
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
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          uint16 temp = (static_cast<unsigned int>(*src++) << 0) | (static_cast<unsigned int>(*src++) << 8);
          bassert(
            (temp >= 0xdc00) && (temp <= 0xdfff),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
          code = (code - 0xd800) * 0x0400 + (static_cast<unsigned int>(temp) - 0xdc00); // not final ucs4 value
          bassert(
            (code <= 0x0000ffff) && ((code & 0xf) < 0xe),
            MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>())
          );
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

unsigned int WideString::UCS4ToUTF32BE(
  uint8* dest,
  const ucs4* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
  
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
      bassert(*src++ <= 0x0010ffff, WideStringException("Invalid UCS-4 character code", Type::getType<WideString>()));
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
  return dest - begin;
}

unsigned int WideString::UCS4ToUTF32LE(
  uint8* dest,
  const ucs4* src,
  unsigned int size,
  unsigned int flags) throw(WideStringException) {
  
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
      bassert(*src++ <= 0x0010ffff, WideStringException("Invalid UCS-4 character code", Type::getType<WideString>()));
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
  
  bassert(size % 4 == 0, MultibyteException("Invalid length of string"));

  if (size == 0) {
    bassert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
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
      bassert((flags & EXPECT_BOM) == 0, MultibyteException("BOM is excepted"));
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
      bassert(
        (encoding == UTF32BE) || (encoding == UTF32LE),
        MultibyteException("Ambiguous UTF-32 encoding")
      );
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
        bassert(
          (code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
          MultibyteException("Invalid UTF-32 encoding")
        );
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
        bassert(
          (code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
          MultibyteException("Invalid UTF-32 encoding")
        );
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
        bassert(
          (code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
          MultibyteException("Invalid UTF-32 encoding")
        );
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
        bassert(
          (code <= 0x0000ffff) || (code <= 0x0010ffff) && ((code & 0xf) < 0xe),
          MultibyteException("Invalid UTF-32 encoding")
        );
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

String WideString::getMultibyteString(const wchar* string)
  throw(NullPointer, MultibyteException, WideStringException) {
  // TAG: should accept string == 0
  bassert(string, NullPointer(Type::getType<WideString>()));
  const wchar* terminator = find<wchar>(string, MAXIMUM_LENGTH, 0); // find terminator
  bassert(terminator, WideStringException(Type::getType<WideString>())); // maximum length exceeded
  const int nativeLength = terminator - string;
  
  String result;
  if (sizeof(wchar) == sizeof(ucs2)) {
    int multibyteLength = UCS2ToUTF8(0, Cast::pointer<const ucs2*>(string), nativeLength);
    bassert(
      (nativeLength == 0) || (multibyteLength > 0),
      MultibyteException(Type::getType<WideString>())
    );
    result.forceToLength(multibyteLength);
    UCS2ToUTF8(
      Cast::pointer<uint8*>(result.getElements()),
      Cast::pointer<const ucs2*>(string),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    int multibyteLength = UCS4ToUTF8(0, Cast::pointer<const ucs4*>(string), nativeLength);
    bassert(
      (nativeLength == 0) || (multibyteLength > 0),
      MultibyteException(Type::getType<WideString>())
    );
    result.forceToLength(multibyteLength);
    UCS4ToUTF8(
      Cast::pointer<uint8*>(result.getElements()),
      Cast::pointer<const ucs4*>(string),
      nativeLength
    );
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      Type::getType<WideString>()
    );
  }
  return result;
}

// TAG: need getMultibyteString() with maximum length argument

WideString::WideString() throw() : elements(DEFAULT_STRING.elements) {
}

WideString::WideString(unsigned int capacity) throw(MemoryException) {
  elements = new ReferenceCountedCapacityAllocator<ucs4>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

WideString::WideString(const wchar* string) throw(StringException, MemoryException) {
  if (string) {
    const size_t size = wcslen(string);
    initialize(string, size);
  }
}

WideString::WideString(const std::string& string) throw(WideStringException, MemoryException) {
  const std::wstring wide = toWide(string);
  initialize(wide.c_str(), wide.size());
}

WideString::WideString(const std::wstring& string) throw(WideStringException, MemoryException) {
  initialize(string.c_str(), string.size());
}

WideString::WideString(const WideLiteral& literal) throw(WideStringException, MemoryException) {
  MemorySize nativeLength = literal.getLength();
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
    bassert(
      length <= MAXIMUM_LENGTH,
      WideStringException(this)
    ); // TAG: this is not required
    elements = new ReferenceCountedCapacityAllocator<ucs4>(length + 1, GRANULARITY);
    UCS2ToUCS4(
      elements->getElements(),
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    bassert(
      nativeLength <= MAXIMUM_LENGTH,
      WideStringException(this)
    ); // TAG: this is not required
    elements = new ReferenceCountedCapacityAllocator<ucs4>(nativeLength + 1, GRANULARITY);
    copy<ucs4>(
      elements->getElements(),
      Cast::pointer<const ucs4*>(literal.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

WideString::WideString(const NativeWideString& string) throw(WideStringException, MemoryException) {
  
  if (!string.getValue()) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const wchar* terminator = find<wchar>(string.getValue(), MAXIMUM_LENGTH, 0);
  bassert(terminator, WideStringException(this)); // maximum length exceeded
  unsigned int nativeLength = terminator - string.getValue();
  
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(string.getValue()),
      nativeLength
    );
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      length + 1,
      GRANULARITY
    );
    UCS2ToUCS4(
      elements->getElements(),
      Cast::pointer<const ucs2*>(string.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      nativeLength + 1,
      GRANULARITY
    );
    copy<ucs4>(
      elements->getElements(),
      Cast::pointer<const ucs4*>(string.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

WideString::WideString(const NativeWideString& string, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  bassert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this));

  if (!string.getValue()) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const wchar* terminator = find<wchar>(string.getValue(), maximum, 0);
  const unsigned int nativeLength =
    terminator ? (terminator - string.getValue()) : maximum;
  
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(string.getValue()),
      nativeLength
    );
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      length + 1,
      GRANULARITY
    );
    UCS2ToUCS4(
      elements->getElements(),
      Cast::pointer<const ucs2*>(string.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      nativeLength + 1,
      GRANULARITY
    );
    copy<ucs4>(
      elements->getElements(),
      Cast::pointer<const ucs4*>(string.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

WideString::WideString(
  const String& string) throw(MultibyteException, MemoryException) {
  unsigned int multibyteLength = string.getLength();
  unsigned int numberOfCharacters =
    UTF8ToUCS4(
      0,
      Cast::pointer<const uint8*>(string.getElements()),
      multibyteLength
    );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedCapacityAllocator<ucs4>(
    numberOfCharacters + 1,
    GRANULARITY
  );
  if (numberOfCharacters) {
    UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string.getElements()),
      multibyteLength
    );
  }
}

WideString::WideString(const NativeString& string) throw(MultibyteException, MemoryException) {
  if (!string.getValue()) { // is string null
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const char* terminator = find<char>(string.getValue(), MAXIMUM_LENGTH, 0);
  bassert(
    terminator,
    WideStringException(Type::getType<WideString>())
  ); // maximum length exceeded
  const unsigned int multibyteLength = terminator - string.getValue();

  unsigned int numberOfCharacters = UTF8ToUCS4(
      0,
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
  );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedCapacityAllocator<ucs4>(
    numberOfCharacters + 1,
    GRANULARITY
  );
  if (numberOfCharacters) {
    UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
    );
  }
}

WideString::WideString(
  const NativeString& string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException) {
  bassert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this));
  
  if (!string.getValue()) { // is string null
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const char* terminator = find<char>(string.getValue(), maximum, 0);
  const unsigned int multibyteLength =
    (terminator) ? (terminator - string.getValue()) : maximum;
  
  unsigned int numberOfCharacters = UTF8ToUCS4(
    0,
    Cast::pointer<const uint8*>(string.getValue()),
    multibyteLength
  );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedCapacityAllocator<ucs4>(
    numberOfCharacters + 1,
    GRANULARITY
  );
  if (numberOfCharacters) {
    UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
    );
  }
}

WideString& WideString::operator=(const WideLiteral& literal)
  throw(WideStringException, MemoryException) {
  MemorySize nativeLength = literal.getLength();
  bassert(
    nativeLength <= MAXIMUM_LENGTH,
    WideStringException(this)
  ); // TAG: this is not required
  
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      length + 1,
      GRANULARITY
    );
    UCS2ToUCS4(
      elements->getElements(),
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    elements = new ReferenceCountedCapacityAllocator<ucs4>(
      nativeLength + 1,
      GRANULARITY
    );
    copy<ucs4>(
      elements->getElements(),
      Cast::pointer<const ucs4*>(literal.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
  return *this;
}

bool WideString::isASCII() const throw() {
  const ucs4* i = getBuffer();
  const ucs4* end = i + getLength();
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

ucs4 WideString::getAt(unsigned int index) const throw(OutOfRange) {
  bassert(index < getLength(), OutOfRange(this));
  return getBuffer()[index];
}

void WideString::setAt(unsigned int index, ucs4 value) throw(OutOfRange) {
  bassert(index < getLength(), OutOfRange(this));
  if (value != Traits::TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

WideString& WideString::remove(
  unsigned int start, unsigned int end) throw(MemoryException) {
  unsigned int length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      ucs4* buffer = getBuffer(); // we are about to modify the buffer
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

WideString& WideString::insert(unsigned int index, ucs4 ch) throw(WideStringException, MemoryException) {
  unsigned int length = getLength();
  setLength(length + 1);
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    buffer[length] = ch; // insert section at end of string
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, length - index);
    buffer[index] = ch;
  }
  return *this;
}

WideString& WideString::insert(
  unsigned int index,
  const WideString& str) throw(WideStringException, MemoryException) {
  
  unsigned int length = getLength();
  unsigned int strlength = str.getLength();
  setLength(length + strlength); // TAG: also protects against self insertion - but can this be circumvented
  ucs4* buffer = elements->getElements();
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

WideString& WideString::insert(
  unsigned int index,
  const WideLiteral& literal) throw(WideStringException, MemoryException) {
  unsigned int length = getLength();
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    if (sizeof(wchar) == sizeof(ucs2)) {
      MemorySize literalLength = UCS2ToUCS4(
        0,
        Cast::pointer<const ucs2*>(literal.getValue()),
        literal.getLength()
      );
      setLength(length + literalLength);
      UCS2ToUCS4(
        buffer + length,
        Cast::pointer<const ucs2*>(literal.getValue()),
        literal.getLength()
      );
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      setLength(length + literal.getLength());
      copy<ucs4>(
        buffer + length,
        Cast::pointer<const ucs4*>(literal.getValue()),
        literal.getLength()
      ); // no overlap
    } else {
      throw UnexpectedFailure(
        "Unsupported native wide character representation",
        this
      );
    }
  } else {
    // insert section in middle or beginning of string
    if (sizeof(wchar) == sizeof(ucs2)) {
      MemorySize literalLength = UCS2ToUCS4(
        0,
        Cast::pointer<const ucs2*>(literal.getValue()),
        literal.getLength()
      );
      setLength(length + literalLength);
      move<ucs4>(buffer + index + literalLength, buffer + index, length - index);
      UCS2ToUCS4(
        buffer + length,
        Cast::pointer<const ucs2*>(literal.getValue()),
        literal.getLength()
      );
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      setLength(length + literal.getLength());
      move<ucs4>(
        buffer + index + literal.getLength(),
        buffer + index,
        length - index
      );
      copy<ucs4>(
        buffer + index,
        Cast::pointer<const ucs4*>(literal.getValue()),
        literal.getLength()
      ); // no overlap
    } else {
      throw UnexpectedFailure(
        "Unsupported native wide character representation",
        this
      );
    }
  }
  return *this;
}

WideString& WideString::append(
  const WideLiteral& literal) throw(WideStringException, MemoryException) {
  unsigned int length = getLength();
  MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
    setLength(length + nativeLength);
    UCS2ToUCS4(
      buffer + length,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    setLength(length + nativeLength);
    ucs4* buffer = elements->getElements();
    copy<ucs4>(
      buffer + length,
      Cast::pointer<const ucs4*>(literal.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
  return *this;
}

WideString& WideString::append(
  const WideLiteral& literal,
  unsigned int maximum)
  throw(OutOfDomain, WideStringException, MemoryException) {
  
  unsigned int length = getLength();
  MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize length = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
    nativeLength = minimum<MemorySize>(nativeLength, maximum);
    setLength(length + nativeLength);
    UCS2ToUCS4(
      buffer + length,
      Cast::pointer<const ucs2*>(literal.getValue()),
      nativeLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    nativeLength = minimum<MemorySize>(nativeLength, maximum);
    setLength(length + nativeLength);
    ucs4* buffer = elements->getElements();
    copy<ucs4>(
      buffer + length,
      Cast::pointer<const ucs4*>(literal.getValue()),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
  return *this;
}

WideString& WideString::append(
  const wchar* string,
  unsigned int maximum)
  throw(OutOfDomain, WideStringException, MemoryException) {
  
  bassert(
    maximum <= MAXIMUM_LENGTH,
    OutOfDomain(this)
  ); // maximum length exceeded
  bassert(
    string,
    WideStringException(this)
  ); // make sure string is proper (not empty)
  const wchar* terminator = find<wchar>(string, maximum, 0); // find terminator
  unsigned int nativeLength = terminator ? (terminator - string) : maximum;
  unsigned int length = getLength();
  ucs4* buffer = elements->getElements();
  if (sizeof(wchar) == sizeof(ucs2)) {
    MemorySize stringLength = UCS2ToUCS4(
      0,
      Cast::pointer<const ucs2*>(string),
      nativeLength
    );
    stringLength = minimum<MemorySize>(stringLength, maximum);
    setLength(length + stringLength);
    UCS2ToUCS4(
      buffer + length,
      Cast::pointer<const ucs2*>(string),
      stringLength
    );
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    setLength(length + nativeLength);
    copy<ucs4>(
      buffer + length,
      Cast::pointer<const ucs4*>(string),
      nativeLength
    ); // no overlap
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
  return *this;
}

WideString& WideString::replace(
  unsigned int start,
  unsigned int end,
  const WideString& str) throw(WideStringException, MemoryException) {
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

WideString WideString::substring(
  unsigned int start, unsigned int end) const throw(MemoryException) {
  unsigned int length = getLength();
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

WideString& WideString::operator-=(
  const WideString& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

WideString& WideString::reverse() throw() {
  ucs4* p = getBuffer();
  ucs4* q = &p[getLength() - 1]; // last char - empty string => q < p
  ucs4 temp;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

WideString& WideString::toLowerCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

WideString& WideString::toUpperCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int WideString::compareTo(const WideString& string) const throw() {
  // both strings may contain multiple zeros
  unsigned int leftLength = getLength();
  unsigned int rightLength = string.getLength();
  const ucs4* left = getBuffer();
  const ucs4* right = string.getBuffer();
  const ucs4* end = left + minimum(leftLength, rightLength);
  while (left < end) {
    int temp = static_cast<int>(*left) - static_cast<int>(*right);
    if (temp != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  if (leftLength < rightLength) {
    return -1;
  } else if (leftLength > rightLength) {
    return 1;
  } else {
    return 0; // equal
  }
}

int WideString::compareTo(const WideLiteral& literal) const throw() {  
  if (sizeof(wchar) == sizeof(ucs2)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs2* right =
      Cast::pointer<const ucs2*>(literal.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
        value = 0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs4* right = Cast::pointer<const ucs4*>(literal.getValue());
    while (*left && *right) {
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(*right);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

int WideString::compareTo(const wchar* string) const throw() {
  if (!string) {
    return false;
  }
  if (sizeof(wchar) == sizeof(ucs2)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs2* right =
      Cast::pointer<const ucs2*>(string);
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
        value = 0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs4* right = Cast::pointer<const ucs4*>(string);
    while (*left && *right) {
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(*right);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else {
    throw UnexpectedFailure("Unsupported native wide character representation", this);
  }
}

int WideString::compareToIgnoreCase(const ucs4* left, const ucs4* right) throw() {
  // remember caseFold(*left)
  // remember caseFold(*right)
  // while in small buffer to binary compare
  
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

int WideString::compareToIgnoreCase(const WideString& string) const throw() {
  return compareToIgnoreCase(getElements(), string.getElements());
}

// TAG: need support for WideLiteral
// int WideString::compareToIgnoreCase(const WideLiteral& literal) const throw();

bool WideString::startsWith(const WideString& prefix) const throw() {
  unsigned int prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool WideString::startsWith(const WideLiteral& prefix) const throw() {
  if (sizeof(wchar) == sizeof(ucs2)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs2* right = Cast::pointer<const ucs2*>(prefix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
        value =
          0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return false;
      }
      ++left;
      ++right;
    }
    return *right == 0;
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    unsigned int prefixLength = prefix.getLength();
    return (prefixLength > 0) && (prefixLength <= getLength()) &&
      (
        compare<ucs4>(
          getBuffer(),
          Cast::pointer<const ucs4*>(prefix.getValue()),
          prefixLength
        ) == 0
      );
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

bool WideString::endsWith(const WideString& suffix) const throw() {
  unsigned int length = getLength();
  unsigned int suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool WideString::endsWith(const WideLiteral& suffix) const throw() {
  unsigned int suffixLength = suffix.getLength();
  if (sizeof(wchar) == sizeof(ucs2)) {
    if (suffixLength > getLength()) {
      return false;
    }
    // make sure string is terminated
    const ucs4* left = getElements() + getLength() - suffixLength;
    const ucs2* right =
      Cast::pointer<const ucs2*>(suffix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        WideStringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          WideStringException("Invalid sequence")
        );
        value =
          0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return false;
      }
      ++left;
      ++right;
    }
    return *right == 0;
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    return (suffixLength > 0) && (suffixLength <= getLength()) &&
      (
        compare<ucs4>(
          getBuffer() + getLength() - suffixLength,
          Cast::pointer<const ucs4*>(suffix.getValue()),
          suffixLength
        ) == 0
      );
  } else {
    throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

int WideString::indexOf(ucs4 character, unsigned int start) const throw() {
  const unsigned int length = getLength();
  if (start >= length) {
    return -1; // not found
  }
  
  const ucs4* buffer = getElements();
  const ucs4* result = find<ucs4>(buffer + start, length, character);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

int WideString::indexOf(
  const WideString& string, unsigned int start) const throw() {
  if ((start >= getLength()) || (string.getLength() > getLength())) {
    return -1; // not found
  }

  const ucs4* right = string.getElements();
  if (*right) {
    return 0;
  }
  
  const ucs4* begin = getElements();
  const ucs4* left = begin + start;
  
  while (true) {
    while (*left && (*right != *left)) { // skip to matching first character
      ++left;
    }
    if (!*left) { // have we reached the end
      break;
    }
    const ucs4* match = left;
    const ucs4* substr = right;
    while (*substr && (*substr == *match)) {
      ++match;
      ++substr;
    }
    if (!*substr) {
      return left - begin;
    }
    ++left;
  }
  return -1; // not found
}

int WideString::lastIndexOf(ucs4 character, unsigned int start) const throw() {
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const ucs4* buffer = getBuffer();
  const ucs4* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == character) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return -1; // not found
}

int WideString::lastIndexOf(
  const WideString& string, unsigned int start) const throw() {
  if ((string.isEmpty()) || (string.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  // TAG: todo

  return -1; // not found
}

unsigned int WideString::count(ucs4 character, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(character, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

unsigned int WideString::count(
  const WideString& string, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

String WideString::getMultibyteString() const
  throw(MultibyteException, MemoryException) {
  ASSERT((sizeof(wchar) == sizeof(ucs2)) || (sizeof(wchar) == sizeof(ucs4)));
  String result;
  const int numberOfCharacters = getLength(); // TAG: one character per element - could be 2
  if (numberOfCharacters) {
    const ucs4* buffer = getBuffer();
    if (sizeof(wchar) == sizeof(ucs2)) {
      int multibyteLength = UCS2ToUTF8(0, Cast::pointer<const ucs2*>(buffer), numberOfCharacters);
      bassert(
        (numberOfCharacters == 0) || (multibyteLength > 0),
        MultibyteException(Type::getType<WideString>())
      );
      result.forceToLength(multibyteLength);
      UCS2ToUTF8(
        Cast::pointer<uint8*>(result.getElements()),
        Cast::pointer<const ucs2*>(buffer),
        numberOfCharacters
      );
    } else {
      int multibyteLength = UCS4ToUTF8(0, Cast::pointer<const ucs4*>(buffer), numberOfCharacters);
      bassert(
        (numberOfCharacters == 0) || (multibyteLength > 0),
        MultibyteException(Type::getType<WideString>())
      );
      result.forceToLength(multibyteLength);
      UCS4ToUTF8(
        Cast::pointer<uint8*>(result.getElements()),
        Cast::pointer<const ucs4*>(buffer),
        numberOfCharacters
      );
    }
  }
  return result;
}

bool WideString::isUpperCased() const throw() {
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isUpper(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isLowerCased() const throw() {
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isLower(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isTitleCased() const throw() {
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isTitle(*i++)) {
      return false;
    }
  }
  return true;
}

template<>
int compare<WideString>(
  const WideString& left, const WideString& right) throw() {
  return left.compareTo(right);
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const WideString& value) throw(MultibyteException, IOException) {
  // TAG: need support for selected style (e.g. \uXXXXX)
  // TAG: FormatOutputStream - select between \uXXXXX and UTF8, UTF8LE, and UTF8BE, or other UTF
  return stream << value.getMultibyteString();
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  WideString::UnicodeCharacter character) throw(IOException) {
  
  char buffer[sizeof("&#4294967295;")]; // worst case length
  char* dest = buffer;
  unsigned int numberOfDigits = 1;
  unsigned int value = character.getCode();
  
  switch (character.getStyle()) {
  case WideString::STYLE_XML:
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
  case WideString::STYLE_PERL:
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
  case WideString::STYLE_CPP:
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

#if 0
std::wstring toWide(const WideString& s) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
  std::string r = convert.to_bytes((const char32_t*)s.getElements(), (const char32_t*)s.getElements() + s.getLength());
  return toWide(r);
}
#endif

std::string toUTF8(const WideString& s) {
  // TAG: not tested
  std::wstring_convert<std::codecvt_utf8<ucs4>, ucs4> convert;
  return convert.to_bytes(s.getElements(), s.getElements() + s.getLength());
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
