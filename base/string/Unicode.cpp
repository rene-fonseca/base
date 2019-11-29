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
    return UTF8_ERROR_EMPTY;
  }

  uint8 value = *src++;
  if ((value & 0x80) == 0x00) {
    ch = value;
    return 1;
  }

  if ((value & 0xe0) == 0xc0) {
    ucs4 code = value & ~0xe0;
    if (src == end) {
      return UTF8_ERROR_INCOMPLETE;
    }
    uint8 temp = *src++;
    if ((temp & 0xc0) != 0x80) {
      return UTF8_ERROR_BAD_ENCODING;
    }
    code = (code << 6) | (temp & 0x3f);
    ch = code;
    return 2;
  }

  if ((value & 0xf0) == 0xe0) {
    ucs4 code = value & ~0xf0;
    for (int i = 0; i < 2; ++i) {
      if (src == end) {
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 6;
  }
  return UTF8_ERROR_BAD_ENCODING;
}

int Unicode::readUCS4(const uint8* src, ucs4& ch) noexcept
{
  if (!src || !*src) {
    return UTF8_ERROR_EMPTY;
  }

  uint8 value = *src++;
  if ((value & 0x80) == 0x00) {
    ch = value;
    return 1;
  }

  if ((value & 0xe0) == 0xc0) {
    ucs4 code = value & ~0xe0;
    if (!*src) {
      return UTF8_ERROR_INCOMPLETE;
    }
    uint8 temp = *src++;
    if ((temp & 0xc0) != 0x80) {
      return UTF8_ERROR_BAD_ENCODING;
    }
    code = (code << 6) | temp;
    ch = code;
    return 2;
  }

  if ((value & 0xf0) == 0xe0) {
    ucs4 code = value & ~0xf0;
    for (int i = 0; i < 2; ++i) {
      if (!*src) {
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
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
        return UTF8_ERROR_INCOMPLETE;
      }
      uint8 temp = *src++;
      if ((temp & 0xc0) != 0x80) {
        return UTF8_ERROR_BAD_ENCODING;
      }
      code = (code << 6) | (temp & 0x3f);
    }
    ch = code;
    return 6;
  }
  return UTF8_ERROR_BAD_ENCODING;
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
          return INVALID_ENCODING;
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
          return INVALID_ENCODING;
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

// TAG: add test

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
