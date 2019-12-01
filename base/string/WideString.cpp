/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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
#include <base/string/Unicode.h>
#include <base/UnexpectedFailure.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: add reference to RFC 2781
// TAG: RFC 2044
// TAG: need SCSU support see http://www.unicode.org/unicode/reports/tr6/
// TAG: need support for native unicode encoding (e.g. SYSTEM_UNICODE_ENCODING)?
// TAG: need input support for something like: "asdfd\u015fsdjfhd", "&#xXXXXX", and "&#DDDDD"
// TAG: need support for width calculation. NBWSP=>0???

// TAG: GCC 3.2 bug: fails to compile
// const WideTraits::CharacterDescriptor WideTraits::characters[0]; // when 0 elements
// getArraySize(characters);

bool WideTraits::isControl(ucs4 character) noexcept
{
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

bool WideTraits::isUpper(ucs4 character) noexcept
{
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

bool WideTraits::isLower(ucs4 character) noexcept
{
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

bool WideTraits::isTitle(ucs4 character) noexcept
{
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

unsigned int WideTraits::getFlags(ucs4 character) noexcept
{
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

ucs4 WideTraits::toLower(ucs4 character) noexcept
{
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

ucs4 WideTraits::toUpper(ucs4 character) noexcept
{
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

ucs4 WideTraits::toTitle(ucs4 character) noexcept
{
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

bool WideTraits::isSpace(ucs4 character) noexcept
{
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

bool WideTraits::isDigit(ucs4 character) noexcept
{
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

int WideTraits::digitToValue(ucs4 character) noexcept
{
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

namespace {

  inline ReferenceCountedAllocator<ucs4>* allocate(const wchar* text, MemorySize nativeLength)
  {
    // worst case length is nativeLength - allocate worst case and then release unused if possible
    const MemorySize length = Unicode::WCharToUCS4(nullptr, text, nativeLength);
    auto result = new ReferenceCountedAllocator<ucs4>(length + 1);
    Unicode::WCharToUCS4(result->getElements(), text, nativeLength);
    return result;
  }
}

void WideString::initialize(const wchar* string, MemorySize nativeLength)
{
  if (!nativeLength) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  elements = allocate(string, nativeLength);
}

void WideString::initialize(const ucs4* string, MemorySize nativeLength)
{
  if (!nativeLength) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  elements = new ReferenceCountedAllocator<ucs4>(nativeLength + 1);
  copy<ucs4>(elements->getElements(), string, nativeLength); // no overlap
}

void WideString::initialize(const char* string, MemorySize length)
{
  if (!length) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  const MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(nullptr, Cast::pointer<const uint8*>(string), length);
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      elements->getElements(),
      reinterpret_cast<const uint8*>(string),
      length
    );
  }
}

#if 0 // TAG: fixme
inline /*???*/
void WideString::invalidCode(int octetIndex, int suboctetIndex)
{
  throw MultibyteException(
    "Invalid character code.",
    Type::getType<WideString>(),
    MultibyteException::INVALID_CHARACTER_CODE,
    octetIndex,
    suboctetIndex
  );
}

inline /*???*/
void WideString::incompleteCode(int octetIndex, int suboctetIndex)
{
  throw MultibyteException(
    "Incomplete character code.",
    Type::getType<WideString>(),
    MultibyteException::INCOMPLETE_CHARACTER_CODE,
    octetIndex,
    suboctetIndex
  );
}
#endif

#if 0
MemorySize WideString::UTF16ToUCS4(ucs4* dest, const uint8* src, MemorySize size, unsigned int flags)
{
  if (!src) {
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
            const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
            src += 2;
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
            const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
            src += 2;
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
        unsigned int code = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
        src += 2;
        if (code <= 0xd7ff) {
          *dest++ = code; // always valid
        } else if (code <= 0xdbff) {
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
          src += 2;
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
        uint16 code = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
        src += 2;
        if (code <= 0xd7ff) {
        } else if (code <= 0xdbff) {
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
          src += 2;
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
        unsigned int code = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
        src += 2;
        if (code <= 0xd7ff) {
          *dest++ = code; // always valid
        } else if (code <= 0xdbff) {
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
          src += 2;
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
        unsigned int code = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
        src += 2;
        if (code <= 0xd7ff) {
        } else if (code <= 0xdbff) {
          bassert(src < end, MultibyteException("Invalid UTF-16 encoding", Type::getType<WideString>()));
          const uint16 temp = (static_cast<unsigned int>(src[0]) << 8) | (static_cast<unsigned int>(src[1]) << 0);
          src += 2;
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

WideString::MultibyteEncoding WideString::getMultibyteEncoding(const uint8* src, MemorySize size) noexcept
{
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
        bool validBe = (be <= 0x0000ffff) || ((be <= 0x0010ffff) && ((be & 0xf) < 0xe));
        bool validLe = (le <= 0x0000ffff) || ((le <= 0x0010ffff) && ((le & 0xf) < 0xe));
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
#endif

#if 0
MemorySize WideString::UTF32ToUCS4(ucs4* dest, const uint8* src, MemorySize size, unsigned int flags)
{
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

        bool validBe = (be <= 0x0000ffff) || ((be <= 0x0010ffff) && ((be & 0xf) < 0xe));
        bool validLe = (le <= 0x0000ffff) || ((le <= 0x0010ffff) && ((le & 0xf) < 0xe));
        
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
    break;
  case UTF32LE:
    break;
  default:
    throw MultibyteException("Invalid UTF-32 encoding");
  }
}
#endif

WideString::WideString(String::Default d)
{
  // force non-nullptr elements
  auto e = new ReferenceCountedAllocator<ucs4>(1);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

WideString::WideString() noexcept
  : elements(DEFAULT_STRING.elements)
{
}

WideString::WideString(MemorySize capacity)
{
  elements = new ReferenceCountedAllocator<ucs4>();
  elements->ensureCapacity(capacity + 1);
  elements->setSize(1); // after setting capacity
  ucs4* result = elements->getElements();
  result[0] = Traits::TERMINATOR;
}

WideString::WideString(const wchar* string)
{
  initialize(string, getNullTerminatedLength(string));
}

WideString::WideString(const wchar* string, MemorySize length)
{
  initialize(string, length);
}

WideString::WideString(const ucs4* string)
{
  initialize(string, getNullTerminatedLength(string));
}

WideString::WideString(const ucs4* string, MemorySize length)
{
  initialize(string, length);
}

WideString::WideString(const std::string& string)
{
  initialize(string.c_str(), string.size());
}

WideString::WideString(const std::wstring& string)
{
  initialize(string.c_str(), string.size());
}

WideString::WideString(const WideLiteral& literal)
{
  initialize(literal.getValue(), literal.getLength());
}

WideString::WideString(const NativeWideString& string)
{
  initialize(string.getValue(), string.getLength());
}

WideString::WideString(const NativeWideString& string, MemorySize maximum)
{
  if (maximum >= MAXIMUM_LENGTH) {
    throw OutOfDomain(this);
  }
  const MemorySize nativeLength = getNullTerminatedLength(string.getValue(), maximum);
  initialize(string, nativeLength);
}

WideString::WideString(const String& string)
{
  MemorySize multibyteLength = string.getLength();
  MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(
    nullptr,
    reinterpret_cast<const uint8*>(string.getElements()),
    multibyteLength
  );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      elements->getElements(),
      reinterpret_cast<const uint8*>(string.getElements()),
      multibyteLength
    );
  }
}

WideString::WideString(const NativeString& string)
{
  if (!string.getValue()) { // is string null
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const MemorySize multibyteLength = getNullTerminatedLength(string.getValue(), MAXIMUM_LENGTH);

  MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(
    nullptr,
    Cast::pointer<const uint8*>(string.getValue()),
    multibyteLength
  );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
    );
  }
}

WideString::WideString(const NativeString& string, MemorySize maximum)
{
  if (maximum >= MAXIMUM_LENGTH) {
    throw OutOfDomain(this);
  }
  
  if (!string.getValue()) { // is string null
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const MemorySize multibyteLength = getNullTerminatedLength(string.getValue(), maximum);
  
  MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(
    nullptr,
    Cast::pointer<const uint8*>(string.getValue()),
    multibyteLength
  );
  bassert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      elements->getElements(),
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
    );
  }
}

WideString& WideString::operator=(const WideLiteral& literal)
{
  initialize(literal.getValue(), literal.getLength());
  return *this;
}

bool WideString::isASCII() const noexcept
{
  const ucs4* i = getBuffer();
  const ucs4* end = i + getLength();
  while (i < end) {
    if (!ASCIITraits::isASCII(*i++)) {
      return false;
    }
  }
  return true;
}

void WideString::ensureCapacity(MemorySize capacity)
{
  elements->ensureCapacity(capacity); // no need to do copyOnWrite
}

void WideString::clear()
{
  elements = DEFAULT_STRING.elements;
}

void WideString::garbageCollect()
{
  elements->garbageCollect(); // no need to do copyOnWrite
}

ucs4 WideString::getAt(MemorySize index) const
{
  if (index >= getLength()) {
    throw OutOfRange(this);
  }
  return getBuffer()[index];
}

void WideString::setAt(MemorySize index, ucs4 value)
{
  if (index >= getLength()) {
    throw OutOfRange(this);
  }
  if (value != Traits::TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

WideString& WideString::remove(MemorySize start, MemorySize end)
{
  MemorySize length = getLength();
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

WideString& WideString::removeFrom(MemorySize start)
{
  if (start < getLength()) { // protect against some cases
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(start + 1); // remove section from end of string
  }
  return *this;
}

WideString& WideString::insert(MemorySize index, ucs4 ch)
{
  MemorySize length = getLength();
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

WideString& WideString::insert(MemorySize index, const WideString& str)
{
  MemorySize length = getLength();
  MemorySize strlength = str.getLength();
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

WideString& WideString::insert(MemorySize index, const WideLiteral& literal)
{
  const MemorySize length = getLength();
  const MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    const MemorySize _length = Unicode::WCharToUCS4(nullptr, literal.getValue(), nativeLength);
    setLength(length + _length);
    Unicode::WCharToUCS4(buffer + length, literal.getValue(), nativeLength);
  } else {
    // insert section in middle or beginning of string
    const MemorySize _length = Unicode::WCharToUCS4(nullptr, literal.getValue(), nativeLength);
    setLength(length + _length);
    move<ucs4>(buffer + index + _length, buffer + index, length - index);
    Unicode::WCharToUCS4(buffer + index, literal.getValue(), nativeLength);
  }
  return *this;
}

WideString& WideString::append(const WideLiteral& literal)
{
  const MemorySize length = getLength();
  const MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  const MemorySize _length = Unicode::WCharToUCS4(nullptr, literal.getValue(), nativeLength);
  setLength(length + _length);
  Unicode::WCharToUCS4(buffer + length, literal.getValue(), nativeLength);
  return *this;
}

WideString& WideString::append(const WideLiteral& literal, MemorySize maximum)
{
  const MemorySize length = getLength();
  const MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  MemorySize _length = Unicode::WCharToUCS4(nullptr, literal.getValue(), nativeLength);
  _length = minimum<MemorySize>(_length, maximum);
  setLength(length + _length);
  Unicode::WCharToUCS4(buffer + length, literal.getValue(), nativeLength);
  return *this;
}

WideString& WideString::append(const wchar* string, MemorySize maximum)
{
  bassert(
    maximum <= MAXIMUM_LENGTH,
    OutOfDomain(this)
  ); // maximum length exceeded
  bassert(
    string,
    StringException(this)
  ); // make sure string is proper (not empty)
  const MemorySize nativeLength = getNullTerminatedLength(string, maximum);
  MemorySize length = getLength();
  ucs4* buffer = elements->getElements();
  MemorySize stringLength = Unicode::WCharToUCS4(nullptr, string, nativeLength);
  stringLength = minimum<MemorySize>(stringLength, maximum);
  setLength(length + stringLength);
  Unicode::WCharToUCS4(buffer + length, string, stringLength);
  return *this;
}

WideString& WideString::replace(
  MemorySize start,
  MemorySize end,
  const WideString& str)
{
  // need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

MemorySize WideString::replaceAll(const WideString& fromStr, const WideString& toStr)
{
  MemorySize count = 0;
  MemorySize start = 0;
  MemoryDiff found = 0;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength() - 1, toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

WideString WideString::substring(MemorySize start, MemorySize end) const
{
  MemorySize length = getLength();
  if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    // 0 <= start < end <= getLength()
    MemorySize lengthOfSubstring = end - start;
    WideString result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    return WideString(); // return empty string
  }
}

WideString& WideString::operator-=(const WideString& suffix)
{
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

WideString& WideString::reverse() noexcept
{
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

WideString& WideString::toLowerCase() noexcept
{
  transform(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

WideString& WideString::toUpperCase() noexcept
{
  transform(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int WideString::compareTo(const WideString& string) const noexcept
{
  // both strings may contain multiple zeros
  MemorySize leftLength = getLength();
  MemorySize rightLength = string.getLength();
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

int WideString::compareTo(const WideLiteral& literal) const noexcept
{
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right =
      Cast::pointer<const utf16*>(literal.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence")
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

int WideString::compareTo(const wchar* string) const noexcept
{
  if (!string) {
    return false;
  }
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right = Cast::pointer<const utf16*>(string);
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence")
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

int WideString::compareToIgnoreCase(const ucs4* left, const ucs4* right) noexcept
{
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

int WideString::compareToIgnoreCase(const WideString& string) const noexcept
{
  return compareToIgnoreCase(getElements(), string.getElements());
}

// TAG: need support for WideLiteral
// int WideString::compareToIgnoreCase(const WideLiteral& literal) const noexcept;

bool WideString::startsWith(const WideString& prefix) const noexcept
{
  MemorySize prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool WideString::startsWith(const WideLiteral& prefix) const noexcept
{
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right = Cast::pointer<const utf16*>(prefix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence")
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
    MemorySize prefixLength = prefix.getLength();
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

bool WideString::endsWith(const WideString& suffix) const noexcept
{
  MemorySize length = getLength();
  MemorySize suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool WideString::endsWith(const WideLiteral& suffix) const noexcept
{
  MemorySize suffixLength = suffix.getLength();
  if (sizeof(wchar) == sizeof(utf16)) {
    if (suffixLength > getLength()) {
      return false;
    }
    // make sure string is terminated
    const ucs4* left = getElements() + getLength() - suffixLength;
    const utf16* right = Cast::pointer<const utf16*>(suffix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence")
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

MemoryDiff WideString::indexOf(ucs4 character, MemorySize start) const noexcept
{
  const MemorySize length = getLength();
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

MemoryDiff WideString::indexOf(const WideString& string, MemorySize start) const noexcept
{
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

MemoryDiff WideString::lastIndexOf(ucs4 character, MemorySize start) const noexcept
{
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

MemoryDiff WideString::lastIndexOf(const WideString& string, MemorySize start) const noexcept
{
  if ((string.isEmpty()) || (string.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  // TAG: todo

  return -1; // not found
}

MemorySize WideString::count(ucs4 character, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(character, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

MemorySize WideString::count(const WideString& string, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

bool WideString::isUpperCased() const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isUpper(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isLowerCased() const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isLower(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isTitleCased() const noexcept
{
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
int compare<WideString>(const WideString& left, const WideString& right)
{
  return left.compareTo(right);
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value)
{
  String temp(value);
  return stream << temp;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(WideString) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/string");
  TEST_IMPACT(CRITICAL)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    WideString a;
    TEST_ASSERT(!a);
    TEST_ASSERT(a.isEmpty());
    WideString b("literal");
    a = "qwerty";
    TEST_ASSERT(a);
    TEST_ASSERT(!a.isEmpty());
    auto c = a + b;
    TEST_ASSERT(c == "qwertyliteral");
    
    TEST_ASSERT(c.indexOf('l') == 6);
    TEST_ASSERT(c.indexOf("literal") == 6);
    TEST_ASSERT(c.lastIndexOf('y') == 5);
    TEST_ASSERT(c.lastIndexOf("literal") == 6);

#if 0
    c = WideString();
    // c.clear();
    c.append(L"Hello");
    c.append(L" ");
    c.append(L"World!");
    TEST_ASSERT(c == L"Hello World!");
    TEST_PRINT(c);
    c.reverse();
    TEST_PRINT(c);
#endif
    
    // TEST_ASSERT(c[6] == 'W');

#if 0
    MemorySize count = 0;
    for (auto ch : c) {
      ++count;
    }
    TEST_ASSERT(count == c.getLength());
#endif
    
    c.garbageCollect();
    
    // TEST_ASSERT((String("=*-") * 10).getLength() == 30);
  }
};

TEST_REGISTER(WideString);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
