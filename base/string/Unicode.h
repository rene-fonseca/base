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
#include <base/mem/Allocator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class String;
class WideString;

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
    ERROR_EMPTY = 0, /** No codes to read. */
    ERROR_INCOMPLETE = -1, /** Missing code for character. */
    ERROR_BAD_ENCODING = -2, /** Invalid encoding. */
    INVALID_UCS4_CHARACTER = -3, /** Invalid UCS2 character. */
    INVALID_UCS2_CHARACTER = -4 /** Invalid UCS2 character. */
  };

  /**
    Converts the UTF-8 bytes into UCS4 character.

    @param src The start of the buffer.
    @param end The end of the buffer.
    @param ch The result character.

    @return Bytes read. If negative this is the error code.
  */
  static int readUCS4(const uint8* src, const uint8* end, ucs4& ch) noexcept;

  /**
    Read UCS4 from null terminated UTF-8 sequence.

    @return Number of bytes read.
  */
  static int readUCS4(const uint8* src, ucs4& ch) noexcept;

  /**
    Read UCS4 from null terminated UTF-16 sequence.

    @return Number of words read (not bytes).
  */
  static int readUCS4(const utf16* src, ucs4& ch) noexcept;

  /**
    Read UCS4 from null terminated UTF-16 sequence.

    @return Number of words read (not bytes).
  */
  static inline int readUCS4(const char16_t* src, ucs4& ch) noexcept
  {
    return readUCS4(reinterpret_cast<const utf16*>(src), ch);
  }

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

  /** Returns the number of characters. */
  static MemoryDiff getUTF8StringLength(const uint8* src, MemorySize size) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const utf16* src) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const utf16* src, MemorySize size) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const char16_t* src) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const char16_t* src, MemorySize size) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const wchar* src) noexcept;

  /** Returns the number of characters. */
  static MemoryDiff getStringLength(const wchar* src, MemorySize size) noexcept;

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
    // TAG: add iso codes also
    return 0;
  }

  /**
    Converts UCS4 to UTF-8. Returns 0 if invalid UCS4 character. Buffer must have room for minimum 4 bytes for Unicode
    code.
  */
  static inline MemorySize writeUTF8(uint8* dest, ucs4 ch) noexcept
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
    // TAG: add iso codes also
    return 0;
  }

  /** Returns the number of UTF-16 words required to represent the given code. */
  static inline MemorySize getUTF16Words(ucs4 ch) noexcept
  {
    if (ch < 0x10000) {
      return 1;
    } else if (ch < MAX) {
      return 2;
    }
    return 0;
  }

  /** Writes the given code as UTF-16. Destination must ave room for minimum 2 words. */
  static inline MemorySize writeUTF16(utf16* dest, ucs4 ch) noexcept
  {
    if (ch < 0x10000) {
      *dest++ = static_cast<utf16>(ch);
      return 1;
    } else if (ch < MAX) {
      *dest++ = static_cast<utf16>(((ch - 0x10000) >> 10) + 0xd800); // 10 bit - high
      *dest++ = static_cast<utf16>(((ch - 0x10000) & 0x3ff) + 0xdc00); // 10 bit - low
      return 2;
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
    @param src The UTF16 encoded string.
    @param size The number of characters in the UTF16 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static MemoryDiff UTF16ToUTF8(
    uint8* dest,
    const utf16* src,
    MemorySize size,
    unsigned int flags = 0) noexcept;

  /**
    Low-level method which converts an UTF16 encoded string to UTF-8. A
    null-terminator is NOT appended to the string. The destination buffer must
    have room for enough bytes (guaranteed to not exceed
    (size + 1) * getMaximumNumberOfMultibytes(UTF8)).

    @param dest The destination buffer (may be nullptr).
    @param src The UTF16 encoded string.
    @param size The number of characters in the UTF16 encoded string.
    @param flags The encoding flags. The default is 0.

    @return The number of bytes occupied by the UTF-8 encoded string.
  */
  static inline MemoryDiff UTF16ToUTF8(
    uint8* dest,
    const char16_t* src,
    MemorySize size,
    unsigned int flags = 0) noexcept
  {
    return UTF16ToUTF8(dest, reinterpret_cast<const utf16*>(src), size, flags);
  }

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
  static inline MemoryDiff UTF32ToUTF8(
    uint8* dest,
    const char32_t* src,
    MemorySize size,
    unsigned int flags = 0) noexcept
  {
    return UCS4ToUTF8(dest, reinterpret_cast<const ucs4*>(src), size, flags);
  }

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
  static inline MemoryDiff UTF8ToUCS4(ucs4* dest,
                                      const char* src,
                                      MemorySize size,
                                      unsigned int flags = EAT_BOM) noexcept
  {
    return UTF8ToUCS4(dest, reinterpret_cast<const uint8*>(src), size, flags);
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
    @param flags The encoding flags. The default is EAT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemoryDiff UTF16ToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM) noexcept;

  /** Convert in-memory (no BOM) UTF-16 to UCS-4. */
  static MemoryDiff UTF16ToUCS4(ucs4* dest, const utf16* src, MemorySize size, unsigned int flags = 0) noexcept;

  /** Convert in-memory (no BOM) UTF-16 to UCS-4. */
  static inline MemoryDiff UTF16ToUCS4(ucs4* dest,
                                       const char16_t* src,
                                       MemorySize size,
                                       unsigned int flags = 0) noexcept
  {
    return UTF16ToUCS4(dest, reinterpret_cast<const utf16*>(src), size, flags);
  }

  /** Convert in-memory (no BOM) UCS-4 to UTF-16. */
  static MemoryDiff UCS4ToUTF16(utf16* dest, const ucs4* src, MemorySize size, unsigned int flags = 0) noexcept;

  /** Convert in-memory (no BOM) UCS-4 to UTF-16. */
  static inline MemoryDiff UCS4ToUTF16(char16_t* dest,
                                       const ucs4* src,
                                       MemorySize size,
                                       unsigned int flags = 0) noexcept
  {
    return UCS4ToUTF16(reinterpret_cast<utf16*>(dest), src, size, flags);
  }

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
    @param flags The encoding flags. The default is EAT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemoryDiff UTF32LEToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM);

  /**
    Low-level method which converts an UTF-32 encoded string to UCS-4 encoding.
    The destination buffer must have room for enough characters (guaranteed to
    not exceed size). See the technical report available at
    http://www.unicode.org/unicode/reports/tr19. The UCS-4 characters are
    restricted to values in the range 0x00000000-0x0010ffff.

    @param dest The destination buffer (may be nullptr).
    @param src The UTF-32 encoded string.
    @param size The number of bytes in the UTF-32 encoded string.
    @param flags The encoding flags. The default is EAT_BOM.

    @return The number of characters in the UCS-4 encoded string.
  */
  static MemoryDiff UTF32BEToUCS4(
    ucs4* dest,
    const uint8* src,
    MemorySize size,
    unsigned int flags = EAT_BOM);

  // wchar support

  /** Converts UCS-4 string to wchar string. */
  static inline MemoryDiff UCS4ToWChar(wchar* dest, const ucs4* src, MemorySize size) noexcept
  {
    if (!src) {
      return 0;
    }
    if (sizeof(wchar) == sizeof(utf16)) {
      return UCS4ToUTF16(reinterpret_cast<utf16*>(dest), src, size);
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      if (dest) {
        for (const auto end = src + size; src != end; ++dest, ++src) { // no code validation
          *dest = *src;
        }
      }
      return size;
    } else {
      BASSERT(!"Unsupported wchar.");
    }
  }

  /** Converts wchar string to UCS-4 string. */
  static inline MemoryDiff WCharToUCS4(ucs4* dest, const wchar* src, MemorySize size) noexcept
  {
    if (!src) {
      return 0;
    }
    if (sizeof(wchar) == sizeof(utf16)) {
      return UTF16ToUCS4(dest, reinterpret_cast<const utf16*>(src), size);
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      if (dest) {
        for (const auto end = src + size; src != end; ++dest, ++src) { // no code validation
          *dest = *src;
        }
      }
      return size;
    } else {
      BASSERT(!"Unsupported wchar.");
    }
  }

  /** Converts UTF-8 string to wchar string. */
  static inline MemoryDiff UTF8ToWChar(wchar* dest, const uint8* src, MemorySize size) noexcept
  {
    if (!src) {
      return 0;
    }
    if (sizeof(wchar) == sizeof(utf16)) {
      return UTF8ToUTF16(reinterpret_cast<utf16*>(dest), src, size);
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      return UTF8ToUCS4(reinterpret_cast<ucs4*>(dest), src, size);
    } else {
      BASSERT(!"Unsupported wchar.");
    }
  }

  /** Converts wchar string to UTF-8 string. */
  static inline MemoryDiff WCharToUTF8(uint8* dest, const wchar* src, MemorySize size) noexcept
  {
    if (!src) {
      return 0;
    }
    if (sizeof(wchar) == sizeof(utf16)) {
      return UTF16ToUTF8(dest, reinterpret_cast<const utf16*>(src), size);
    } else if (sizeof(wchar) == sizeof(ucs4)) {
      return UCS4ToUTF8(dest, reinterpret_cast<const ucs4*>(src), size);
    } else {
      BASSERT(!"Unsupported wchar.");
    }
  }
};

/** 8-bit charset. */
class CharSet {
public:
  
  virtual ucs4 operator()(uint8 code) const noexcept = 0;
};

/** Automation object for converting string to temporary wchar string. */
class ToWCharString {
private:

  Allocator<wchar> buffer;
  const wchar* string = nullptr;
public:

  /** Empty string. */
  ToWCharString();
  /** Assumes UTF-8. */
  ToWCharString(const char* string);
  /** Initialize self. No conversion happens. Do NOT modify string until automation object is destructed. */
  ToWCharString(const wchar* string);
  /** Initialize from UCS-4 string. */
  ToWCharString(const ucs4* string);
  /** Initialize from UTF-8 string. */
  ToWCharString(const String& string);
  /** Initialize from UCS-4 string. */
  ToWCharString(const WideString& string);

  /** Returns the length of the string. Excluding null terminator. */
  inline MemorySize getLength() const noexcept
  {
    return !buffer.isEmpty() ? (buffer.getSize() - 1) : getNullTerminatedLength(string);
  }

  /** Only valid until automation object is destructed. */
  inline const wchar* native() const noexcept
  {
    return string;
  }

  /** Only valid until automation object is destructed. */
  inline operator const wchar*() const noexcept
  {
    return string;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
