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

class _COM_AZURE_DEV__BASE__API LineColumn {
public:
  
  MemorySize line = 0;
  MemorySize column = 0;
  
  void nextLine() noexcept {
    ++line;
    column = 0;
  }

  void nextColumn() noexcept {
    ++column;
  }
};

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const LineColumn& value);

/** JSON exception. */
class _COM_AZURE_DEV__BASE__API JSONException : public Exception {
private:

  LineColumn position;
public:
  
  inline JSONException() : Exception() {
  }

  inline JSONException(const char* message) : Exception(message) {
  }

  inline JSONException(const char* message, const LineColumn& _position)
    : Exception(message), position(_position) {
  }

  inline const LineColumn& getPosition() const noexcept {
    return position;
  }

  inline void setPosition(const LineColumn& _position) noexcept {
    position = _position;
  }
};

/**
  JSON parser. See https://www.json.org/.

  @short JSON
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API JSON : public DynamicObject {
private:

  class JSONParser : public Parser {
  private:
    
    LineColumn position;
  public:

    JSONParser() noexcept
    {
    }

    JSONParser(const uint8* src, const uint8* end) noexcept
      : Parser(src, end)
    {
    }
    
    inline const LineColumn& getPosition() const noexcept {
      return position;
    }
    
    /** Skips JSON space. */
    inline void skipJSONSpaces() noexcept
    {
      while (src != end) {
        switch (peek()) {
        case ' ':
        case '\n':
          position.nextLine();
          break;
        case '\r':
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
  inline void skipSpaces(JSONParser& parser) noexcept {
    parser.skipJSONSpaces();
    // parser.skipAny(" \n\r\t");
  }

  /** Parses integer. */
  bool parseIntegerImpl(JSONParser& parser, int& i);
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
  Reference<ObjectModel::Value> parse(const String& text);

  static Reference<ObjectModel::Value> parseFile(const String& path);
  
  /** Converts given ObjectModel to JSON. */
  static String getJSON(Reference<ObjectModel::Value> value, bool niceFormat = true);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
