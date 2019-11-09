/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/objectmodel/YAML.h>
#include <base/io/File.h>
#include <base/string/Posix.h>
#include <base/mathematics/Math.h>
#include <base/UnitTest.h>
#include <locale>
#include <codecvt>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: implement YAML parser - copy from JSON for now
 
YAML::YAML()
{
}

Reference<ObjectModel::Void> YAML::parseNull(YAMLParser& parser)
{
  skipSpaces(parser);
  parser.read("null");
  return objectModel.createVoid();
}

Reference<ObjectModel::Boolean> YAML::parseBoolean(YAMLParser& parser)
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
  throw YAMLException("Expected boolean.", parser.getPosition());
}

bool YAML::parseIntegerImpl(YAMLParser& parser, int& j)
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

Reference<ObjectModel::Integer> YAML::parseInteger(YAMLParser& parser)
{
  skipSpaces(parser);
  int i = 0;
  if (!parseIntegerImpl(parser, i)) {
    throw YAMLException("Expected integer.", parser.getPosition());
  }
  return objectModel.createInteger(i);
}

Reference<ObjectModel::Float> YAML::parseFloat(YAMLParser& parser)
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
    throw YAMLException("Malformed float.", parser.getPosition());
  }
  
  return objectModel.createFloat(d);
}

Reference<ObjectModel::Value> YAML::parseNumber(YAMLParser& parser)
{
  skipSpaces(parser);
  int i = 0;
  YAMLParser integerParser = parser;
  if (parseIntegerImpl(integerParser, i)) {
    parser = integerParser;
    return objectModel.createInteger(i);
  }
  return parseFloat(parser);
}

Reference<ObjectModel::String> YAML::parseString(YAMLParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('"')) {
    throw YAMLException("Expected string.", parser.getPosition());
  }
  text.clear();
  text.reserve(1024 * 8);
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
        {
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
            throw YAMLException("Malformed string literal.", parser.getPosition());
          }
        }
        break;
      default:
        throw YAMLException("Malformed string literal.", parser.getPosition());
      }
    } else {
      if (static_cast<uint8>(ch) < 0x20) {
        throw YAMLException("Malformed string literal.", parser.getPosition());
      }
      if (static_cast<uint8>(ch) < 0x80) {
        text.push_back(ch);
        continue;
      }

      // ACCEPTING: '0020' . '10FFFF' - '"' - '\'
      parser.unwind();
      const auto begin = parser.getCurrent();
      const ucs4 uch = parser.readUCS4();
      if (uch > 0x10ffff) {
        throw YAMLException("Bad UTF8 string literal.", parser.getPosition());
      }
      MemoryDiff bytesRead = parser.getCurrent() - begin;
      for (MemoryDiff i = 0; i < bytesRead; ++i) {
        text.push_back(begin[i]);
      }
    }
  }
  parser.read('"');
  return objectModel.createString(String(text));
}

Reference<ObjectModel::Array> YAML::parseArray(YAMLParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('[')) {
    throw YAMLException("Expected array.", parser.getPosition());
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
      throw YAMLException("Malformed array.", parser.getPosition());
    }
  }
  return nullptr;
}

Reference<ObjectModel::Object> YAML::parseObject(YAMLParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('{')) {
    throw YAMLException("Expected object.", parser.getPosition());
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
      throw YAMLException("Malformed object.", parser.getPosition());
    }
  }
  return nullptr;
}

Reference<ObjectModel::Value> YAML::parseValue(YAMLParser& parser)
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

Reference<ObjectModel::Value> YAML::parse(const uint8* src, const uint8* end)
{
  YAMLParser parser(src, end);
  auto result = parseObject(parser);
  skipSpaces(parser);
  if (parser.hasMore()) {
    throw YAMLException("Unexpected content after object.", parser.getPosition());
  }
  return result;
}

