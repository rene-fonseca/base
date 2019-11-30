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
#include <base/Literal.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Unicode helper functions.
*/

class _COM_AZURE_DEV__BASE__API Unicode {
public:

  /** Specifies the byte order mark. */
  static constexpr ucs4 BOM = 0x0000feff;
  /** Specifies the maximum valid UCS4 code (Unicode). */
  static constexpr ucs4 MAX = 0x10ffff;
  /** The ISO/IEC 10646 standard. */
  static constexpr ucs4 MAX_ISO = 0x7ffffff;
  /** Invalid code. */
  static constexpr ucs4 BAD = 0xffffffff;

  /** Multibyte encoding. */
  enum MultibyteEncoding {
    /** Unicode transformation format (UTF-7). */
    UTF7,
    /** Unicode transformation format (UTF-8). */
    UTF8,
    /** Unicode transformation format (UTF-16). */
    UTF16,
    /** Unicode transformation format (UTF-16) with big endian byte order. */
    UTF16BE,
    /** Unicode transformation format (UTF-16) with little endian byte order. */
    UTF16LE,
    /** Unicode transformation format (UTF-32). */
    UTF32,
    /** Unicode transformation format (UTF-32) with big endian byte order. */
    UTF32BE,
    /** Unicode transformation format (UTF-32) with little endian byte order. */
    UTF32LE
  };

  /**
    Returns the maximum number of bytes required to represent any UCS-4 character.
  */
  static inline unsigned int getMaximumNumberOfMultibytes(MultibyteEncoding encoding) noexcept
  {
    static const unsigned int MAXIMUM_MULTIBYTES[] = {
      0 /*FIXME*/, 6 /*4 for valid code*/, 4 /*surrogate pairs*/, 4 /*surrogate pairs*/, 4 /*surrogate pairs*/, 4, 4, 4
    };
    return MAXIMUM_MULTIBYTES[encoding];
  }

  /**
    Returns a MIME charsets for the specified encoding.

    @param encoding The multibyte encoding.
  */
  static Literal getMIMECharset(MultibyteEncoding encoding) noexcept;
  
  // TAG: static ucs4 makeFromSurrogates(uint16, uint16) noexcept;
  // TAG: static struct{uint16, uint16} makeSurrogates(ucs4) noexcept;

  /** Returns true if the code is reserved surrogate code. */
  static inline bool isSurrogateCode(unsigned int value) noexcept
  {
    return (value >= 0xd800) && (value <= 0xdfff);
  }

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
    ERROR_EMPTY = 0,
    ERROR_INCOMPLETE = -1,
    ERROR_BAD_ENCODING = -2,
    INVALID_UCS4_CHARACTER = -3,
    INVALID_UCS2_CHARACTER = -4
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

  /**
    Validates if the the given string is using valid UTF-8 encoding. Returns the number of characters if valid.
    Otherwise returns negative status.
  */
  static MemoryDiff getUTF8StringLength(const uint8* src, const uint8* end) noexcept;

  /**
    Validates if the the given null-terminated string is using valid UTF-8 encoding. Returns the number of characters
    if valid. Otherwise returns negative status.
  */
  static MemoryDiff getUTF8StringLength(const uint8* src) noexcept;

