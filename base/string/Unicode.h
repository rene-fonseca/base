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

  /** Specifies the byte order mark. */
  static constexpr ucs4 BOM = 0x0000feff;
  
  /** Returns true if the code is a valid UCS4 code. */
  static inline bool isUCS4(unsigned int value) noexcept
  {
    if (value < 0xd800) {
      return true;
    } else if (value <= 0x10ffff) {
      return (value >= 0xe000);
    }
    return false;
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
  static int readUCS4(const uint8* src, const uint8* end, ucs4& ch) noexcept;

  /** Read UCS4 from null terminated sequence. */
  static int readUCS4(const uint8* src, ucs4& ch) noexcept;

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
    if (ch <= 0x7f) { // 7 bit
      *dest++ = static_cast<uint8>(ch);
      return 1;
    }
    if (ch <= 0x7ff) { // 11 bit
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
  
  /** Encoding flags. */
  enum EncodingFlags {
    /** Specifies that a BOM should be inserted when encoding to UTF. */
    ADD_BOM = 1
  };
  
  enum {
    INVALID_UCS4_CHARACTER = -1
  };

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-32BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF32BE)).
    
    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-32BE encoded string.
  */
  MemoryDiff UCS4ToUTF32BE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
