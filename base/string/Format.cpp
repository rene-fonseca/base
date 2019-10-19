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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: add method for validating UTF-8 encoding in String - getUTF8Length()

String Format::subst(const String& text)
{
  return Subst(text, reinterpret_cast<const String**>(nullptr), 0).format();
}

String Format::subst(const String& text, const String& a)
{
  const String* args[] = {&a};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b)
{
  const String* args[] = {&a, &b};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c)
{
  const String* args[] = {&a, &b, &c};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d)
{
  const String* args[] = {&a, &b, &c, &d};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e)
{
  const String* args[] = {&a, &b, &c, &d, &e};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f)
{
  const String* args[] = {&a, &b, &c, &d, &e, &f};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g)
{
  const String* args[] = {&a, &b, &c, &d, &e, &f, &g};
  return Subst(text, args, getArraySize(args)).format();
}

String Format::subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h)
{
  const String* args[] = {&a, &b, &c, &d, &e, &f, &g, &h};
  return Subst(text, args, getArraySize(args)).format();
}

Format::Subst::Subst(const String& _text, const String** _args, unsigned int _numberOfArgs)
  : text(_text),
    args(_args),
    numberOfArgs(_numberOfArgs)
{
}

Format::Subst::Subst(const String& _text, const MemorySpan* _args, unsigned int _numberOfArgs)
  : text(_text),
    args2(_args),
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

String Format::subst(const String& text, std::initializer_list<const char*> list)
{
  MemorySpan spans[32];
  if (!INLINE_ASSERT(list.size() <= getArraySize(spans))) {
    throw OutOfRange();
  }
  unsigned int j = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    spans[j++] = MemorySpan(*i);
  }
  return Subst(text, spans, j).format();
}

String Format::subst(const String& text, std::initializer_list<Literal> list)
{
  MemorySpan spans[32];
  if (!INLINE_ASSERT(list.size() <= getArraySize(spans))) {
    throw OutOfRange();
  }
  unsigned int j = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    spans[j++] = MemorySpan(i->getValue(), i->getLength());
  }
  return Subst(text, spans, j).format();
}

// TAG: we can add support for fast substitution by splitting text into array with indices to the arg
// TAG: Format::subst("My name is %1. I'm %2 years old.", Format::String() << DEC << myAge);

String Format::Subst::format() const
{
  String buffer; // we could reuse buffer for thread but may be too weird if copy() is forgotten
  // TAG: add String::operator<<()?

  MemorySize capacity = text.getLength();
  for (unsigned int i = 0; i < numberOfArgs; ++i) {
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
      // ASSERT(!"Unexpected end after % for substitution string.");
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
      // ASSERT(!"Expected digits after % for substitution string.");
      buffer.append('%'); // better to allow bad string format since this could comes from easy mistakes
      segmentBegin = src; // restart segment
      continue;
    }

    if ((src - digitsBegin) > 2) { // we expect max 2 digits
      // ASSERT(!"Too many digits after % for substitution string.");
      buffer.append(MESSAGE("<NULL>")); // behave like missing argument
      segmentBegin = src; // restart segment
      continue;
    }

    unsigned int index = 0;
    for (MemoryDiff i = 0; i < (src - digitsBegin); ++i) { // we expect max 2 digits
      index *= 10;
      index = static_cast<unsigned int>(ASCIITraits::digitToValue(digitsBegin[i]));
    }

    if (index == 0) {
      // ASSERT(!"Expected 1-based index for substitution string.");
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
