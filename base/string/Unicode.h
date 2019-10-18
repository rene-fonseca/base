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

  /** Returns true if the code is a valid UCS4 code. */
  static inline bool isUCS4(unsigned int value) throw() {
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
  static inline int readUCS4(const uint8* src, const uint8* end, ucs4& ch)
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

  // TAG: add method for converting ucs4 to bytes
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
