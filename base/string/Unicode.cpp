/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Unicode.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Literal Unicode::getMIMECharset(MultibyteEncoding encoding) noexcept
{
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

int Unicode::readUCS4(const uint8* src, const uint8* end, ucs4& ch) noexcept
{
  if (src == end) {
    return ERROR_EMPTY;
  }

  uint8 value = *src++;
  if ((value & 0x80) == 0x00) {
    ch = value;
    return 1;
  }

  if ((value & 0xe0) == 0xc0) {
    ucs4 code = value & ~0xe0;
    if (src == end) {
      return ERROR_INCOMPLETE;
    }
    uint8 temp = *src++;
    if ((temp & 0xc0) != 0x80) {
      return ERROR_BAD_ENCODING;
    }
    code = (code << 6) | (temp & 0x3f);
    ch = code;
    return 2;
  }

  if ((value & 0xf0) == 0xe0) {
    ucs4 code = value & ~0xf0;
    for (int i = 0; i < 2; ++i) {
      if (src == end) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 3;
  }

  if ((value & 0xf8) == 0xf0) {
    ucs4 code = value & ~0xf8;
    for (int i = 0; i < 3; ++i) {
      if (src == end) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 4;
  }

  if ((value & 0xfc) == 0xf8) {
    ucs4 code = value & ~0xfc;
    for (int i = 0; i < 4; ++i) {
      if (src == end) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 5;
  }

  if ((value & 0xfe) == 0xfc) {
    ucs4 code = value & ~0xfe;
    for (int i = 0; i < 5; ++i) {
      if (src == end) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 6;
  }
  return ERROR_BAD_ENCODING;
}

int Unicode::readUCS4(const uint8* src, ucs4& ch) noexcept
{
  if (!src || !*src) {
    return ERROR_EMPTY;
  }

  uint8 value = *src++;
  if ((value & 0x80) == 0x00) {
    ch = value;
    return 1;
  }

  if ((value & 0xe0) == 0xc0) {
    ucs4 code = value & ~0xe0;
    if (!*src) {
      return ERROR_INCOMPLETE;
    }
    uint8 temp = *src++;
    if ((temp & 0xc0) != 0x80) {
      return ERROR_BAD_ENCODING;
    }
    code = (code << 6) | temp;
    ch = code;
    return 2;
  }

  if ((value & 0xf0) == 0xe0) {
    ucs4 code = value & ~0xf0;
    for (int i = 0; i < 2; ++i) {
      if (!*src) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 3;
  }

  if ((value & 0xf8) == 0xf0) {
    ucs4 code = value & ~0xf8;
    for (int i = 0; i < 3; ++i) {
      if (!*src) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 4;
  }

  if ((value & 0xfc) == 0xf8) {
    ucs4 code = value & ~0xfc;
    for (int i = 0; i < 4; ++i) {
      if (!*src) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 5;
  }

  if ((value & 0xfe) == 0xfc) {
    ucs4 code = value & ~0xfe;
    for (int i = 0; i < 5; ++i) {
      if (!*src) {
        return ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 6;
  }
  return ERROR_BAD_ENCODING;
}

int Unicode::readUCS4(const utf16* src, ucs4& ch) noexcept
{
  if (!src) {
    return 0;
  }

  const utf16 code = *src++;
  if ((code >= 0xd800) && (code <= 0xdfff)) { // surrogate codes
    if (code >= 0xdc00) { // expecting high surrogate
      return ERROR_BAD_ENCODING;
    }
    if (*src) {
      return ERROR_INCOMPLETE;
    }
    const uint16 high = code - 0xd800;
    const uint16 low = (*src++ - 0xdc00);
    ch = (static_cast<ucs4>(high) << 10) | low;
    return 2;
  } else {
    ch = code;
    return 1;
  }
}

MemoryDiff Unicode::getUTF8StringLength(const uint8* src, const uint8* end) noexcept
{
  MemoryDiff length = 0;
  while (src != end) {
    ucs4 ch = 0;
    auto status = readUCS4(src, end, ch);
    if (status > 0) {
      src += status;
      ++length;
      continue;
    }
    return status;
  }
  return length;
}

MemoryDiff Unicode::getUTF8StringLength(const uint8* src) noexcept
{
  if (!src || !*src) {
    return 0;
  }
  MemoryDiff length = 0;
  while (*src) {
    ucs4 ch = 0;
    auto status = readUCS4(src, ch);
    if (status > 0) {
      src += status;
      ++length;
      continue;
    }
    return status;
  }
  return length;
}

MemoryDiff Unicode::getUTF8StringLength(const uint8* src, MemorySize size) noexcept
{
  return getUTF8StringLength(src, src + size);
}

MemoryDiff Unicode::getStringLength(const utf16* src) noexcept
{
  MemoryDiff length = UTF16ToUCS4(nullptr, src, getNullTerminatedLength(src));
  return length;
}

MemoryDiff Unicode::getStringLength(const utf16* src, MemorySize size) noexcept
{
  MemoryDiff length = UTF16ToUCS4(nullptr, src, size);
  return length;
}

MemoryDiff Unicode::getStringLength(const char16_t* src) noexcept
{
  MemoryDiff length = UTF16ToUCS4(nullptr, reinterpret_cast<const utf16*>(src), getNullTerminatedLength(src));
  return length;
}

MemoryDiff Unicode::getStringLength(const char16_t* src, MemorySize size) noexcept
{
  MemoryDiff length = UTF16ToUCS4(nullptr, reinterpret_cast<const utf16*>(src), size);
  return length;
}

MemoryDiff Unicode::getStringLength(const wchar* src) noexcept
{
  return WCharToUCS4(nullptr, src, getNullTerminatedLength(src));
}

MemoryDiff Unicode::getStringLength(const wchar* src, MemorySize size) noexcept
{
  return WCharToUCS4(nullptr, src, size);
}

MemoryDiff Unicode::UCS4ToUCS2(ucs2* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const ucs2* const begin = dest;
  const ucs4* end = src + size;
  if (dest) { // C++: compiler hint optimize and run to code pick best solution
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        *dest++ = value;
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        *dest++ = value;
      } else if (value <= MAX) {
        return INVALID_UCS2_CHARACTER;
        // *dest++ = ((value - 0x10000) >> 10) + 0xd800; // 10 bit - high
        // *dest++ = ((value - 0x10000) & 0x3ff) + 0xdc00; // 10 bit - low
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
      } else if (value <= MAX) {
        return INVALID_UCS2_CHARACTER;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UCS2ToUCS4(ucs4* dest, const ucs2* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const ucs4* const begin = dest;
  const ucs2* end = src + size;
  if (dest) {
    while (src != end) {
      const ucs2 code = *src++;
      if ((code >= 0xd800) && (code <= 0xdfff)) {
        return INVALID_UCS2_CHARACTER;
      }
      *dest++ = code;
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src != end) {
      const ucs2 code = *src++;
      if ((code >= 0xd800) && (code <= 0xdfff)) {
        return INVALID_UCS2_CHARACTER;
      }
      ++length;
    }
    return length;
  }
}

MemoryDiff Unicode::UCS2ToUTF8(uint8* dest, const ucs2* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
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
    while (src != end) {
      const ucs2 code = *src++;
      if (code <= 0x7fU) {
        *dest++ = static_cast<uint8>(code);
      } else if (code <= 0x7ffU) {
        *dest++ = 0xc0 | (code >> 6); // 5 bit - most significant
        *dest++ = 0x80 | ((code >> 0) & ((1 << 6) - 1)); // 6 bit
      } else {
        if ((code >= 0xd800) && (code <= 0xdfff)) {
          return INVALID_UCS2_CHARACTER;
        }
        *dest++ = 0xe0 | (code >> 12); // 4 bit - most significant
        *dest++ = 0x80 | ((code >> 6) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((code >> 0) & ((1 << 6) - 1)); // 6 bit
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 3; // <0xef> <0xbb> <0xbf>
    }
    while (src != end) {
      const ucs2 code = *src++;
      if (code <= 0x7fU) {
        ++length;
      } else if (code <= 0x7ffU) {
        length += 2;
      } else {
        if ((code >= 0xd800) && (code <= 0xdfff)) {
          return INVALID_UCS2_CHARACTER;
        }
        length += 3;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UTF16ToUTF8(uint8* dest, const utf16* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const uint8* const begin = dest;
  const utf16* end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      dest += writeUTF8(dest, BOM);
    }
    while (src != end) {
      const utf16 code = *src++;
      if (code < 0xd800) {
        dest += writeUTF8(dest, code);
      } else if (code <= 0xdbff) {
        if (src == end) {
          return ERROR_INCOMPLETE;
        }
        const utf16 lowSurrogate = *src++;
        if (!((code >= 0xdc00) && (code <= 0xdfff))) {
          return ERROR_BAD_ENCODING;
        }
        const ucs4 ch = 0x10000U +
          (static_cast<ucs4>(code - 0xd800) << 10) |
          static_cast<ucs4>(lowSurrogate - 0xdc00);
        dest += writeUTF8(dest, ch);
      } else if (code <= 0xdfff) {
        return ERROR_BAD_ENCODING;
      } else {
        dest += writeUTF8(dest, code);
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 3; // <0xef> <0xbb> <0xbf>
    }
    while (src != end) {
      const utf16 code = *src++;
      if (code <= 0x7fU) {
        ++length;
      } else if (code <= 0x7ffU) {
        length += 2;
      } else {
        if ((code >= 0xdc00) && (code <= 0xdfff)) {
          return ERROR_BAD_ENCODING;
        }
        if ((code >= 0xd800) && (code <= 0xdbff)) {
          if (src == end) {
            return ERROR_INCOMPLETE;
          }
          const utf16 lowSurrogate = *src++;
          if (!((lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff))) {
            return ERROR_BAD_ENCODING;
          }
          length += 4;
        } else {
          length += 3;
        }
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UCS4ToUTF8(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
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
    while (src != end) {
      ucs4 value = *src++;
      if (value <= 0x7fU) {
        *dest++ = value;
      } else if (value <= 0x7ffU) {
        *dest++ = 0xc0 | (value >> 6); // 5 bit - most significant
        *dest++ = 0x80 | ((value >> 0) & ((1 << 6) - 1)); // 6 bit
      } else if (value <= 0xffffU) {
        *dest++ = 0xe0 | (value >> 12); // 4 bit - most significant
        *dest++ = 0x80 | ((value >> 6) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 0) & ((1 << 6) - 1)); // 6 bit
      } else if (value <= 0x1fffffU) {
        *dest++ = 0xf0 | (value >> 18); // 3 bit - most significant
        *dest++ = 0x80 | ((value >> 12) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 6) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 0) & ((1 << 6) - 1)); // 6 bit
      } else if (value <= 0x3ffffffU) {
        *dest++ = 0xf8 | (value >> 24); // 3 bit - most significant
        *dest++ = 0x80 | ((value >> 18) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 12) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 6) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 0) & ((1 << 6) - 1)); // 6 bit
      } else if (value <= MAX_ISO) {
        *dest++ = 0xfc | (value >> 30); // 1 bit - most significant
        *dest++ = 0x80 | ((value >> 24) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 18) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 12) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 6) & ((1 << 6) - 1)); // 6 bit
        *dest++ = 0x80 | ((value >> 0) & ((1 << 6) - 1)); // 6 bit
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 3; // <0xef> <0xbb> <0xbf>
    }
    while (src != end) {
      ucs4 value = *src++;
      if (value <= 0x7fU) {
        ++length;
      } else if (value <= 0x7ffU) {
        length += 2;
      } else if (value <= 0xffffU) {
        length += 3;
      } else if (value <= 0x1fffffU) {
        length += 4;
      } else if (value <= 0x3ffffffU) {
        length += 5;
      } else if (value <= 0x7fffffffU) {
        length += 6;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UTF8ToUTF16(utf16* dest, const uint8* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }

  const uint8* const srcBegin = src;
  const uint8* const end = src + size;

  if (flags & EAT_BOM) {
    if (src != end) {
      ucs4 ch = 0;
      const uint8* _src = src;
      int status = Unicode::readUCS4(_src, end, ch);
      if (status <= 0) {
        return status;
      }
      if (ch == BOM) {
        src = _src;
      }
    }
  }
  
  if (dest) {
    const utf16* const begin = dest;
    while (src != end) {
      ucs4 ch = 0;
      int status = Unicode::readUCS4(src, end, ch);
      if (status <= 0) {
        return status;
      }
      src += status; // bytes read
      if (ch > MAX) {
        return INVALID_UCS4_CHARACTER;
      } else if (ch >= 0x10000) {
        *dest++ = ((ch - 0x10000) >> 10) + 0xd800; // 10 bit - high
        *dest++ = ((ch - 0x10000) & 0x3ff) + 0xdc00; // 10 bit - low
      } else {
        *dest++ = ch;
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src != end) {
      ucs4 ch = 0;
      int status = Unicode::readUCS4(src, end, ch);
      if (status <= 0) {
        return status;
      }
      src += status; // bytes read
      length += getUTF16Words(ch);
    }
    return length;
  }
}

MemoryDiff Unicode::UTF8ToUCS4(ucs4* dest, const uint8* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }

  const uint8* const srcBegin = src;
  const uint8* const end = src + size;

  if (flags & EAT_BOM) {
    if (src != end) {
      ucs4 ch = 0;
      const uint8* _src = src;
      int status = Unicode::readUCS4(_src, end, ch);
      if (status <= 0) {
        return status;
      }
      if (ch == BOM) {
        src = _src;
      }
    }
  }
  
  if (dest) {
    const ucs4* const begin = dest;
    while (src != end) {
      ucs4 ch = 0;
      int status = Unicode::readUCS4(src, end, ch);
      if (status <= 0) {
        return status;
      }
      src += status; // bytes read
      *dest++ = ch;
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src != end) {
      ucs4 ch = 0;
      int status = Unicode::readUCS4(src, end, ch);
      if (status <= 0) {
        return status;
      }
      src += status; // bytes read
      ++length;
    }
    return length;
  }
}

MemoryDiff Unicode::UCS4ToUTF16BE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = static_cast<uint8>((BOM >> 8) & 0xff); // most significant
      *dest++ = static_cast<uint8>((BOM >> 0) & 0xff);
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        *dest++ = static_cast<uint8>((value >> 8) & 0xff); // most significant
        *dest++ = static_cast<uint8>((value >> 0) & 0xff);
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        *dest++ = static_cast<uint8>((value >> 8) & 0xff); // most significant
        *dest++ = static_cast<uint8>((value >> 0) & 0xff);
      } else if (value <= MAX) {
        uint16 high = ((value - 0x10000) >> 10) + 0xd800; // 10 bit - high
        *dest++ = static_cast<uint8>((high >> 8) & 0xff); // most significant
        *dest++ = static_cast<uint8>((high >> 0) & 0xff);
        uint16 low = ((value - 0x10000) & 0x3ff) + 0xdc00; // 10 bit - low
        *dest++ = static_cast<uint8>((low >> 8) & 0xff); // most significant
        *dest++ = static_cast<uint8>((low >> 0) & 0xff);
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 2;
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        length += 2;
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        length += 2;
      } else if (value <= MAX) {
        length += 2 * 2;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UCS4ToUTF16LE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const uint8* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = static_cast<uint8>((BOM >> 0) & 0xff);
      *dest++ = static_cast<uint8>((BOM >> 8) & 0xff); // most significant
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        *dest++ = static_cast<uint8>((value >> 0) & 0xff);
        *dest++ = static_cast<uint8>((value >> 8) & 0xff); // most significant
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        *dest++ = static_cast<uint8>((value >> 0) & 0xff);
        *dest++ = static_cast<uint8>((value >> 8) & 0xff); // most significant
      } else if (value <= MAX) {
        uint16 high = ((value - 0x10000) >> 10) + 0xd800; // 10 bit - high
        *dest++ = static_cast<uint8>((high >> 0) & 0xff);
        *dest++ = static_cast<uint8>((high >> 8) & 0xff); // most significant
        uint16 low = ((value - 0x10000) & 0x3ff) + 0xdc00; // 10 bit - low
        *dest++ = static_cast<uint8>((low >> 0) & 0xff);
        *dest++ = static_cast<uint8>((low >> 8) & 0xff); // most significant
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      length += 2;
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        length += 2;
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        length += 2;
      } else if (value <= MAX) {
        length += 2 * 2;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UTF16ToUCS4(ucs4* dest, const utf16* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const ucs4* const begin = dest;
  const utf16* end = src + size;
  if (dest) {
    while (src != end) {
      const utf16 code = *src++;
      if ((code >= 0xd800) && (code <= 0xdfff)) { // surrogate codes
        if (code >= 0xdc00) { // expecting high surrogate
          return ERROR_BAD_ENCODING;
        }
        if (src == end) {
          return ERROR_INCOMPLETE;
        }
        const uint16 high = code - 0xd800;
        const uint16 low = (*src++ - 0xdc00);
        *dest++ = (static_cast<ucs4>(high) << 10) | low;
      } else {
        *dest++ = code;
      }
    }
    return dest - begin;
  } else { // calculate length and validate
    MemorySize length = 0;
    while (src != end) {
      const utf16 code = *src++;
      if ((code >= 0xd800) && (code <= 0xdfff)) { // surrogate codes
        if (code >= 0xdc00) { // expecting high surrogate
          return ERROR_BAD_ENCODING;
        }
        if (src == end) {
          return ERROR_INCOMPLETE;
        }
      }
      ++length;
    }
    return length;
  }
}

MemoryDiff Unicode::UCS4ToUTF16(utf16* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
    return 0;
  }
  const utf16* const begin = dest;
  const ucs4* const end = src + size;
  if (dest) {
    if (flags & ADD_BOM) {
      *dest++ = BOM;
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        *dest++ = value;
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        *dest++ = value;
      } else if (value <= MAX) {
        uint16 high = ((value - 0x10000) >> 10) + 0xd800; // 10 bit - high
        *dest++ = high;
        uint16 low = ((value - 0x10000) & 0x3ff) + 0xdc00; // 10 bit - low
        *dest++ = low;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    if (flags & ADD_BOM) {
      ++length;
    }
    while (src != end) {
      const ucs4 value = *src++;
      if (value < 0xd800) {
        ++length;
      } else if (value <= 0xdfff) {
        return INVALID_UCS4_CHARACTER;
      } else if (value <= 0xffff) {
        ++length;
      } else if (value <= MAX) {
        length += 2;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return length;
  }
}

MemoryDiff Unicode::UCS4ToUTF32BE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
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
    
    while (src != end) {
      const ucs4 code = *src++;
      if (code <= MAX) {
        *dest++ = (code >> 24) & 0xff; // most significant
        *dest++ = (code >> 16) & 0xff;
        *dest++ = (code >> 8) & 0xff;
        *dest++ = (code >> 0) & 0xff;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
  } else {
    while (src != end) {
      const ucs4 code = *src++;
      if (code <= MAX) {
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
  return dest - begin;
}

MemoryDiff Unicode::UCS4ToUTF32LE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept
{
  if (!src) {
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
    
    while (src != end) {
      const ucs4 code = *src++;
      if (code <= MAX) {
        *dest++ = (code >> 0) & 0xff; // least significant
        *dest++ = (code >> 8) & 0xff;
        *dest++ = (code >> 16) & 0xff;
        *dest++ = (code >> 24) & 0xff;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return dest - begin;
  } else {
    while (src != end) {
      const ucs4 code = *src++;
      if (code <= MAX) {
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
}

MemoryDiff Unicode::UTF32LEToUCS4(
  ucs4* dest,
  const uint8* src,
  MemorySize size,
  unsigned int flags)
{
  if (!src) {
    return 0;
  }
  
  // handle BOM
  const ucs4* const begin = dest;
  const uint8* const end = src + size;

  if (dest) {
    while (src != end) {
      uint32 code = 0;
      code |= static_cast<uint32>(*src++) << 0;
      code |= static_cast<uint32>(*src++) << 8;
      code |= static_cast<uint32>(*src++) << 16;
      code |= static_cast<uint32>(*src++) << 24; // most significant
      if (code >= MAX_ISO) {
        return INVALID_UCS4_CHARACTER;
      }
      *dest++ = code;
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    while (src != end) {
      uint32 code = 0;
      code |= static_cast<uint32>(*src++) << 0;
      code |= static_cast<uint32>(*src++) << 8;
      code |= static_cast<uint32>(*src++) << 16;
      code |= static_cast<uint32>(*src++) << 24; // most significant
      if (code >= MAX_ISO) {
        return INVALID_UCS4_CHARACTER;
      }
      ++length;
    }
    return length;
  }
}

MemoryDiff Unicode::UTF32BEToUCS4(
  ucs4* dest,
  const uint8* src,
  MemorySize size,
  unsigned int flags)
{
  if (!src) {
    return 0;
  }
  
  // handle BOM
  const ucs4* const begin = dest;
  const uint8* const end = src + size;

  if (dest) {
    while (src != end) {
      uint32 code = 0;
      code |= static_cast<uint32>(*src++) << 24; // most significant
      code |= static_cast<uint32>(*src++) << 16;
      code |= static_cast<uint32>(*src++) << 8;
      code |= static_cast<uint32>(*src++) << 0;
      if (code >= MAX_ISO) {
        return INVALID_UCS4_CHARACTER;
      }
      *dest++ = code;
    }
    return dest - begin;
  } else {
    MemorySize length = 0;
    while (src != end) {
      uint32 code = 0;
      code |= static_cast<uint32>(*src++) << 24; // most significant
      code |= static_cast<uint32>(*src++) << 16;
      code |= static_cast<uint32>(*src++) << 8;
      code |= static_cast<uint32>(*src++) << 0;
      if (code >= MAX_ISO) {
        return INVALID_UCS4_CHARACTER;
      }
      ++length;
    }
    return length;
  }
}

#if 0
Unicode::MultibyteEncoding Unicode::getMultibyteEncoding(const uint8* src, MemorySize size) noexcept
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
        uint32 be = (static_cast<uint32>(s[0]) << 24) |
          (static_cast<uint32>(s[1]) << 16) |
          (static_cast<uint32>(s[2]) << 8) |
          (static_cast<uint32>(s[3]) << 0);
        uint32 le = (static_cast<uint32>(s[0]) << 0) |
          (static_cast<uint32>(s[1]) << 8) |
          (static_cast<uint32>(s[2]) << 16) |
          (static_cast<uint32>(s[3]) << 24);
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

ToWCharString::ToWCharString()
{
}

ToWCharString::ToWCharString(const char* string)
{
  const MemorySize length = getNullTerminatedLength(string);
  const MemoryDiff _length = Unicode::UTF8ToWChar(nullptr, reinterpret_cast<const uint8*>(string), length);
  buffer.setSize(_length + 1);
  Unicode::UTF8ToWChar(buffer.getElements(), reinterpret_cast<const uint8*>(string), length);
  buffer.getElements()[_length] = 0;
  this->string = buffer.getElements();
}

ToWCharString::ToWCharString(const wchar* string)
{
  this->string = string;
}

ToWCharString::ToWCharString(const ucs4* string)
{
  if (sizeof(wchar) == sizeof(ucs4)) {
    this->string = reinterpret_cast<const wchar*>(string);
  } else {
    const MemorySize length = getNullTerminatedLength(string);
    const MemoryDiff _length = Unicode::UCS4ToWChar(nullptr, string, length);
    buffer.setSize(_length + 1);
    Unicode::UCS4ToWChar(buffer.getElements(), string, length);
    buffer.getElements()[_length] = 0;
    this->string = buffer.getElements();
  }
}

ToWCharString::ToWCharString(const String& string)
{
  const MemorySize length = getNullTerminatedLength(string.native());
  const MemoryDiff _length = Unicode::UTF8ToWChar(nullptr, reinterpret_cast<const uint8*>(string.native()), length);
  buffer.setSize(_length + 1);
  Unicode::UTF8ToWChar(buffer.getElements(), reinterpret_cast<const uint8*>(string.native()), length);
  buffer.getElements()[_length] = 0;
  this->string = buffer.getElements();
}

ToWCharString::ToWCharString(const WideString& string)
{
  if (sizeof(wchar) == sizeof(ucs4)) {
    this->string = reinterpret_cast<const wchar*>(string.native());
  } else {
    const MemorySize length = getNullTerminatedLength(string.native());
    const MemoryDiff _length = Unicode::UCS4ToWChar(nullptr, string.native(), length);
    buffer.setSize(_length + 1);
    Unicode::UCS4ToWChar(buffer.getElements(), string.native(), length);
    buffer.getElements()[_length] = 0;
    this->string = buffer.getElements();
  }
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Unicode) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/string");

#if !defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
  typedef char char8_t;
#endif

  void run() override
  {
    const char8_t* s8 = u8"abc 123 æøå ! ÆØÅ ! âêîôû";
    TEST_ASSERT(Unicode::UTF8ToUCS4(nullptr, reinterpret_cast<const uint8*>(s8), getNullTerminatedLength(s8)) == 25);
    const char16_t* s16 = u"abc 123 æøå ! ÆØÅ ! âêîôû";
    TEST_ASSERT(Unicode::UTF16ToUCS4(nullptr, s16, getNullTerminatedLength(s16)) == 25);
    const char32_t* s32 = U"abc 123 æøå ! ÆØÅ ! âêîôû";
    TEST_ASSERT(getNullTerminatedLength(s32) == 25);
  }
};

TEST_REGISTER(Unicode);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
