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

#include <base/objectmodel/ObjectModel.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Parse exception. */
class ParseException : public Exception {
public:
  
  inline ParseException() {
  }
  
  inline ParseException(const char* message) : Exception(message) {
  }
};

/** Parser. */
class Parser {
private:
  
  const uint8* src = nullptr;
  const uint8* end = nullptr;
public:
  
  Parser() noexcept {
  }

  Parser(const uint8* _src, const uint8* _end) noexcept : src(_src), end(_end) {
    ASSERT(src <= end);
  }

  /** Returns true if more chars available. */
  inline bool hasMore() const noexcept {
    return src != end;
  }
  
  /** Returns the next char without reading it. */
  inline char peek() const {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    return *src;
  }

  /** Returns the next char without reading it. */
  inline bool peek(char ch) const noexcept {
    return peek() == ch;
  }

  /** Skip next char. */
  inline void skip() {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    char result = *src;
    ++src;
  }

  /** Returns the next char. */
  inline void read(char ch) {
    if (src == end) {
      throw ParseException("Unexpected end reached.");
    }
    char result = *src;
    ++src;
  }
  
  /** Reads the given text. */
  void read(const char* text) {
    if (!text) {
      throw NullPointer();
    }
    while (*text) {
      const char ch = *text++;
      read(ch);
    }
  }
  
  /** Skip any of the given chars until no more. */
  inline void skipAny(const char* chars) noexcept {
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

class _COM_AZURE_DEV__BASE__API JSONException : public Exception {
public:
  
  inline JSONException() : Exception() {
  }

  inline JSONException(const char* message) : Exception(message) {
  }
};

/**
  JSON parser.

  @short JSON
  @see SpinLock
  @ingroup concurrency
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API JSON : public DynamicObject {
private:

  ObjectModel objectModel;
  
  inline void skipSpaces(Parser& parser) noexcept {
    parser.skipAny(" \t");
  }
public:

  JSON();
  
  Reference<ObjectModel::Void> parseNull(Parser& parser);
  Reference<ObjectModel::Boolean> parseBoolean(Parser& parser);
  Reference<ObjectModel::Integer> parseInteger(Parser& parser);
  Reference<ObjectModel::Float> parseFloat(Parser& parser);
  Reference<ObjectModel::Value> parseNumber(Parser& parser);
  Reference<ObjectModel::String> parseString(Parser& parser);
  Reference<ObjectModel::Array> parseArray(Parser& parser);
  Reference<ObjectModel::Object> parseObject(Parser& parser);
  Reference<ObjectModel::Value> parseValue(Parser& parser);

  /** Returns ObjectModel for the given JSON text. */
  Reference<ObjectModel::Value> parse(const uint8* src, const uint8* end);

  /** Returns ObjectModel for the given JSON text. */
  Reference<ObjectModel::Value> parse(const String& text);

  static Reference<ObjectModel::Value> parseFile(const String& path);
  
  /** Converts given ObjectModel to JSON. */
  static String getJSON(Reference<ObjectModel::Value> value, bool niceFormat = true);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
