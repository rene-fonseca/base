/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

// TAG: add support for UTF8,UTF16,UTF32
// TAG: handle chars in JSON strings!

#include <base/objectmodel/JSON.h>
#include <base/io/File.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const LineColumn& position)
{
  return stream << position.line << ":" << position.column;
}

JSON::JSON()
{
  buffer.resize(4096);
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
  _throw JSONException("Expected boolean.", parser.getPosition());
}

bool JSON::parseIntegerImpl(JSONParser& parser, int64& j)
{
  // numbers in the range [-(2**53)+1, (2**53)-1] must be exact

  skipSpaces(parser);
  if (!parser.hasMore()) {
    return false;
  }
  int64 i = 0;
  int sign = 1;
  if (parser.peek() == '-') {
    parser.read();
    sign = -1;
  }

  // TAG: add support info on position of failure for exceptions - line and column

  if (!parser.hasMore()) {
    return false;
  }
  if (parser.peek() == '0') {
    parser.skip(); // done
  } else {
    if (!((parser.peek() >= '1') && (parser.peek() <= '9'))) {
      return false;
    }
    const char ch = parser.read();
    i = static_cast<uint8>(ch - '0'); // first digit
    while (parser.hasMore() && (parser.peek() >= '0') && (parser.peek() <= '9')) {
      const char ch = parser.read();
      uint8 digit = static_cast<uint8>(ch - '0');
      i = i * 10 + digit;
      if ((i * sign) < PrimitiveTraits<int64>::MINIMUM) {
        return false;
      } else if ((i * sign) > PrimitiveTraits<int64>::MAXIMUM) {
        return false;
      }
    }
  }

  if (parser.hasMore()) {
    switch (parser.peek()) {
    case '.':
    case 'e':
    case 'E':
      return false; // float chars
    }
  }
  
  j = static_cast<int64>(i * sign);
  return true;
}

Reference<ObjectModel::Integer> JSON::parseInteger(JSONParser& parser)
{
  skipSpaces(parser);
  int64 i = 0;
  if (!parseIntegerImpl(parser, i)) {
    _throw JSONException("Expected integer.", parser.getPosition());
  }
  return objectModel.createInteger(i);
}

Reference<ObjectModel::Float> JSON::parseFloat(JSONParser& parser)
{
  skipSpaces(parser);

  const char* b = reinterpret_cast<const char*>(parser.getCurrent());

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

  const char* e = reinterpret_cast<const char*>(parser.getCurrent());

  double d = 0;
  if (!posix.getSeries(b, e, d)) {
    _throw JSONException("Malformed float.", parser.getPosition());
  }
  
  return objectModel.createFloat(d);
}

Reference<ObjectModel::Value> JSON::parseNumber(JSONParser& parser)
{
  skipSpaces(parser);
  int64 i = 0;
  JSONParser integerParser = parser;
  if (parseIntegerImpl(integerParser, i)) {
    parser = integerParser;
    return objectModel.createInteger(i);
  }
  return parseFloat(parser);
}

namespace {

  class BufferWrapper {
  public:

    PrimitiveArray<char>& buffer;
    char* dest = nullptr;
    const char* end = nullptr;
  public:

    inline BufferWrapper(PrimitiveArray<char>& _buffer) noexcept
      : buffer(_buffer)
    {
      dest = buffer.begin();
      end = buffer.end();
    }

    void expand()
    {
      const MemorySize offset = dest - buffer.begin();
      buffer.resize(buffer.size() * 2);
      dest = buffer.begin() + offset; // new write position
      end = buffer.end();
    }

    inline void push(char ch)
    {
      if (dest == end) {
        expand();
      }
      *dest = ch;
      ++dest;
    }

    inline operator String() const
    {
      return String(buffer.begin(), dest - buffer.begin());
    }
  };
}

namespace {

  uint16 readUTF16Word(JSON::JSONParser& parser)
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
      return value;
    }
    _throw JSONException("Malformed UTF-16 word for string literal.", parser.getPosition());
  }
}

