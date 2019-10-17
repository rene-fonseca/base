/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/objectmodel/JSON.h>
#include <base/io/File.h>
#include <base/string/Posix.h>
#include <locale>
#include <codecvt>

// TAG: handle chars in JSON strings!

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const LineColumn& position)
{
  return stream << position.line << ":" << position.column;
}

JSON::JSON()
{
}

Reference<ObjectModel::Void> JSON::parseNull(JSONParser& parser)
{
  skipSpaces(parser);
  parser.read("null");
  return objectModel.createVoid();
}

Reference<ObjectModel::Boolean> JSON::parseBoolean(JSONParser& parser)
{
  skipSpaces(parser);
  switch (parser.peek()) {
  case 'f':
    parser.read("false");
    return objectModel.createBoolean(false);
  case 't':
    parser.read("true");
    return objectModel.createBoolean(true);
  }
  throw JSONException("Expected boolean.", parser.getPosition());
}

bool JSON::parseIntegerImpl(JSONParser& parser, int& j)
{
  skipSpaces(parser);
  int i = 0;
  int sign = 1;
  if (parser.peek() == '-') {
    parser.read();
    sign = -1;
  }

  // TAG: add support info on position of failure for exceptions - line and column

  if (parser.peek() == '0') {
    parser.skip(); // done
  } else {
    if (!((parser.peek() >= '1') && (parser.peek() <= '9'))) {
      return false;
    }
    const char ch = parser.read();
    i = static_cast<uint8>(ch - '0'); // first digit
    while ((parser.peek() >= '0') && (parser.peek() <= '9')) {
      const char ch = parser.read();
      uint8 digit = static_cast<uint8>(ch - '0');
      i = i * 10 + digit;
      if (i * sign < PrimitiveTraits<int>::MINIMUM) {
        return false;
      } else if (i * sign > PrimitiveTraits<int>::MAXIMUM) {
        return false;
      }
    }
  }

  switch (parser.peek()) {
  case '.':
  case 'e':
  case 'E':
    return false; // float chars
  }

  j = static_cast<int>(i * sign);
  return true;
}

Reference<ObjectModel::Integer> JSON::parseInteger(JSONParser& parser)
{
  skipSpaces(parser);
  int i = 0;
  if (!parseIntegerImpl(parser, i)) {
    throw JSONException("Expected integer.", parser.getPosition());
  }
  return objectModel.createInteger(i);
}

Reference<ObjectModel::Float> JSON::parseFloat(JSONParser& parser)
{
  skipSpaces(parser);

  const Parser p = parser;

  if (parser.peek() == '-') {
    parser.skip();
  }

  if (parser.peek() == '0') {
    parser.skip();
  } else {
    if ((parser.peek() >= '1') && (parser.peek() <= '9')) {
      parser.skip();
    }
    while ((parser.peek() >= '0') && (parser.peek() <= '9')) {
      parser.skip();
    }
  }
  
  // fraction
  if (parser.peek() == '.') {
    parser.skip();
    // skip digits
    while ((parser.peek() >= '0') && (parser.peek() <= '9')) {
      parser.skip();
    }
  }
  
  // exponent
  if ((parser.peek() == 'E') || (parser.peek() == 'e')) {
    parser.skip();
    if ((parser.peek() == '-') || (parser.peek() == '+')) {
      parser.skip();
    }
    while ((parser.peek() >= '0') && (parser.peek() <= '9')) {
      parser.skip();
    }
  }

  const char* b = reinterpret_cast<const char*>(p.getCurrent());
  const char* e = reinterpret_cast<const char*>(parser.getCurrent());

  double d = 0;
  if (!posix.getSeries(b, e, d)) {
    throw JSONException("Malformed float.", parser.getPosition());
  }
  
  return objectModel.createFloat(d);
}

Reference<ObjectModel::Value> JSON::parseNumber(JSONParser& parser)
{
  skipSpaces(parser);
  int i = 0;
  JSONParser integerParser = parser;
  if (parseIntegerImpl(integerParser, i)) {
    parser = integerParser;
    return objectModel.createInteger(i);
  }
  return parseFloat(parser);
}

