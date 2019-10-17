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
#include <base/Primitives.h>
#include <base/Object.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Parse exception. */
class _COM_AZURE_DEV__BASE__API ParseException : public Exception {
public:
  
  inline ParseException() {
  }
  
  inline ParseException(const char* message) : Exception(message) {
  }
};

/** Parser. */
class _COM_AZURE_DEV__BASE__API Parser /*: public Object*/ {
protected:
  
  // TAG: add support for skipping UTF-8 chars
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
    ASSERT(src <= end);
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
      throw ParseException("Unexpected end reached.");
    }
    return *src;
  }

  /** Returns the next char without reading it. */
  inline char peek() const
  {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    return *src;
  }

  /** Returns true if the next char is as expected without reading it. */
  inline bool peek(char ch) const noexcept
  {
    return peek() == ch;
  }

  /** Rewinds 1 byte. */
  inline void unwind()
  {
#if 0
    if (src == begin) {
      throw ParseException("Unexpected beginning reached.");
    }
#endif
    --src;
  }

  /** Skip next char. */
  inline void skip()
  {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    char result = *src;
    ++src;
  }

  /** Returns the number of bytes in the next UTF-8 encoded char. Returns 0 for invalid encoding. */
  unsigned int getUCS4Bytes() const noexcept;

  /** Peeks the next UCS4 character. */
  ucs4 peekUCS4() const;

  /** Returns the next UCS4 char assuming UTF8 encoding. */
  ucs4 readUCS4();

  /** Returns the next char. */
  inline char read()
  {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    const char result = *src;
    ++src;
    return result;
  }

  /** Reads the given char. */
  inline void read(char ch)
  {
    if (read() != ch) {
      throw ParseException("Unexpected char.");
    }
  }

  /** Reads the given text. */
  inline void read(const char* text)
  {
    if (!text) {
      throw NullPointer();
    }
    while (*text) {
      read(*text++);
    }
  }
  
  /** Skips JSON space. */
  inline void skipJSONSpaces() noexcept
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
  inline void skipAny(const char* chars) noexcept
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
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
