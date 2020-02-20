/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/UTF8Stringify.h>
#include <base/Literal.h>
#include <base/WideLiteral.h>
#include <base/string/Unicode.h>
#include <base/string/StringOutputStream.h>
#include <base/AnyValue.h>
#include <base/concurrency/ThreadLocalContext.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ThreadStringOutputStream::ThreadStringOutputStream()
{
  //sos = new StringOutputStream();
  //return;
  
  if (auto tlc = Thread::getLocalContext()) {
    if (tlc->stringOutputStreamUsage != 0) {
      _throw ResourceException("Thread local string output stream is already in use.");
    }
    tlc->stringOutputStreamUsage++;
    sos = &tlc->stringOutputStream;
    sos->restart();
  } else {
    _throw ResourceException("Thread has no thread local context.");
  }
}

FormatOutputStream& ThreadStringOutputStream::getStream()
{
  return *sos;
}

const String& ThreadStringOutputStream::getString() const
{
  return sos->getString();
}

ThreadStringOutputStream::~ThreadStringOutputStream()
{
  //return;
  if (auto tlc = Thread::getLocalContext()) {
    sos->restart();
    BASSERT(tlc->stringOutputStreamUsage > 0);
    tlc->stringOutputStreamUsage--;
  }
}

UTF8Stringify::UTF8Stringify()
{
}

UTF8Stringify::UTF8Stringify(NullPtr)
{
  span = ConstSpan<char>("nullptr", 7);
}

UTF8Stringify::UTF8Stringify(bool src)
{
  span = src ? ConstSpan<char>("true", 4) : ConstSpan<char>("false", 5);
}

UTF8Stringify::UTF8Stringify(int value)
{
  // TAG: we could delay string conversion till actual output
  BASSERT(sizeof(value) <= 4); // -2147483648
  char* dest = tiny + getArraySize(tiny);
  unsigned int temp = (value >= 0) ? value : -value;
  do {
    *--dest = ASCIITraits::valueToDigit(temp % 10); // get digit
    temp = temp/10;
  } while(temp > 0);
  if (value < 0) {
    *--dest = '-';
  }
  BASSERT(dest >= static_cast<const char*>(tiny));
  span = ConstSpan<char>(dest, (tiny + getArraySize(tiny)) - dest);
}

UTF8Stringify::UTF8Stringify(unsigned int value)
{
  // TAG: we could delay string conversion till actual output
  BASSERT(sizeof(value) <= 4); // 4294967295
  char* dest = tiny + getArraySize(tiny);
  do {
    *--dest = ASCIITraits::valueToDigit(value % 10); // get digit
    value = value/10;
  } while(value > 0);
  BASSERT(dest >= static_cast<const char*>(tiny));
  span = ConstSpan<char>(dest, (tiny + getArraySize(tiny)) - dest);
}

UTF8Stringify::UTF8Stringify(int64 value)
{
  // -9223372036854775808
  char* dest = tiny + getArraySize(tiny);
  unsigned int temp = (value >= 0) ? value : -value;
  do {
    *--dest = ASCIITraits::valueToDigit(temp % 10); // get digit
    temp = temp/10;
  } while(temp > 0);
  if (value < 0) {
    *--dest = '-';
  }
  BASSERT(dest >= static_cast<const char*>(tiny));
  span = ConstSpan<char>(dest, (tiny + getArraySize(tiny)) - dest);
}

UTF8Stringify::UTF8Stringify(uint64 value)
{
  // 18446744073709551615
  char* dest = tiny + getArraySize(tiny);
  do {
    *--dest = ASCIITraits::valueToDigit(value % 10); // get digit
    value = value/10;
  } while(value > 0);
  BASSERT(dest >= static_cast<const char*>(tiny));
  span = ConstSpan<char>(dest, (tiny + getArraySize(tiny)) - dest);
}

UTF8Stringify::UTF8Stringify(char src)
{
  tiny[0] = src;
  span = ConstSpan<char>(tiny, 1);
}

UTF8Stringify::UTF8Stringify(wchar src)
{
  ucs4 ch = src;
  MemorySize length = Unicode::writeUTF8(reinterpret_cast<uint8*>(tiny), ch);
  BASSERT(length <= getArraySize(tiny));
  span = ConstSpan<char>(tiny, length);
}

UTF8Stringify::UTF8Stringify(ucs4 src)
{
  MemorySize length = Unicode::writeUTF8(reinterpret_cast<uint8*>(tiny), src);
  BASSERT(length <= getArraySize(tiny));
  span = ConstSpan<char>(tiny, length);
}

UTF8Stringify::UTF8Stringify(const char* src)
  : span(src, getNullTerminatedLength(src))
{
}

UTF8Stringify::UTF8Stringify(const Literal& src)
  : span(src.getValue(), src.getLength())
{
}

UTF8Stringify::UTF8Stringify(const wchar* src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const WideLiteral& src)
{
  String temp(src.getValue(), src.getLength());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const ucs4* src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const String& src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const WideString& src)
{
#if 0
  MemoryDiff length = Unicode::UCS4ToUTF8(nullptr, src.native(), src.getLength());
  if (static_cast<MemorySize>(length) <= getArraySize(tiny)) {
    Unicode::UCS4ToUTF8(reinterpret_cast<uint8*>(&tiny), src.native(), src.getLength());
  }
#endif
  
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const AnyValue& src)
{
  String temp(src.getString());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(FormatOutputStream& src)
{
  String temp(src.toString());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(UTF8Stringify&& move)
{
  if (move.buffer) { // owned
    buffer = moveObject(move.buffer);
    span = move.span;
    move.span = ConstSpan<char>();
    return;
  }

  if (isUsingTiny()) {
    const char* end = copyTo(tiny, move.span);
    span = ConstSpan<char>(tiny, end);
    return;
  }

  span = move.span; // not owned
}

UTF8Stringify& UTF8Stringify::operator=(UTF8Stringify&& move)
{
  if (move.buffer) { // owned
    buffer = moveObject(move.buffer);
    span = move.span;
    move.span = ConstSpan<char>();
    return *this;
  }

  if (isUsingTiny()) {
    const char* end = copyTo(tiny, move.span);
    span = ConstSpan<char>(tiny, end);
    return *this;
  }

  span = move.span; // not owned
  return *this;
}

void UTF8Stringify::ensureOwnership()
{
  if (!buffer) { // not owned
    if (isUsingTiny()) {
      return; // we are using the tiny buffer so nothing to do
    }
    if (span.getSize() <= getArraySize(tiny)) { // use tiny if possible
      const char* end = copyTo(tiny, span);
      span = ConstSpan<char>(tiny, end);
      return;
    }
    // we cant differentiate string literals from dynamic strings
    String temp(span.begin(), span.getSize());
    buffer = temp.getContainer();
    span = temp.getSpan();
  }
}

void UTF8Stringify::setString(const StringOutputStream& src)
{
  const String& s = src.getString();
  buffer = s.getContainer();
  span = s.getSpan();
}

void UTF8Stringify::setString(const String& src)
{
  buffer = src.getContainer();
  span = src.getSpan();
}

Reference<ReferenceCountedAllocator<char> > UTF8Stringify::getStringBuffer() const
{
  if (buffer) {
    return buffer;
  } else {
    // we could update stringify here also if not const
    Reference<ReferenceCountedAllocator<char> > result = new ReferenceCountedAllocator<char>(span.getSize() + 1);
    char* dest = result->getElements();
    copy(dest, span.begin(), span.getSize());
    dest[span.getSize()] = '\0';
    return result;
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
