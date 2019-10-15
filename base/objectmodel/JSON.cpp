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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

JSON::JSON()
{
}

Reference<ObjectModel::Void> JSON::parseNull(Parser& parser)
{
  parser.read("null");
  return objectModel.createVoid();
}

Reference<ObjectModel::Boolean> JSON::parseBoolean(Parser& parser)
{
  switch (parser.peek()) {
  case 'f':
    parser.read("false");
    return objectModel.createBoolean(false);
  case 't':
    parser.read("true");
    return objectModel.createBoolean(true);
  }
  throw JSONException("Expected boolean.");
}

Reference<ObjectModel::Integer> JSON::parseInteger(Parser& parser)
{
  return nullptr;
}

Reference<ObjectModel::Float> JSON::parseFloat(Parser& parser)
{
  return nullptr;
}

Reference<ObjectModel::Value> JSON::parseNumber(Parser& parser)
{
  return parseInteger(parser);
  return parseFloat(parser);
  return nullptr;
}

Reference<ObjectModel::String> JSON::parseString(Parser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('"')) {
    throw JSONException("Expected string.");
  }
  parser.read('"');
  const Parser begin = parser;
  while (parser.peek() != '"') {
    // TAG: handle escapes
    parser.skip();
  }
  parser.read('"');
  return nullptr;
}

Reference<ObjectModel::Array> JSON::parseArray(Parser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('[')) {
    throw JSONException("Expected array.");
  }
  parser.read('[');
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
      parser.read(',');
    case ']':
      parser.read(']');
      return result;
    default:
      throw JSONException("Malformed array.");
    }
  }
  return nullptr;
}

Reference<ObjectModel::Object> JSON::parseObject(Parser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('{')) {
    throw JSONException("Expected object.");
  }
  parser.read('{');
  Reference<ObjectModel::Object> result = objectModel.createObject();
  skipSpaces(parser);
  if (parser.peek() == '}') {
    parser.skip();
    return result; // TAG: reuse empty object
  }
  while (true) {
    skipSpaces(parser);
    Reference<ObjectModel::String> key = parseString(parser);
    skipSpaces(parser);
    parser.read(':');
    skipSpaces(parser);
    Reference<ObjectModel::String> value = parseValue(parser);
    result->setValue(key, value);
    skipSpaces(parser);
    switch (parser.peek()) {
    case ',':
      parser.read(',');
      break;
    case '}':
      parser.read('}');
      return result;
    default:
      throw JSONException("Malformed object.");
    }
  }
  return nullptr;
}

Reference<ObjectModel::Value> JSON::parseValue(Parser& parser)
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
  Parser parser(src, end);
  return parseObject(parser);
}

Reference<ObjectModel::Value> JSON::parseFile(const String& path)
{
  PrimitiveArray<uint8> buffer(0);
  try {
    File file(path, File::READ, 0);
    buffer.resize(file.getSize());
    file.read(buffer, buffer.size(), false);
    file.close();
  } catch (...) {
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
