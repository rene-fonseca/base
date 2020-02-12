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

#include <base/mem/NullPointer.h>
#include <base/string/String.h>
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Parse exception. */
class _COM_AZURE_DEV__BASE__API ParseException : public Exception {
public:
  
  ParseException() noexcept;
  
  ParseException(const char* message) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/** Parser. */
class _COM_AZURE_DEV__BASE__API Parser {
protected:
  
  const uint8* src = nullptr;
  const uint8* end = nullptr;
public:
  
  /** Constructs parser. */
  Parser() noexcept
  {
  }

  /** Constructs parser. */
  Parser(const uint8* _src, const uint8* _end) noexcept
    : src(_src), end(_end)
  {
    BASSERT(src <= end);
  }
  
  /** Constructs parser. */
  Parser(const String& text) noexcept
    : src(reinterpret_cast<const uint8*>(text.native())), end(reinterpret_cast<const uint8*>(text.getEnd()))
  {
    BASSERT(src <= end);
  }

  /** Returns the current marker. */
  inline const uint8* getCurrent() const noexcept
  {
    return src;
  }

  /** Returns the available bytes. */
  inline MemorySize getAvailable() const noexcept
  {
    return end - src;
  }

  /** Returns true if more chars available. */
  inline bool hasMore() const noexcept
  {
    return src != end;
  }

  /** Returns the next byte without reading it. */
  inline uint8 peekByte() const
  {
    if (src == end) {
      _throw ParseException("Unexpected end reached.");
    }
    return *src;
  }

  /** Returns the next char without reading it. */
  inline char peek() const
  {
    if (src == end) {
      _throw ParseException("Unexpected end reached.");
    }
    return *src;
  }

  /** Returns true if the next char is as expected without reading it. */
  inline bool peek(char ch) const
  {
    return peek() == ch;
  }

  /** Rewinds 1 byte. */
  inline void unwind()
  {
#if 0
    if (src == begin) {
      _throw ParseException("Unexpected beginning reached.");
    }
#endif
    --src;
  }

  /** Skip next char. */
  inline void skip()
  {
    if (src == end) {
      _throw ParseException("Unexpected end reached.");
    }
    // char result = *src;
    ++src;
  }

  /** Returns the number of bytes in the next UTF-8 encoded char. Returns 0 for invalid encoding. */
  unsigned int getUCS4Bytes() const noexcept;

  /** Peeks the next UCS4 character. */
  ucs4 peekUCS4() const;

  /** Returns the next UCS4 char assuming UTF8 encoding. */
  ucs4 readUCS4();

  /** Reads the given char. */
  inline void read(ucs4 ch)
  {
    if (readUCS4() != ch) {
      _throw ParseException("Unexpected char.");
    }
  }
  
  /** Skips the next UTF-8 encoded character. */
  inline void skipUCS4()
  {
    readUCS4();
  }
  
  /** Reads a digit 0-9. */
  inline unsigned int readDigit()
  {
    ucs4 ch = readUCS4();
    if ((ch >= '0') && (ch <= '9')) {
      return ch - '0';
    }
    _throw ParseException("Unexpected char.");
  }

  /** Reads n digits (up to 9). */
  inline unsigned int readDigits(unsigned int count)
  {
    BASSERT(count <= 9);
    unsigned int result = 0;
    while (count--) {
      uint8 value = readDigit();
      result = result * 10 + value;
    }
    return result;
  }

  /** Returns the number of available digits. */
  inline unsigned int getNumberOfDigits() const noexcept
  {
    const uint8* src = this->src;
    while (src != end) {
      uint8 value = *src; // fine without ucs4
      if (!((value >= '0') && (value <= '9'))) {
        break;
      }
      ++src;
    }
    return static_cast<unsigned int>(src - this->src);
  }

  /** Returns the next char. */
  inline char read()
  {
    if (src == end) {
      _throw ParseException("Unexpected end reached.");
    }
    const char result = *src;
    ++src;
    return result;
  }

  /** Reads the given char. */
  inline void read(char ch)
  {
    if (read() != ch) {
      _throw ParseException("Unexpected char.");
    }
  }

  /** Reads the given text. */
  inline void read(const char* text)
  {
    if (!text) {
      _throw NullPointer();
    }
    while (*text) {
      read(*text++);
    }
  }
  
  /** Skips JSON space. */
  inline void skipJSONSpaces()
  {
    while (src != end) {
      switch (peek()) {
      case ' ':
      case '\n':
      case '\r':
      case '\t':
        break;
      default:
        return;
      }
      ++src;
    }
  }
  
  /** Skip any of the given chars until no more. */
  inline void skipAny(const char* chars)
  {
    while (src != end) {
      const char ch = peek();
      bool skip = false;
      for (const char* s = chars; *s; ++s) {
        if (ch == *s) {
          skip = true;
          break;
        }
      }
      if (!skip) {
        return;
      }
      ++src;
    }
  }

  /** Returns true if the string matches the given pattern (using * and ?). */
  static bool doesMatchPattern(const String& pattern, const String& text) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