Reference<ObjectModel::Value> YAML::parse(const String& text)
{
  return parse(
    reinterpret_cast<const uint8*>(text.getElements()),
    reinterpret_cast<const uint8*>(text.getElements()) + text.getLength()
  );
}

Reference<ObjectModel::Value> YAML::parseFile(const String& path)
{
  // TAG: need support for streaming
  PrimitiveArray<uint8> buffer(0);
  {
    File file(path, File::READ, 0);
    buffer.resize(file.getSize());
    file.read(buffer, buffer.size(), false);
  }
  YAML yaml;
  return yaml.parse(buffer, buffer + buffer.size());
}

class YAMLOutputStream : public StringOutputStream {
public:
  
  unsigned int level = 0;
  bool eol = true;
  
  inline YAMLOutputStream() : StringOutputStream() {
  }
};

void toStringYAML(YAMLOutputStream& stream, const Reference<ObjectModel::Value>& value);

void toStringYAML(YAMLOutputStream& stream, const Reference<ObjectModel::Void>& value)
{
  stream << MESSAGE("null");
}

void toStringYAML(YAMLOutputStream& stream, const Reference<ObjectModel::Boolean>& value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  stream << (value->value ? MESSAGE("true") : MESSAGE("false"));
}

void toStringYAML(YAMLOutputStream& stream, Reference<ObjectModel::Integer> value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  stream << value->value;
}

void toStringYAML(YAMLOutputStream& stream, Reference<ObjectModel::Float> value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  
  if (Math::isFinite(value->value)) { // not inf or nan
    stream << value->value; // POSIX from higher level
  } else if (Math::isNaN(value->value)) {
    stream << MESSAGE(".NaN"); // should we use null instead of string
  } else if (Math::isInfinity(value->value)) {
    stream << ((value->value < 0) ? MESSAGE("-.inf") : MESSAGE(".inf")); // should we use null instead of string
  } else {
    BASSERT(!"Unsupported float.");
    stream << value->value;
  }
}

// TAG: add support for comment type

void toStringYAML(YAMLOutputStream& stream, Reference<ObjectModel::String> value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  
  stream  << '"';
  const MemorySize length = value->value.getLength();
  for (MemorySize i = 0; i < length; ++i) {
    const char ch = value->value[i];
    // TAG: read ucs4
    BASSERT(ch <= 0x7f); // TAG: add support
    if (ch < ' ') {
      stream << '\\';
      switch (ch) {
      case '\b':
        stream << 'b';
        break;
      case '\f':
        stream << 'f';
        break;
      case '\n':
        stream << 'n';
        break;
      case '\r':
        stream << 'r';
        break;
      case '\t':
        stream << 't';
        break;
      default:
        stream << 'u';
        stream << ASCIITraits::valueToDigit(0);
        stream << ASCIITraits::valueToDigit(0);
        stream << ASCIITraits::valueToDigit((ch >> 4) & 0xf);
        stream << ASCIITraits::valueToDigit((ch >> 0) & 0xf);
      }
    } else if (ch == '\\') {
      stream << '\\';
      stream << '\\';
    } else if (ch == '"') {
      stream << '\\';
      stream << '"';
    } else {
      stream << ch;
    }
  }
  stream << '"';

  // TAG: stream << '|'; // folded style - indent before each line
}

void toStringYAML(YAMLOutputStream& stream, Reference<ObjectModel::Array> value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  
  if (value->values.isEmpty()) {
    stream << MESSAGE("[]");
    return;
  }

  const unsigned int spaces = stream.level * 2;
  ++stream.level;
  stream << EOL;
  for (const auto& v : value->values) {
    stream.eol = false;
    stream << indent(spaces) << MESSAGE("- ");
    toStringYAML(stream, v);
    stream << EOL;
    stream.eol = true;
  }
  --stream.level;
}

