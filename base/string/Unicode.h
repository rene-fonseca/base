/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Unicode helper functions.
*/

class _COM_AZURE_DEV__BASE__API Unicode {
public:

  // TAG: add BOM detection
  
  /** Returns true if the code is a valid UCS4 code. */
  static inline bool isUCS4(unsigned int value) noexcept
  {
    if (value < 0xd800) {
      return true;
    } else if (value < 0x10000) {
      return (value >= 0xe000) && (value < 0xffff); // TAG: 0xfffe is special
    } else {
      return true;
    }
  }

  enum {
    UTF8_ERROR_EMPTY = 0,
    UTF8_ERROR_INCOMPLETE = -1,
    UTF8_ERROR_BAD_ENCODING = -2
  };

  /**
    Converts the UTF-8 bytes into UCS4 character.

    @param src The start of the buffer.
    @param end The end of the buffer.
    @param ch The result character.

    @return Bytes read. If negative this is the error code.
  */
  static int readUCS4(const uint8* src, const uint8* end, ucs4& ch) noexcept
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

  /** Read UCS4 from null terminated sequence. */
  static int readUCS4(const uint8* src, ucs4& ch) noexcept
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

  /** Validates if the the given string is using valid UTF-8 encoding. Returns the number of characters if valid. Otherwise returns negative status. */
  static MemoryDiff getUTF8StringLength(const uint8* src, const uint8* end) noexcept;

  /** Validates if the the given null-terminated string is using valid UTF-8 encoding. Returns the number of characters if valid. Otherwise returns negative status. */
  static MemoryDiff getUTF8StringLength(const uint8* src) noexcept;

  /** Returns number of bytes required for UTF-8 encoding of the given ucs4 character. Returns 0 is invalid ucs4 character. */
  static inline MemorySize getUTF8Bytes(ucs4 ch) noexcept
  {
    if (ch < ~0x7f) { // 7 bit
      return 1;
    }
    if (ch < ~0x7ff) { // 11 bit
      return 2;
    }
    if (ch <= 0xffff) { // 16 bit
      return 3;
    }
    if (ch <= 0x10ffff) { // 21 bit
      return 4;
    }
    return 0;
  }

  /** Converts UCS4 to UTF-8. Returns 0 is invalid ucs4 character. Buffer must have room for minimum 4 bytes. */
  static inline MemorySize writeUTF8(char* dest, ucs4 ch) noexcept
  {
    if (ch < ~0x7f) { // 7 bit
      *dest++ = static_cast<uint8>(ch);
      return 1;
    }
    if (ch < ~0x7ff) { // 11 bit
      *dest++ = static_cast<uint8>(((ch >> 6) & ((1 << 5) - 1)) | 0xc0);
      *dest++ = static_cast<uint8>(((ch >> 0) & ((1 << 6) - 1)) | 0x80);
      return 2;
    }
    if (ch <= 0xffff) { // 16 bit
      *dest++ = static_cast<uint8>(((ch >> 12) & ((1 << 4) - 1)) | 0xc0);
      *dest++ = static_cast<uint8>(((ch >> 6) & ((1 << 6) - 1)) | 0x80);
      *dest++ = static_cast<uint8>(((ch >> 0) & ((1 << 6) - 1)) | 0x80);
      return 3;
    }
    if (ch <= 0x10ffff) { // 21 bit
      *dest++ = static_cast<uint8>(((ch >> 18) & ((1 << 3) - 1)) | 0xc0);
      *dest++ = static_cast<uint8>(((ch >> 12) & ((1 << 6) - 1)) | 0x80);
      *dest++ = static_cast<uint8>(((ch >> 6) & ((1 << 6) - 1)) | 0x80);
      *dest++ = static_cast<uint8>(((ch >> 0) & ((1 << 6) - 1)) | 0x80);
      return 4;
    }
    return 0;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