Reference<ObjectModel::String> JSON::parseString(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('"')) {
    _throw JSONException("Expected string.", parser.getPosition());
  }

  BufferWrapper buffer(this->buffer);

  parser.skip();
  while (parser.peek() != '"') {
    const char ch = parser.read();
    if (ch == '\\') { // escape
      const char ch2 = parser.read();
      switch (ch2) {
      case '"':
        buffer.push('"');
        break;
      case '\\':
        buffer.push('\\');
        break;
      case '/':
        buffer.push('/');
        break;
      case 'b':
        buffer.push('\b');
        break;
      case 'f':
        buffer.push('\f');
        break;
      case 'n':
        buffer.push('\n');
        break;
      case 'r':
        buffer.push('\r');
        break;
      case 't':
        buffer.push('\t');
        break;
      case 'u':
        {
          // none unicode chars are allowed per rfc7159 - should we allow this
          const uint16 value = readUTF16Word(parser);
          if ((value >= 0xd800) && (value <= 0xdfff)) { // surrogate words
            if (value >= 0xdc00) {
              _throw JSONException("Unexpected UTF-16 low surrogate.", parser.getPosition());
            }
            if (parser.peek() != '\\') {
              _throw JSONException("Missing UTF-16 low surrogate.", parser.getPosition());
            }
            parser.skip();
            if (parser.peek() != 'u') {
              _throw JSONException("Missing UTF-16 low surrogate.", parser.getPosition());
            }
            parser.skip();
            const uint16 value2 = readUTF16Word(parser);
            if (!((value2 >= 0xdc00) && (value2 <= 0xdfff))) { // surrogate word
              _throw JSONException("Expected UTF-16 low surrogate.", parser.getPosition());
            }
            const uint32 high = value - 0xd800; // leading
            const uint32 low = value2 - 0xdc00; // trailing
            const ucs4 ch = (high << 10) | low;
            uint8 bytes[4];
            MemorySize size = Unicode::writeUTF8(bytes, ch);
            for (MemorySize i = 0; i < size; ++i) {
              buffer.push(bytes[i]);
            }
          } else {
            uint8 bytes[4];
            MemorySize size = Unicode::writeUTF8(bytes, ch);
            for (MemorySize i = 0; i < size; ++i) {
              buffer.push(bytes[i]);
            }
          }
        }
        break;
      default:
        _throw JSONException("Malformed string literal.", parser.getPosition());
      }
    } else {
      if (static_cast<uint8>(ch) < 0x20) {
        _throw JSONException("Malformed string literal.", parser.getPosition());
      }
      if (static_cast<uint8>(ch) < 0x80) {
        buffer.push(ch);
        continue;
      }

      // ACCEPTING: '0020' . '10FFFF' - '"' - '\'
      parser.unwind();
      const auto begin = parser.getCurrent();
      const ucs4 uch = parser.readUCS4(); // TAG: handle UTF-8/16/32
      if (uch > 0x10ffff) {
        _throw JSONException("Bad UTF8 string literal.", parser.getPosition());
      }
      const MemoryDiff bytesRead = parser.getCurrent() - begin;
      for (MemoryDiff i = 0; i < bytesRead; ++i) {
        buffer.push(begin[i]);
      }
    }
  }
  parser.read('"');
  return objectModel.createString(buffer);
}

String JSON::parseString(const String& text)
{
  JSON json;
  JSON::JSONParser parser(
    reinterpret_cast<const uint8*>(text.native()),
    reinterpret_cast<const uint8*>(text.native()) + text.getLength()
  );
  auto s = json.parseString(parser);
  return s->value;
}

Reference<ObjectModel::Array> JSON::parseArray(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('[')) {
    _throw JSONException("Expected array.", parser.getPosition());
  }
  parser.skip();
  skipSpaces(parser);
  Reference<ObjectModel::Array> result = objectModel.createArray();
  // result->values.ensureCapacity(16); // use previous array size
  if (parser.peek() == ']') {
    parser.skip();
    return result; // empty
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
      {
        parser.skip();
        return result;
      }
    default:
      _throw JSONException("Malformed array.", parser.getPosition());
    }
  }
  return nullptr;
}

