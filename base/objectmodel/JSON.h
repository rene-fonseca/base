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
#include <base/string/Parser.h>
#include <base/string/Posix.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Line and column position with text file. Line and column are 1 based. */
class _COM_AZURE_DEV__BASE__API LineColumn {
public:
  
  MemorySize line = 0;
  MemorySize column = 0;
  
  inline LineColumn() noexcept
  {
  }

  inline LineColumn(MemorySize _line, MemorySize _column) noexcept
    : line(_line), column(_column)
  {
  }
  
  inline bool isProper() const noexcept
  {
    return line > 0;
  }

  inline void nextLine() noexcept
  {
    ++line;
    column = 0;
  }

  inline void nextColumn() noexcept
  {
    ++column;
  }
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const LineColumn& value);

/** JSON exception. */
class _COM_AZURE_DEV__BASE__API JSONException : public Exception {
private:

  LineColumn position;
public:
  
  inline JSONException()
  {
  }

  inline JSONException(const char* message)
    : Exception(message) {
  }

  inline JSONException(const char* message, const LineColumn& _position)
    : Exception(message), position(_position)
  {
  }

  inline const LineColumn& getPosition() const noexcept
  {
    return position;
  }

  inline void setPosition(const LineColumn& _position) noexcept
  {
    position = _position;
  }
  
  inline bool isCommonException() const noexcept override
  {
    return true;
  }

  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/**
  JSON parser. See https://www.json.org/ and https://www.ietf.org/rfc/rfc7159.txt.

  Implementation preserves 64-bit signed integer and float types.

  @short JSON
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API JSON : public DynamicObject {
public:

  class _COM_AZURE_DEV__BASE__API JSONParser : public Parser {
  private:
    
    unsigned int line = 0;
    const uint8* lastLine = nullptr;
    unsigned int tabSize = 2;
  public:
    
    JSONParser() noexcept
    {
    }

    JSONParser(const uint8* src, const uint8* end) noexcept
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
    
    /** Skips JSON space. */
    inline void skipJSONSpaces() noexcept
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
private:
  
  ObjectModel objectModel;
  PrimitiveArray<char> buffer; // reused - do NOT reuse on recursion
  Posix posix; // get series of floats

  /** Skip space. */
  inline void skipSpaces(JSONParser& parser) noexcept
  {
    parser.skipJSONSpaces();
    // parser.skipAny(" \n\r\t");
  }

  /** Parses integer. */
  bool parseIntegerImpl(JSONParser& parser, int64& i);
public:

  /** Constructs JSON parser. */
  JSON();
  
  /** Returns void/null from input. */
  Reference<ObjectModel::Void> parseNull(JSONParser& parser);

  /** Returns boolean from input. */
  Reference<ObjectModel::Boolean> parseBoolean(JSONParser& parser);

  /** Returns integer from input. */
  Reference<ObjectModel::Integer> parseInteger(JSONParser& parser);

  /** Returns float from input. */
  Reference<ObjectModel::Float> parseFloat(JSONParser& parser);

  /** Returns number from input. */
  Reference<ObjectModel::Value> parseNumber(JSONParser& parser);

  /** Returns string from input. */
  Reference<ObjectModel::String> parseString(JSONParser& parser);

  /** Returns array from input. */
  Reference<ObjectModel::Array> parseArray(JSONParser& parser);

  /** Returns object from input. */
  Reference<ObjectModel::Object> parseObject(JSONParser& parser);

  /** Returns any value from input. */
  Reference<ObjectModel::Value> parseValue(JSONParser& parser);

  /** Returns ObjectModel for the given JSON text. */
  Reference<ObjectModel::Value> parse(const uint8* src, const uint8* end);

  /** Returns ObjectModel for the given JSON text. */
  static Reference<ObjectModel::Value> parse(const String& text);

  enum Encoding {
    UTF_8,
    UTF_16,
    UTF_32
  };
  
  /** Returns ObjectModel for the given file. */
  static Reference<ObjectModel::Value> parseFile(const String& path);
  
  /** Converts given ObjectModel to JSON. Use getJSONNoFormatting() to ensure no formatting. */
  static String getJSON(Reference<ObjectModel::Value> value, unsigned int flags = ObjectModel::DEFAULT_FORMATTING);

  /** Converts given ObjectModel to JSON. */
  static inline String getJSONNoFormatting(Reference<ObjectModel::Value> value) {
    return getJSON(value, 0);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
