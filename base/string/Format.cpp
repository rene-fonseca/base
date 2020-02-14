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

#if 0
void Subst::setArgs(std::initializer_list<UTF8Stringify> _args)
{
  if (_args.size() > getArraySize(args)) {
    _throw OutOfRange("Too many arguments.", this);
  }
  MemorySize i = 0;
  for (const UTF8Stringify& arg : _args) { // we cant move arg since const
    args[i] = arg;
  }
  size = _args.size();
}
#endif

void Subst::setArgs(UTF8Stringify* _args, MemorySize _size)
{
  if (size > getArraySize(args)) {
    _throw OutOfRange("Too many arguments.", this);
  }
  for (MemorySize i = 0; i < _size; ++i) {
    args[i] = moveObject(_args[i]);
    args[i].ensureOwnership(); // make sure we dont release temp memory
  }
  size = _size;
}

String operator%(const UTF8Stringify& text, const Subst& subst)
{
  return Format::substImpl(text, subst.getArgs(), subst.getNumberOfArgs());
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

namespace {

  class MyClass {
  public:
  };
  
  FormatOutputStream& operator<<(FormatOutputStream& stream, const MyClass& value)
  {
    return stream << "MyClass";
  }
}

class TEST_CLASS(Format) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/string");

  void run() override
  {
    TEST_EQUAL(Format::subst("My name is %1 and I'm %2 years old.", "John", 18), "My name is John and I'm 18 years old.");
    TEST_EQUAL(Format::subst("Current percent is %1%%.", 45), "Current percent is 45%.");

    TEST_EQUAL(Format::subst("Class %1", MyClass()), "Class MyClass");

    TEST_EQUAL(Format::subst("%1", false), "false");
    TEST_EQUAL(Format::subst("%1", true), "true");
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

    TEST_EQUAL("My name is %1 %2." % Subst("John", "Doe"), "My name is John Doe.");
    TEST_ASSERT("Complex %1 %2 %3 %4." % Subst(true, -123, Array<int>({1,-5,9,-11}), String("=") * 64));
  }
};

TEST_REGISTER(Format);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