void toStringYAML(YAMLOutputStream& stream, Reference<ObjectModel::Object> value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  
  if (value->values.isEmpty()) {
    stream << MESSAGE("{}");
    return;
  }

  const unsigned int spaces = stream.level * 2;
  ++stream.level;
  for (const auto& v : value->values) {
    // TAG: if simple then no need for escaping
    if (!stream.eol) {
      stream << EOL;
    }
    stream.eol = false;
    stream << indent(spaces);
    toStringYAML(stream, v.first);
    stream << MESSAGE(": "); // TAG: do not add space if eol follow
    toStringYAML(stream, v.second);
    if (!stream.eol) {
      stream << EOL;
    }
    stream.eol = true;
  }
  --stream.level;
}

// TAG: binary blob type for objectmodel

void toStringYAML(YAMLOutputStream& stream, const Reference<ObjectModel::Value>& value)
{
  if (!value) {
    stream << MESSAGE("null");
    return;
  }
  switch (value->getType()) {
  case ObjectModel::Value::TYPE_VOID:
    toStringYAML(stream, value.cast<ObjectModel::Void>());
    break;
  case ObjectModel::Value::TYPE_BOOLEAN:
    toStringYAML(stream, value.cast<ObjectModel::Boolean>());
    break;
  case ObjectModel::Value::TYPE_INTEGER:
    toStringYAML(stream, value.cast<ObjectModel::Integer>());
    break;
  case ObjectModel::Value::TYPE_FLOAT:
    toStringYAML(stream, value.cast<ObjectModel::Float>());
    break;
  case ObjectModel::Value::TYPE_STRING:
    toStringYAML(stream, value.cast<ObjectModel::String>());
    break;
#if 0
  case ObjectModel::Value::TYPE_BINARY:
    toStringYAML(stream, value.cast<ObjectModel::Binary>());
    break;
#endif
    case ObjectModel::Value::TYPE_ARRAY:
    toStringYAML(stream, value.cast<ObjectModel::Array>());
    break;
  case ObjectModel::Value::TYPE_OBJECT:
    toStringYAML(stream, value.cast<ObjectModel::Object>());
    break;
  default:
    throw ObjectModelException("Invalid type.");
  }
}

void toStringYAMLDocument(YAMLOutputStream& stream, Reference<ObjectModel::Value> value)
{
  stream << "%YAML 1.1" << EOL
         << "---" << EOL;
  stream.eol = true;
  toStringYAML(stream, value);
  stream << "..." << ENDL;
  stream.eol = true;
}

String YAML::getYAML(Reference<ObjectModel::Value> value)
{
  if (!value) {
    throw NullPointer();
  }
  YAMLOutputStream stream;
  toStringYAMLDocument(stream, value);
  return stream.getString();
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(YAML) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/objectmodel");

  bool ensureFailure(const char* text)
  {
    try {
      YAML().parse(text);
    } catch (...) {
      return true;
    }
    return false;
  }

  void run() override
  {
    ObjectModel o;
    auto root = o.createObject();
    TEST_ASSERT(root->isEmpty());
    root->setValue(o.createString("name"), o.createString("value"));
    root->setValue(o.createString("state"), o.createBoolean(true));
    root->setValue(o.createString("integer"), o.createInteger(-123));
    root->setValue(o.createString("number"), o.createFloat(123.0));
    TEST_ASSERT(!root->isEmpty());
    TEST_ASSERT(root->getSize() == 4);

    auto a = o.createArray();
    root->setValue(o.createString("list"), a);
    a->append(o.createInteger(12));
    a->append(o.createInteger(54));
    a->append(o.createInteger(54));
    a->append(o.createInteger(66));
    a->append(o.createBoolean(false)); // mixed types
    a->append(o.createFloat(-456));
    a->append(o.createString(L"Hello, World!"));
    TEST_ASSERT(a->getSize() == 7);

    String normal = YAML::getYAML(root);
    TEST_ASSERT(normal);
    // auto o1 = YAML().parse(normal).cast<ObjectModel::Object>();
  }
};

TEST_REGISTER(YAML);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
