/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/Format.h>
#include <base/string/StringOutputStream.h>
#include <base/AnyValue.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Format::substImpl(const UTF8Stringify& text, const UTF8Stringify* args, MemorySize numberOfArgs)
{
  Subst s(text, args, numberOfArgs);
  return s.format();
}

Format::Subst::Subst(const UTF8Stringify& _text, const UTF8Stringify* _args, MemorySize _numberOfArgs)
  : text(_text),
    args(_args),
    numberOfArgs(_numberOfArgs)
{
}

String Format::Subst::format() const
{
  String buffer; // we could reuse buffer for thread but may be too weird if copy() is forgotten

  ConstSpan<char> _text = text.getSpan();
  MemorySize capacity = _text.getSize();
  for (MemorySize i = 0; i < numberOfArgs; ++i) {
    capacity += getArg(i).getSize();
  }
  buffer.ensureCapacity(capacity);

  const char* src = _text.begin();
  const char* end = _text.end();
  const char* segmentBegin = src;
  while (src != end) {
    if (*src != '%') {
      ++src;
      continue;
    }
    buffer.append(ConstSpan<char>(segmentBegin, src));
    ++src; // skip %
    if (src == end) {
      // BASSERT(!"Unexpected end after % for substitution string.");
      buffer.append('%'); // better to allow bad string format since this could comes from easy mistakes
      segmentBegin = src; // restart segment
      break;
    }
    if (*src == '%') { // escaped %
      ++src; // skip %
      buffer.append('%');
      segmentBegin = src; // restart segment
      continue;
    }
    
    const char* digitsBegin = src;
    while ((src != end) && ASCIITraits::isDigit(*src)) {
      ++src;
    }
    if (src == digitsBegin) { // no digits
      // BASSERT(!"Expected digits after % for substitution string.");
      buffer.append('%'); // better to allow bad string format since this could comes from easy mistakes
      segmentBegin = src; // restart segment
      continue;
    }

    if ((src - digitsBegin) > 2) { // we expect max 2 digits
      // BASSERT(!"Too many digits after % for substitution string.");
      buffer.append(MESSAGE("<NULL>")); // behave like missing argument
      segmentBegin = src; // restart segment
      continue;
    }

    MemorySize index = 0;
    for (MemoryDiff i = 0; i < (src - digitsBegin); ++i) { // we expect max 2 digits
      index *= 10;
      index = index + static_cast<MemorySize>(ASCIITraits::digitToValue(digitsBegin[i]));
    }

    if (index == 0) {
      // BASSERT(!"Expected 1-based index for substitution string.");
      buffer.append(MESSAGE("<NULL>")); // behave like missing argument
      segmentBegin = src; // restart segment
      continue;
    }

    --index;
    if (index < getNumberOfArgs()) { // assert may be too annoying
      buffer.append(getArg(index));
    } else {
      buffer.append(MESSAGE("<NULL>")); // missing argument
    }
    segmentBegin = src;
  }

  buffer.append(ConstSpan<char>(segmentBegin, src));
  return buffer;
}

FormatOutputStream& Format::Subst::operator<<(FormatOutputStream& stream) const
{
  // TAG: write directly without buffer?
  stream << format();
  return stream;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Format::Subst& subst)
{
  return subst.operator<<(stream);
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Format) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/string");

  void run() override
  {
    TEST_EQUAL(Format::subst("My name is %1 and I'm %2 years old.", "John", 18), "My name is John and I'm 18 years old.");
    TEST_EQUAL(Format::subst("Current percent is %1%%.", 45), "Current percent is 45%.");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
    defined(__GNUC__) && (__GNUC__ < 6) // temporary workaround // Ubuntu 16.04
    TEST_EQUAL(Format::subst("%1", AnyValue(false)), "false");
    TEST_EQUAL(Format::subst("%1", AnyValue(true)), "true");
#else
    TEST_EQUAL(Format::subst("%1", false), "false");
    TEST_EQUAL(Format::subst("%1", true), "true");
#endif
    TEST_EQUAL(Format::subst("%1", '!'), "!");
    TEST_EQUAL(Format::subst("%1", L'!'), "!");
    TEST_EQUAL(Format::subst("%1", -123), "-123");
    TEST_EQUAL(Format::subst("%1", 123U), "123");
    TEST_EQUAL(Format::subst("%1", -123.0f), "-123");
    TEST_EQUAL(Format::subst("%1", -123.0), "-123");
    TEST_EQUAL(Format::subst("%1", -123.0L), "-123");

    volatile int zero = 0;
    TEST_EQUAL(Format::subst("%1", 0.0f/zero), "nan");
    TEST_EQUAL(Format::subst("%1", 0.0/zero), "nan");
    TEST_EQUAL(Format::subst("%1", 0.0L/zero), "nan");
    TEST_EQUAL(Format::subst("%1", 123.0f/zero), "inf");
    TEST_EQUAL(Format::subst("%1", 123.0/zero), "inf");
    TEST_EQUAL(Format::subst("%1", 123.0L/zero), "inf");
    TEST_EQUAL(Format::subst("%1", -123.0f/zero), "-inf");
    TEST_EQUAL(Format::subst("%1", -123.0/zero), "-inf");
    TEST_EQUAL(Format::subst("%1", -123.0L/zero), "-inf");

    TEST_EQUAL(Format::subst("%1", String("")), "");
    TEST_EQUAL(Format::subst("%1", String("Hello, World!")), "Hello, World!");
    TEST_EQUAL(Format::subst("%1", String(WideString(L"Hello, World!"))), "Hello, World!");

    // TEST_EQUAL(Format::subst(""), "");
    TEST_EQUAL(Format::subst("%1 %2 %3", 1, 2, 3), "1 2 3");
    TEST_EQUAL(Format::subst("%1 %1", 1), "1 1"); // repeat
    TEST_EQUAL(Format::subst("%1 %1", 1, 2), "1 1"); // unused
    TEST_EQUAL(Format::subst("%1 %2 %3 %4", 1, 2, 3), "1 2 3 <NULL>"); // missing

    // TEST_EQUAL(Format::subst("%0"), "<NULL>"); // bad index
    TEST_EQUAL(Format::subst("%99", 0), "<NULL>"); // bad index
    TEST_EQUAL(Format::subst("%-", 0), "%-"); // missing % escape

    TEST_EQUAL(
      Format::subst(
        "The value is %1 = %2 = %3.",
        format() << 12345, format() << HEX << 12345, format() << ZEROPAD << setWidth(10) << 12345
      ),
      "The value is 12345 = 0x3039 = 0000012345."
    ); // inline formatting

    TEST_EQUAL("My name is %1 %2."_s("John", "Doe"), "My name is John Doe.");
    TEST_EQUAL(String("My name is %1 %2.")("John", "Doe"), "My name is John Doe.");

    fout << Format::subst("Hello %1 %2 %3 %4 %5.", "111", String("21312312"), L"234234", WideString("Hello")) << ENDL;
  }
};

TEST_REGISTER(Format);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
