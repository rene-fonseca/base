/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Parser.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
MemorySize WideString::UTF8ToUCS4(ucs4* dest, const uint8* src, MemorySize size, unsigned int flags) throw(MultibyteException)
{
  const uint8* const srcBegin = src;
  const ucs4* const begin = dest;
  const uint8* const end = src + size;

    while (src < end) {
      uint8 value = *src++;
      if ((value & 0x80) == 0x00) {
        *dest++ = value;
      }
      else if ((value & 0xe0) == 0xc0) {
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
      }
      else if ((value & 0xf0) == 0xe0) {
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
      }
      else if ((value & 0xf8) == 0xf0) {
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
      }
      else if ((value & 0xfc) == 0xf8) {
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
      }
      else if ((value & 0xfe) == 0xfc) {
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
      }
      else {
        throw MultibyteException();
      }
    }
    return dest - begin;
}
#endif

// TAG: put in Unicode class

enum {
  UTF8_ERROR_EMPTY = 0,
  UTF8_ERROR_INCOMPLETE = -1,
  UTF8_ERROR_BAD_ENCODING = -2
};

/**
  Converts the UTF-8 bytes into UCS4 character.

  @param src The start of the buffer.
  @param end The end of the buffer.
  @param ch The result character. Set to 0xffffffff on error.

  @return Bytes read. If negative this is the error code.
*/
inline int readUCS4(const uint8* src, const uint8* end, ucs4& ch)
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
    code = (code << 6) | temp;
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
      code = (code << 6) | temp;
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
      code = (code << 6) | temp;
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
      code = (code << 6) | temp;
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
      code = (code << 6) | temp;
    }
    ch = code;
    return 6;
  }
  return UTF8_ERROR_BAD_ENCODING;
}

unsigned int Parser::getUCS4Bytes() const noexcept
{
  ucs4 ch = 0;
  const int status = base::readUCS4(src, end, ch);
  return (status >= 0) ? status : 0;
}

ucs4 Parser::peekUCS4() const
{
  ucs4 ch = 0;
  const int status = base::readUCS4(src, end, ch);
  if (status <= 0) {
    switch (status) {
    case UTF8_ERROR_EMPTY:
      throw ParseException("Unexpected end reached.");
    case UTF8_ERROR_INCOMPLETE:
      throw ParseException("Incomplete UTF-8 character encoding.");
    case UTF8_ERROR_BAD_ENCODING:
    default:
      throw ParseException("Bad UTF-8 character encoding.");
    }
  }
  return ch;
}

ucs4 Parser::readUCS4() {
  ucs4 ch = 0;
  const int status = base::readUCS4(src, end, ch);
  if (status <= 0) {
    switch (status) {
    case UTF8_ERROR_EMPTY:
      throw ParseException("Unexpected end reached.");
    case UTF8_ERROR_INCOMPLETE:
      throw ParseException("Incomplete UTF-8 character encoding.");
    case UTF8_ERROR_BAD_ENCODING:
    default:
      throw ParseException("Bad UTF-8 character encoding.");
    }
  }
  src += status;
  return ch;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
