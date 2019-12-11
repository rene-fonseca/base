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
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  inline MemorySpan toSpan(const String& s) noexcept
  {
    return MemorySpan(s.getElements(), s.getLength());
  }
}

String Format::subst(const String& text)
{
  return Subst(text, nullptr, 0).format();
}

String Format::subst(const String& text, const String& a)
{
  MemorySpan spans[] = {toSpan(a)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g), toSpan(h)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g), toSpan(h), toSpan(i)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g), toSpan(h), toSpan(i), toSpan(j)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j, const String& k)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g), toSpan(h), toSpan(i), toSpan(j), toSpan(k)};
  return Subst(text, spans, getArraySize(spans)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j, const String& k, const String& l)
{
  MemorySpan spans[] = {toSpan(a), toSpan(b), toSpan(c), toSpan(d), toSpan(e), toSpan(f), toSpan(g), toSpan(h), toSpan(i), toSpan(j), toSpan(k), toSpan(l)};
  return Subst(text, spans, getArraySize(spans)).format();
}

Format::Subst::Subst(const String& _text, const MemorySpan* _args, MemorySize _numberOfArgs)
  : text(_text),
    args(_args),
    numberOfArgs(_numberOfArgs)
{
}

String Format::subst(const String& text, const AnyValue& a)
{
  return subst(text, a.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b)
{
  return subst(text, a.getString(), b.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c)
{
  return subst(text, a.getString(), b.getString(), c.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString(), h.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j, const AnyValue& k, const AnyValue& l)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString(), h.getString(), h.getString(), i.getString(), j.getString(), k.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString(), h.getString(), h.getString(), i.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString(), h.getString(), h.getString(), i.getString(), j.getString());
}

String Format::subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j, const AnyValue& k)
{
  return subst(text, a.getString(), b.getString(), c.getString(), d.getString(), e.getString(), f.getString(), g.getString(), h.getString(), h.getString(), i.getString(), j.getString(), k.getString());
}

String Format::subst(const String& text, std::initializer_list<const char*> list)
{
  MemorySpan spans[MAX_ARGS];
  if (!INLINE_ASSERT(list.size() <= getArraySize(spans))) {
    throw OutOfRange();
  }
  MemorySize j = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    spans[j++] = MemorySpan(*i);
  }
  return Subst(text, spans, j).format();
}

String Format::subst(const String& text, std::initializer_list<Literal> list)
{
  MemorySpan spans[MAX_ARGS];
  if (!INLINE_ASSERT(list.size() <= getArraySize(spans))) {
    throw OutOfRange();
  }
  MemorySize j = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    spans[j++] = MemorySpan(i->getValue(), i->getLength());
  }
  return Subst(text, spans, j).format();
}

String Format::Subst::format() const
{
  String buffer; // we could reuse buffer for thread but may be too weird if copy() is forgotten

  MemorySize capacity = text.getLength();
  for (MemorySize i = 0; i < numberOfArgs; ++i) {
    capacity += getArg(i).getSize();
  }
  buffer.ensureCapacity(capacity);

  const char* src = text.getElements();
  const char* end = text.getEnd();
  const char* segmentBegin = src;
  while (src != end) {
    if (*src != '%') {
      ++src;
      continue;
    }
    buffer.append(MemorySpan(segmentBegin, src));
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
      index = static_cast<MemorySize>(ASCIITraits::digitToValue(digitsBegin[i]));
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

  buffer.append(MemorySpan(segmentBegin, src));
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

    TEST_EQUAL(Format::subst(""), "");
    TEST_EQUAL(Format::subst("%1 %2 %3", 1, 2, 3), "1 2 3");
    TEST_EQUAL(Format::subst("%1 %1", 1), "1 1"); // repeat
    TEST_EQUAL(Format::subst("%1 %1", 1, 2), "1 1"); // unused
    TEST_EQUAL(Format::subst("%1 %2 %3 %4", 1, 2, 3), "1 2 3 <NULL>"); // missing

    TEST_EQUAL(Format::subst("%0"), "<NULL>"); // bad index
    TEST_EQUAL(Format::subst("%99"), "<NULL>"); // bad index
    TEST_EQUAL(Format::subst("%-"), "%-"); // missing % escape

    TEST_EQUAL(
      Format::subst(
        "The value is %1 = %2 = %3.",
        format() << 12345, format() << HEX << 12345, format() << ZEROPAD << setWidth(10) << 12345
      ),
      "The value is 12345 = 0x3039 = 0000012345."
    ); // inline formatting

  }
};

TEST_REGISTER(Format);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
