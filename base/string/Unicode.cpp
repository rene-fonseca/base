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

/** Encoding flags. */
enum EncodingFlags {
  /** Specifies that a BOM should be inserted when encoding to UTF. */
  ADD_BOM = 1,
  /** Specifies that the BOM should be skipped if present. */
  EAT_BOM = 2,
  /** Specifies that the BOM must be present. */
  EXPECT_BOM = 4,
  /** Specifies that the encoding is in native byte order. */
  ASSUME_NATIVE_BYTE_ORDER = 8,
  /**
    Specifies that the encoding is in big endian byte order (ignored if
    ASSUME_NATIVE_BYTE_ORDER is set).
  */
  ASSUME_BE = 16,
  /**
    Specifies that the encoding is in little endian byte order (ignored if
    ASSUME_NATIVE_BYTE_ORDER or ASSUME_BE are set).
  */
  ASSUME_LE = 32
};


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
    
    while (src < end) {
      const ucs4 code = *src++;
      if (code <= 0x0010ffff) {
        *dest++ = (code >> 24) & 0xff; // most significant
        *dest++ = (code >> 16) & 0xff;
        *dest++ = (code >> 8) & 0xff;
        *dest++ = (code >> 0) & 0xff;
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
  } else {
    while (src < end) {
      const ucs4 code = *src++;
      if (code <= 0x0010ffff) {
      } else {
        return INVALID_UCS4_CHARACTER;
      }
    }
    return 4 * (size + ((flags & ADD_BOM) ? 1 : 0));
  }
  return dest - begin;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
