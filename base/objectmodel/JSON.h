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

/** JSON exception. */
class _COM_AZURE_DEV__BASE__API JSONException : public Exception {
public:
  
  inline JSONException() : Exception() {
  }

  inline JSONException(const char* message) : Exception(message) {
  }
};

/**
  JSON parser. See https://www.json.org/.

  @short JSON
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API JSON : public DynamicObject {
private:

  ObjectModel objectModel;
  std::string text; // reused
  Posix posix; // get series of floats

  /** Skip space. */
  inline void skipSpaces(Parser& parser) noexcept {
    parser.skipJSONSpaces();
    // parser.skipAny(" \n\r\t");
  }

  /** Parses integer. */
  bool parseIntegerImpl(Parser& parser, int& i);
public:

  /** Constructs JSON parser. */
  JSON();
  
  /** Returns void/null from input. */
  Reference<ObjectModel::Void> parseNull(Parser& parser);

  /** Returns boolean from input. */
  Reference<ObjectModel::Boolean> parseBoolean(Parser& parser);

  /** Returns integer from input. */
  Reference<ObjectModel::Integer> parseInteger(Parser& parser);

  /** Returns float from input. */
  Reference<ObjectModel::Float> parseFloat(Parser& parser);

  /** Returns number from input. */
  Reference<ObjectModel::Value> parseNumber(Parser& parser);

  /** Returns string from input. */
  Reference<ObjectModel::String> parseString(Parser& parser);

  /** Returns array from input. */
  Reference<ObjectModel::Array> parseArray(Parser& parser);

  /** Returns object from input. */
  Reference<ObjectModel::Object> parseObject(Parser& parser);

  /** Returns any value from input. */
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
