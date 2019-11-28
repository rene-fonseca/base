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

#include <base/objectmodel/JSON.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** YAML exception. */
class _COM_AZURE_DEV__BASE__API YAMLException : public Exception {
private:

  LineColumn position;
public:
  
  inline YAMLException() : Exception() {
  }

  inline YAMLException(const char* message) : Exception(message) {
  }

  inline YAMLException(const char* message, const LineColumn& _position)
    : Exception(message), position(_position) {
  }

  inline const LineColumn& getPosition() const noexcept {
    return position;
  }

  inline void setPosition(const LineColumn& _position) noexcept {
    position = _position;
  }

  inline bool isCommonException() const noexcept override
  {
    return true;
  }

  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/**
  YAML parser. See https://yaml.org/refcard.html and https://en.wikipedia.org/wiki/YAML.

  @short YAML
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API YAML : public DynamicObject {
private:

  class _COM_AZURE_DEV__BASE__API YAMLParser : public Parser {
  private:
    
    unsigned int line = 0;
    const uint8* lastLine = nullptr;
    unsigned int tabSize = 2;
  public:

    // TAG: handle UTF8/non ASCII?
    
    YAMLParser() noexcept
    {
    }

    YAMLParser(const uint8* src, const uint8* end) noexcept
      : Parser(src, end)
    {
      lastLine = src;
    }
    
    /** Finds the column. */
    static MemorySize getColumn(const uint8* src, const uint8* end, unsigned int tabSize = 2)
    {
      MemorySize column = 0;
      while (src != end) {
        switch (*src) {
        case '\t':
          column += tabSize;
          break;
        case '\n':
          column = 0;
          break;
        // case '\r':
        default:
          ++column;
        }
        ++src;
      }
      return column;
    }

    // TAG: only return line and offset within line - and let caller handle tab size
    
    inline LineColumn getPosition() const noexcept
    {
      MemorySize column = 0;
      if (lastLine) {
        column = getColumn(lastLine, src, tabSize);
      }
      return LineColumn(line + 1, column + 1);
    }
    
    /** Skips YAML space. */
    inline void skipYAMLSpaces() noexcept
    {
      while (src != end) {
        switch (peek()) {
        case ' ':
          break;
        case '\n':
          ++line;
          break;
        case '\r':
          break;
        case '\t':
          break;
        default:
          return;
        }
        ++src;
      }
    }
  };
  
  ObjectModel objectModel;
  std::string text; // reused
  Posix posix; // get series of floats

  /** Skip space. */
  inline void skipSpaces(YAMLParser& parser) noexcept {
    parser.skipYAMLSpaces();
    // parser.skipAny(" \n\r\t");
  }

  /** Parses integer. */
  bool parseIntegerImpl(YAMLParser& parser, int& i);
public:

  /** Constructs YAML parser. */
  YAML();
  
  /** Returns void/null from input. */
  Reference<ObjectModel::Void> parseNull(YAMLParser& parser);

  /** Returns boolean from input. */
  Reference<ObjectModel::Boolean> parseBoolean(YAMLParser& parser);

  /** Returns integer from input. */
  Reference<ObjectModel::Integer> parseInteger(YAMLParser& parser);

  /** Returns float from input. */
  Reference<ObjectModel::Float> parseFloat(YAMLParser& parser);

  /** Returns number from input. */
  Reference<ObjectModel::Value> parseNumber(YAMLParser& parser);

  /** Returns string from input. */
  Reference<ObjectModel::String> parseString(YAMLParser& parser);

  /** Returns array from input. */
  Reference<ObjectModel::Array> parseArray(YAMLParser& parser);

  /** Returns object from input. */
  Reference<ObjectModel::Object> parseObject(YAMLParser& parser);

  /** Returns any value from input. */
  Reference<ObjectModel::Value> parseValue(YAMLParser& parser);

  /** Returns ObjectModel for the given YAML text. */
  Reference<ObjectModel::Value> parse(const uint8* src, const uint8* end);

  /** Returns ObjectModel for the given YAML text. */
  Reference<ObjectModel::Value> parse(const String& text);

  static Reference<ObjectModel::Value> parseFile(const String& path);
  
  /** Converts given ObjectModel to YAML. */
  static String getYAML(Reference<ObjectModel::Value> value);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