Reference<ObjectModel::String> JSON::parseString(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('"')) {
    throw JSONException("Expected string.", parser.getPosition());
  }
  text.clear();
  text.reserve(1024);
  parser.skip();
  while (parser.peek() != '"') {
    const char ch = parser.read();
    if (ch == '\\') { // escape
      const char ch2 = parser.read();
      switch (ch2) {
      case '"':
        text.push_back('"');
        break;
      case '\\':
        text.push_back('\\');
        break;
      case '/':
        text.push_back('/');
        break;
      case 'b':
        text.push_back('\b');
        break;
      case 'f':
        text.push_back('\f');
        break;
      case 'n':
        text.push_back('\n');
        break;
      case 'r':
        text.push_back('\r');
        break;
      case 't':
        text.push_back('\t');
        break;
      case 'u':
        { // TAG: make sure a-f and A-F works
          const char h0 = parser.read();
          const char h1 = parser.read();
          const char h2 = parser.read();
          const char h3 = parser.read();
          if (ASCIITraits::isHexDigit(h0) || ASCIITraits::isHexDigit(h1) ||
              ASCIITraits::isHexDigit(h2) || ASCIITraits::isHexDigit(h3)) {
            const uint16 d0 = ASCIITraits::digitToValue(h0);
            const uint16 d1 = ASCIITraits::digitToValue(h1);
            const uint16 d2 = ASCIITraits::digitToValue(h2);
            const uint16 d3 = ASCIITraits::digitToValue(h3);
            const uint16 value = (d0 << 12) | (d1 << 8) | (d2 << 4) | (d3 << 0);
            const wchar* src = reinterpret_cast<const wchar*>(&value);
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
            std::string s = convert.to_bytes(src, src + 1);
            text += s;
          } else {
            throw JSONException("Malformed string.", parser.getPosition());
          }
        }
        break;
      default:
        throw JSONException("Malformed string.", parser.getPosition());
      }
    } else {
      text.push_back(ch);
    }
  }
  parser.read('"');
  return objectModel.createString(String(text));
}

Reference<ObjectModel::Array> JSON::parseArray(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('[')) {
    throw JSONException("Expected array.", parser.getPosition());
  }
  parser.skip();
  Reference<ObjectModel::Array> result = objectModel.createArray();
  skipSpaces(parser);
  if (parser.peek() == ']') {
    parser.skip();
    return result;
  }
  while (true) {
    skipSpaces(parser);
    result->append(parseValue(parser));
    skipSpaces(parser);
    switch (parser.peek()) {
    case ',':
      parser.skip();
      break;
    case ']':
      parser.skip();
      return result;
    default:
      throw JSONException("Malformed array.", parser.getPosition());
    }
  }
  return nullptr;
}

Reference<ObjectModel::Object> JSON::parseObject(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('{')) {
    throw JSONException("Expected object.", parser.getPosition());
  }
  parser.skip();
  Reference<ObjectModel::Object> result = objectModel.createObject();
  skipSpaces(parser);
  if (parser.peek() == '}') {
    parser.skip();
    return result;
  }
  while (true) {
    skipSpaces(parser);
    Reference<ObjectModel::String> key = parseString(parser);
    skipSpaces(parser);
    parser.read(':');
    skipSpaces(parser);
    Reference<ObjectModel::Value> value = parseValue(parser);
    result->setValue(key, value);
    skipSpaces(parser);
    switch (parser.peek()) {
    case ',':
      parser.skip();
      break;
    case '}':
      parser.skip();
      return result;
    default:
      throw JSONException("Malformed object.", parser.getPosition());
    }
  }
  return nullptr;
}

Reference<ObjectModel::Value> JSON::parseValue(JSONParser& parser)
{
  skipSpaces(parser);
  switch (parser.peek()) {
  case 't':
  case 'f':
    return parseBoolean(parser);
  case 'n':
    return parseNull(parser);
  case '"':
    return parseString(parser);
  case '{':
    return parseObject(parser);
  case '[':
    return parseArray(parser);
  default:
    return parseNumber(parser);
  }
}

Reference<ObjectModel::Value> JSON::parse(const uint8* src, const uint8* end)
{
  JSONParser parser(src, end);
  auto result = parseObject(parser);
  skipSpaces(parser);
  if (parser.hasMore()) {
    throw JSONException("Unexpected content after object.", parser.getPosition());
  }
  return result;
}

Reference<ObjectModel::Value> JSON::parse(const String& text)
{
  return parse(
    reinterpret_cast<const uint8*>(text.getElements()),
    reinterpret_cast<const uint8*>(text.getElements()) + text.getLength()
  );
}

Reference<ObjectModel::Value> JSON::parseFile(const String& path)
{
  // TAG: need support for streaming
  PrimitiveArray<uint8> buffer(0);
  {
    File file(path, File::READ, 0);
    buffer.resize(file.getSize());
    file.read(buffer, buffer.size(), false);
  }
  JSON json;
  return json.parse(buffer, buffer + buffer.size());
}

String JSON::getJSON(Reference<ObjectModel::Value> value, bool niceFormat)
{
  if (!value) {
    throw NullPointer();
  }
  return value->toString(niceFormat);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