Reference<ObjectModel::Object> JSON::parseObject(JSONParser& parser)
{
  skipSpaces(parser);
  if (!parser.peek('{')) {
    _throw JSONException("Expected object.", parser.getPosition());
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
      _throw JSONException("Malformed object.", parser.getPosition());
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
  
  // TAG: add support for UTF-16 and UTF-32
  
  // we may read BOM - but not write it - see rfc7159
  if (static_cast<uint8>(parser.peek()) == 0xef) {
    parser.skip();
    if (static_cast<uint8>(parser.peek()) == 0xbb) {
      parser.skip();
      if (static_cast<uint8>(parser.peek()) == 0xbf) {
        parser.skip();
      } else {
        parser.unwind();
        parser.unwind();
      }
    } else {
      parser.unwind();
    }
  }
  
  Reference<ObjectModel::Value> result = JSON::parseValue(parser);
  skipSpaces(parser);
  if (parser.hasMore()) {
    _throw JSONException("Unexpected content after object.", parser.getPosition());
  }
  return result;
}

Reference<ObjectModel::Value> JSON::parse(const String& text)
{
  JSON json;
  return json.parse(text.getBytes(), text.getBytes() + text.getLength());
}

Reference<ObjectModel::Value> JSON::parseFile(const String& path)
{
  // TAG: need support for streaming
  PrimitiveStackArray<uint8> buffer(0);
  {
    File file(path, File::READ, 0);
    buffer.resize(file.getSize());
    file.read(buffer, (unsigned int)buffer.size(), false);
  }
  JSON json;
  return json.parse(buffer, buffer + buffer.size());
}

String JSON::getJSON(Reference<ObjectModel::Value> value, unsigned int flags)
{
  if (!value) {
    _throw NullPointer();
  }
  return value->toString(flags);
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(JSON) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/objectmodel");

  bool ensureFailure(const char* text)
  {
#if (!defined(_COM_AZURE_DEV__BASE__NO_EXCEPTIONS)) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__S390X)
    try {
      JSON().parse(text);
    } catch (...) {
      return true;
    }
    return false;
#else
    return true;
#endif
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

    String normal = JSON::getJSONNoFormatting(root);
    TEST_ASSERT(normal.getLength() > 1);
    TEST_ASSERT(normal[0] == '{');
    TEST_ASSERT(normal[normal.getLength() - 1] == '}');
    TEST_ASSERT(normal);
    auto o1 = JSON().parse(normal).cast<ObjectModel::Object>();
    String nice = JSON::getJSON(root);
    TEST_ASSERT(nice);
    auto o2 = JSON().parse(nice).cast<ObjectModel::Object>();
    TEST_EQUAL(JSON::getJSONNoFormatting(o2), normal); // floats should map to the same string representation also

    TEST_ASSERT(ensureFailure("#"));
    TEST_ASSERT(ensureFailure("{nul}"));
    TEST_ASSERT(ensureFailure("{false}"));
    TEST_ASSERT(ensureFailure("{,}"));
    TEST_ASSERT(ensureFailure("{\"qwerty\":true,}"));
    TEST_ASSERT(ensureFailure("{qwerty:1}"));
    TEST_ASSERT(ensureFailure("{\"qwerty:1}"));
    TEST_ASSERT(ensureFailure("{\"qwerty\"\":1}"));
    // TEST_ASSERT(ensureFailure("{\"qwerty\":1e}")); // if this allowed by spec?
    TEST_ASSERT(ensureFailure("{\"qwerty\":1e1e}"));
    TEST_ASSERT(ensureFailure("{\"qwerty\":1,1e}"));
    TEST_ASSERT(ensureFailure("{\"qwerty\":0x1}"));
    TEST_ASSERT(ensureFailure("[,]"));
    TEST_ASSERT(ensureFailure("[1,]"));
  
    const char* test1 = R""""(
{
  "Image": {
    "Width": 800,
    "Height": 600,
    "Title": "View from 15th Floor",
    "Thumbnail": {
      "Url": "http://www.example.com/image/481989943",
      "Height": 125,
      "Width": 100
    },
    "Animated": false,
    "IDs": [116, 943, 234, 38793]
  }
})"""";
    TEST_ASSERT(JSON().parse(test1));

    const char* test2 = R""""(
[
  {
    "precision": "zip",
    "Latitude":  37.7668,
    "Longitude": -122.3959,
    "Address":   "",
    "City":      "SAN FRANCISCO",
    "State":     "CA",
    "Zip":       "94107",
    "Country":   "US"
  },
  {
    "precision": "zip",
    "Latitude":  37.371991,
    "Longitude": -122.026020,
    "Address":   "",
    "City":      "SUNNYVALE",
    "State":     "CA",
    "Zip":       "94085",
    "Country":   "US"
  }
])"""";
    TEST_ASSERT(JSON().parse(test2));

    TEST_ASSERT(JSON().parse("\"Hello world!\""));
    TEST_ASSERT(JSON().parse("42"));
    TEST_ASSERT(JSON().parse("true"));
  
    const char* test3 = R""""(
{
  "foo": ["bar", "baz"],
  "": 0,
  "a/b": 1,
  "c%d": 2,
  "e^f": 3,
  "g|h": 4,
  "i\\j": 5,
  "k\"l": 6,
  " ": 7,
  "m~n": 8
})"""";
  
    auto root3 = JSON().parse(test3).cast<ObjectModel::Object>();
    TEST_ASSERT(root3->getPath("").isType<ObjectModel::Object>()); // entire document
    TEST_ASSERT(root3->getPath("/foo").isType<ObjectModel::Array>()); // ["bar", "baz"]
    TEST_ASSERT(root3->getString("/foo/0", "") == "bar");
    TEST_ASSERT(root3->getInteger("/", -1) == 0);

    TEST_ASSERT(root3->getInteger("/a~1b", -1) == 1);
    TEST_ASSERT(root3->getInteger("/c%d", -1) == 2);
    TEST_ASSERT(root3->getInteger("/e^f", -1) == 3);
    TEST_ASSERT(root3->getInteger("/g|h", -1) == 4);
    TEST_ASSERT(root3->getInteger("/i\\j", -1) == 5);
    TEST_ASSERT(root3->getInteger("/k\"l", -1) == 6);
    TEST_ASSERT(root3->getInteger("/ ", -1) == 7);
    TEST_ASSERT(root3->getInteger("/m~0n", -1) == 8);
  }
};

TEST_REGISTER(JSON);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