  /**
    Returns number of bytes required for UTF-8 encoding of the given UCS4 character. Returns 0 if invalid UCS4
    character.
  */
  static inline MemorySize getUTF8Bytes(ucs4 ch) noexcept
  {
    if (ch <= 0x7f) { // 7 bit
      return 1;
    } else if (ch <= 0x7ff) { // 11 bit
      return 2;
    } else if (ch <= 0xffff) { // 16 bit
      return 3;
    } else if (ch <= 0x10ffff) { // 21 bit
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

  // TAG: add support for streaming of chars
  // TAG: need validation methods / guidelines

  /**
    Low-level method which converts an UCS-2 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static MemoryDiff UCS2ToUTF8(
    uint8* dest,
    const ucs2* src,
    MemorySize size,
    unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UTF16 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static MemoryDiff UTF16ToUTF8(
    uint8* dest,
    const utf16* src,
    MemorySize size,
    unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UCS-4 encoded string to UCS-2 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). UCS-2 is deprecated.

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags.

    @return The number of characters in the UCS-2 encoded string. Negative error code on error.
  */
  static MemoryDiff UCS4ToUCS2(ucs2* dest, const ucs4* src, MemorySize size, unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UCS-2 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). The UCS-4 characters are restricted to values in the range
    0x00000000-0x0010ffff. UCS-2 is deprecated.

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-2 encoded string.
    @param size The number of characters in the UCS-2 encoded string.
    @param flags The encoding flags.

    @return The number of characters in the UCS-4 encoded string. Negative error code on error.
  */
  static MemoryDiff UCS2ToUCS4(ucs4* dest, const ucs2* src, MemorySize size, unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).
    
    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static MemoryDiff UCS4ToUTF8(
    uint8* dest,
    const ucs4* src,
    MemorySize size,
    unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UTF-8 encoded string to UTF-16 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size).

    @param dest The destination buffer (may be nullptr).
    @param src The UTF-8 encoded string.
    @param size The number of bytes in the UTF-8 encoded string.
    @param flags The encoding flags. The default is EAT_BOM.

    @return The number of words (not characters) in the UTF-16 encoded string.
  */
  static MemoryDiff UTF8ToUTF16(
    utf16* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM) noexcept;

  /**
    Low-level method which converts an UTF-8 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size).

    @param dest The destination buffer (may be nullptr).
    @param src The UTF-8 encoded string.
    @param size The number of bytes in the UTF-8 encoded string.
    @param flags The encoding flags. The default is EAT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemoryDiff UTF8ToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM) noexcept;

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
  // TAG: add flag SKIP_INVALID = 64

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-16BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16BE)).

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16BE encoded string. Negative error code on error.
  */
  static MemoryDiff UCS4ToUTF16BE(
    uint8* dest,
    const ucs4* src,
    MemorySize size,
    unsigned int flags = ADD_BOM) noexcept;

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-16LE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF16LE)).

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-16LE encoded string. Negative error code on error.
  */
  static MemoryDiff UCS4ToUTF16LE(
    uint8* dest,
    const ucs4* src,
    MemorySize size,
    unsigned int flags = ADD_BOM) noexcept;

  /**
    Low-level method which converts an UTF-16 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). The UCS-4 characters are restricted to values in the range
    0x00000000-0x0010ffff.

    @param dest The destination buffer (may be nullptr).
    @param src The UTF-16 encoded string.
    @param size The number of bytes in the UTF-16 encoded string.
    @param flags The encoding flags. The default is EAT_BOM|EXPECT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemoryDiff UTF16ToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM) noexcept;

  /** Convert in-memory (no BOM) UTF-16 to UCS-4. */
  static MemoryDiff UTF16ToUCS4(ucs4* dest, const utf16* src, MemorySize size, unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UCS-4 encoded string to UTF-32BE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF32BE)).
    
    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-32BE encoded string. Negative error code on error.
  */
  static MemoryDiff UCS4ToUTF32BE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags = ADD_BOM) noexcept;
  
  /**
    Low-level method which converts an UCS-4 encoded string to UTF-32LE. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF32LE)).

    @param dest The destination buffer (may be nullptr).
    @param src The UCS-4 encoded string.
    @param size The number of characters in the UCS-4 encoded string.
    @param flags The encoding flags. The default is ADD_BOM.

    @return The number of bytes occupied by the UTF-32LE encoded string. Negative error code on error.
  */
  static MemoryDiff UCS4ToUTF32LE(uint8* dest, const ucs4* src, MemorySize size, unsigned int flags = ADD_BOM) noexcept;

  /**
    Low-level method which converts an UTF-32 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). See the technical report available at
    http://www.unicode.org/unicode/reports/tr19. The UCS-4 characters are
    restricted to values in the range 0x00000000-0x0010ffff.

    @param dest The destination buffer (may be nullptr).
    @param src The UTF-32 encoded string.
    @param size The number of bytes in the UTF-32 encoded string.
    @param flags The encoding flags. The default is EAT_BOM|EXPECT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemorySize UTF32ToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM | 0*EXPECT_BOM) /*throw(MultibyteException)*/;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
