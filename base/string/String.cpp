/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>
#include <base/Functor.h>
#include <base/collection/Array.h>
#include <base/UnitTest.h>
#include <base/security/Base64.h>
#include <string.h>

// C++: nice if we can get header with only forward declaration of basic_string/string/wstring/...

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: make sure to force copy on any change

void String::initialize(const char* src, MemorySize length)
{
  if (length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  auto e = new ReferenceCountedAllocator<char>(length + 1);
  auto dest = e->getElements();
  base::copy<char>(dest, src, length); // no overlap
  dest[length] = Traits::TERMINATOR;
  elements = e;
}

void String::initialize(const wchar* src, MemorySize _length)
{
  if (_length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  const MemorySize length = Unicode::WCharToUTF8(nullptr, src, _length);
  auto e = new ReferenceCountedAllocator<char>(length + 1);
  auto dest = e->getElements();
  Unicode::WCharToUTF8(reinterpret_cast<uint8*>(dest), src, _length);
  dest[length] = Traits::TERMINATOR;
  elements = e;
}

void String::initialize(const char16_t* src, MemorySize _length)
{
  if (_length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  const MemorySize length = Unicode::UTF16ToUTF8(nullptr, reinterpret_cast<const utf16*>(src), _length);
  auto e = new ReferenceCountedAllocator<char>(length + 1);
  auto dest = e->getElements();
  Unicode::UTF16ToUTF8(reinterpret_cast<uint8*>(dest), reinterpret_cast<const utf16*>(src), _length);
  dest[length] = Traits::TERMINATOR;
  elements = e;
}

void String::initialize(const ucs4* src, MemorySize _length)
{
  if (_length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  // TAG: add method to validate WideString codes also
  const MemoryDiff length = Unicode::UCS4ToUTF8(nullptr, src, _length);
  if (length < 0) {
    _throw StringException("Invalid UCS4 string.");
  }

  auto e = new ReferenceCountedAllocator<char>(length + 1);
  auto dest = e->getElements();
  Unicode::UCS4ToUTF8(reinterpret_cast<uint8*>(dest), src, _length);
  dest[length] = Traits::TERMINATOR;
  elements = e;
}

String::String() noexcept
  : elements(DEFAULT_STRING.elements)
{
  BASSERT(elements); // we never want a nullptr
}

String::String(Default d)
{
  // force non-nullptr elements
  auto e = new ReferenceCountedAllocator<char>(1);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

String::String(MemorySize capacity)
{
  if (capacity == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  auto e = new ReferenceCountedAllocator<char>(1, capacity);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

#if 0
String::String(char src)
{
  initialize(&src, 1);
}
#endif

String::String(const char* src)
{
  initialize(src, getNullTerminatedLength(src));
}

String::String(const char* src, MemorySize length)
{
  initialize(src, length);
}

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
String::String(const char8_t* src)
{
  initialize(reinterpret_cast<const char*>(src), getNullTerminatedLength(src));
}

String::String(const char8_t* src, MemorySize length)
{
  initialize(reinterpret_cast<const char*>(src), length);
}
#endif

String::String(const wchar* src)
{
  initialize(src, getNullTerminatedLength(src));
}

String::String(const wchar* src, MemorySize length)
{
  initialize(src, length);
}

String::String(const char16_t* src)
{
  initialize(src, getNullTerminatedLength(src));
}

String::String(const char16_t* src, MemorySize length)
{
  initialize(src, length);
}

String::String(const char32_t* src)
{
  initialize(src, getNullTerminatedLength(src));
}

String::String(const char32_t* src, MemorySize length)
{
  initialize(src, length);
}

String::String(const Literal& src)
{
  initialize(src.getValue(), src.getLength());
}

String::String(const NativeString& src)
{
  initialize(src.getValue(), src.getLength());
}

String::String(const WideString& src)
{
  initialize(src.native(), src.getLength());
}

String::String(StringOutputStream& stream)
{
  operator=(stream.toString());
}

String::String(FormatOutputStream& stream)
{
  operator=(stream.toString());
}

String::String(const NativeString& src, MemorySize maximum)
{
  initialize(src.getValue(), getNullTerminatedLength(src.getValue(), maximum));
}

String& String::operator=(const Literal& assign)
{
  initialize(assign.getValue(), assign.getLength());
  return *this;
}

String& String::operator=(const WideLiteral& assign)
{
  initialize(assign.getValue(), assign.getLength());
  return *this;
}

String& String::operator=(const NativeString& assign)
{
  initialize(assign.getValue(), assign.getLength());
  return *this;
}

String& String::operator=(const char* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

String& String::operator=(const wchar* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

String& String::operator=(const char16_t* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

String& String::operator=(const char32_t* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

String& String::operator=(const WideString& assign)
{
  initialize(assign.getElements(), assign.getLength());
  return *this;
}

String& String::operator=(const StringOutputStream& assign)
{
  operator=(assign.getString());
  return *this;
}

bool String::isASCII() const noexcept
{
  const char* i = getBuffer();
  const char* end = i + getLength();
  while (i < end) {
    if (!ASCIITraits::isASCII(*i++)) {
      return false;
    }
  }
  return true;
}

bool String::isMultiReferenced() const noexcept
{
  return elements.isMultiReferenced();
}

void String::ensureCapacity(MemorySize capacity)
{
  if (!elements.isMultiReferenced()) {
    elements->ensureCapacity(capacity);
    return;
  }
  
  MemorySize length = getLength();
  auto e = new ReferenceCountedAllocator<char>(length + 1, capacity);
  base::copy<char>(e->getElements(), elements->getElements(), length + 1); // no overlap
  elements = e;
}

char* String::getBuffer()
{
  elements.copyOnWrite();
  return elements->getElements();
}

char* String::getBuffer(MemorySize length)
{
  bassert(
    length <= MAXIMUM_LENGTH,
    StringException(Type::getType<String>())
  );

  const MemorySize originalLength = getLength();

  if (!elements.isMultiReferenced()) { // just resize
    if (length == originalLength) { // nothing to do
      return elements->getElements();
    }
    elements->setSize(length + 1);
    auto dest = elements->getElements();
    dest[length] = Traits::TERMINATOR;
    return dest;
  }
  
  if (length == originalLength) { // just copy
    elements.copyOnWrite(); // we are about to modify the buffer
    return elements->getElements();
  }

  // resize and copy
  auto e = new ReferenceCountedAllocator<char>(length + 1); // reset capacity
  auto dest = e->getElements();
  if (length < originalLength) {
    base::copy<char>(dest, elements->getElements(), length); // no overlap
  } else {
    base::copy<char>(dest, elements->getElements(), originalLength); // no overlap
  }
  dest[length] = Traits::TERMINATOR;
  elements = e;

  return dest;
}

bool String::hasNull() const noexcept
{
  const char* i = getBuffer();
  const char* end = i + getLength();
  while (i != end) {
    if (*i == 0) {
      return true;
    }
    ++i;
  }
  return false;
}

void String::clear()
{
  elements = DEFAULT_STRING.elements;
}

String String::copy() const
{
  String result = *this;
  result.getBuffer();
  return result;
}

void String::garbageCollect()
{
  elements->garbageCollect(); // no need to do copyOnWrite
}

void String::forceToLength(MemorySize length)
{
  const MemorySize originalLength = getLength();
  setLength(length);
  if (originalLength < length) {
    auto ch = Traits::TERMINATOR;
    fill(getBuffer() + originalLength, length - originalLength, ch);
  }
}

char String::getAt(MemorySize index) const
{
  if (index >= getLength()) {
    _throw OutOfRange(this);
  }
  return getBuffer()[index];
}

void String::setAt(MemorySize index, char value)
{
  if (index >= getLength()) {
    _throw OutOfRange(this);
  }
  getBuffer()[index] = value; // allow terminated within string
}

String& String::remove(MemorySize start, MemorySize end)
{
  const MemorySize length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      auto buffer = getBuffer(); // we are about to modify the buffer
      move(buffer + start, buffer + end, length - end); // move end of string
      elements->setSize(length - (end - start) + 1); // remember space for terminator
    }
    elements->getElements()[getLength()] = Traits::TERMINATOR;
  }
  return *this;
}

String& String::removeFrom(MemorySize start)
{
  if (start < getLength()) { // protect against some cases
    getBuffer(start); // just resize
  }
  return *this;
}

String& String::insert(MemorySize index, char ch)
{
  return insert(index, ConstSpan<char>(&ch, 1));
}

String& String::insert(MemorySize index, const String& src)
{
  if (getLength() == 0) {
    elements = src.elements; // copy string
    return *this;
  }
  return insert(index, src.getSpan());
}

String& String::insert(MemorySize index, const ConstSpan<char>& src)
{
  const MemorySize length = getLength();
  const MemorySize newLength = length + src.getSize();
  auto buffer = getBuffer(newLength);
  if (index >= length) {
    // insert section at end of string
    copyTo(buffer + length, src);
  } else {
    // insert section in middle or beginning of string
    move<char>(
      buffer + index + src.getSize(),
      buffer + index,
      length - index
    );
    copyTo(buffer + index, src);
  }
  buffer[newLength] = Traits::TERMINATOR;
  return *this;
}

String& String::insert(MemorySize index, const Literal& literal)
{
  return insert(index, ConstSpan<char>(literal.getValue(), literal.getLength()));
}

String& String::insert(MemorySize index, const NativeString& src)
{
  return insert(index, ConstSpan<char>(src.getValue(), src.getLength()));
}

String& String::append(const Literal& literal)
{
  return append(ConstSpan<char>(literal.getValue(), literal.getLength()));
}

String& String::append(const Literal& literal, MemorySize maximum)
{
  return append(ConstSpan<char>(literal.getValue(), minimum(literal.getLength(), maximum)));
}

String& String::append(const NativeString& string, MemorySize maximum)
{
  return append(ConstSpan<char>(string.getValue(), getNullTerminatedLength(string.getValue(), maximum)));
}

String& String::append(ucs4 ch)
{
  uint8 buffer[6];
  MemoryDiff length = Unicode::writeUTF8(buffer, ch);
  return append(ConstSpan<char>(reinterpret_cast<const char*>(buffer), length));
}

String& String::append(const ConstSpan<char>& src)
{
  if (src) {
    const MemorySize suffixLength = src.getSize();
    if (suffixLength > 0) {
      const MemorySize length = getLength();
      const MemorySize newLength = length + suffixLength;
      auto buffer = getBuffer(newLength);
      copyTo(buffer + length, src);
      buffer[newLength] = Traits::TERMINATOR;
    }
  }
  return *this;
}

String& String::replace(MemorySize start, MemorySize end, const String& string)
{
  MemorySize length = getLength();
  MemorySize strlength = string.getLength();
  MemorySize lengthAfterRemove = length;
  bool moveEnd = false;
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      lengthAfterRemove = start;
    } else {
      lengthAfterRemove = length - (end - start);
      moveEnd = true;
    }
  }
  MemorySize finalLength = lengthAfterRemove + strlength;

  if (length < finalLength) { // is resulting string longer
    setLength(finalLength);
  } else {
    elements.copyOnWrite();
  }
  auto buffer = elements->getElements();

  if (moveEnd) {
    move(buffer + start + strlength, buffer + end, length - end); // move end of string
  }

  base::copy(buffer + ((start < length) ? start : length), string.getBuffer(), strlength);

  if (length > finalLength) { // is resulting string shorter
    setLength(finalLength);
  }
  return *this;
}

MemorySize String::replaceAll(const String& fromStr, const String& toStr)
{
  MemorySize count = 0;
  MemorySize start = 0;
  MemoryDiff found = -1;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength(), toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

String String::substring(MemorySize start, MemorySize end) const
{
  const MemorySize length = getLength();
  if ((start == 0) && (end >= length)) {
    return *this; // quick copy by reference
  } else if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    BASSERT((0 <= start) && (start < end) && (end <= getLength()));
    const MemorySize lengthOfSubstring = end - start;
    String result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    base::copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    // C++: could be useful to have a "This/Auto" type to get the class automatically
    return String(); // return empty string
  }
}

String& String::operator-=(const String& suffix)
{
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

String& String::reverse() noexcept
{
  auto p = getBuffer();
  auto q = &p[getLength() - 1]; // last char - empty string => q < p
  char temp = 0;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

String& String::toLowerCase() noexcept
{
  transform<char>(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

String& String::toUpperCase() noexcept
{
  transform<char>(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int String::compareTo(const String& string) const noexcept
{
  // both strings may contain multiple zeros
  MemorySize leftLength = getLength();
  MemorySize rightLength = string.getLength();
  const char* left = getBuffer();
  const char* right = string.getBuffer();
  const char* end = left + minimum(leftLength, rightLength);
  while (left < end) {
    int temp = static_cast<int>(*left) - static_cast<int>(*right);
    if (temp != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  if (leftLength < rightLength) {
    return -1;
  } else if (leftLength > rightLength) {
    return 1;
  } else {
    return 0; // equal
  }
}

int String::compareTo(const Literal& literal) const noexcept
{
  const char* left = getElements();
  const char* right = literal.getValue();
  while (*left && *right) {
    int temp = static_cast<int>(*left) - static_cast<int>(*right);
    if (temp != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  return static_cast<int>(*left) - static_cast<int>(*right);
}

int String::compareTo(const NativeString& string) const noexcept
{
  if (!string.getValue()) {
    return false;
  }
  const char* left = getElements();
  const char* right = string.getValue();
  while (*left && *right) {
    int temp = static_cast<int>(*left) - static_cast<int>(*right);
    if (temp != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  return static_cast<int>(*left) - static_cast<int>(*right);
}

int String::compareToIgnoreCase(const char* left, const char* right) noexcept
{
  while (*left && *right) { // continue until end of any string has been reached
    if (*left != *right) { // not equal
      int result = Traits::toLower(*left) - Traits::toLower(*right);
      if (result != 0) { // not equal
        return result;
      }
    }
    ++left;
    ++right;
  }
  // possible cases: only end of 'left' (less than), only end of 'right' (greater than), end of both (equal)
  return (Traits::toLower(*left) - Traits::toLower(*right));
}

int String::compareToIgnoreCase(const String& string) const noexcept
{
  return compareToIgnoreCase(getElements(), string.getElements());
}

int String::compareToIgnoreCase(const NativeString& string) const
{
  bassert(string.getValue(), StringException(this));
  return compareToIgnoreCase(getElements(), string.getValue());
}

bool String::startsWith(const String& prefix) const noexcept
{
  const MemorySize prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool String::startsWith(const Literal& prefix) const noexcept
{
  const MemorySize prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare<char>(getBuffer(), prefix.getValue(), prefixLength) == 0);
}

bool String::endsWith(const String& suffix) const noexcept
{
  const MemorySize length = getLength();
  const MemorySize suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool String::endsWith(const Literal& suffix) const noexcept
{
  const MemorySize length = getLength();
  const MemorySize suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare<char>(getBuffer() + length - suffixLength, suffix.getValue(), suffixLength) == 0);
}

MemoryDiff String::indexOf(char ch, MemorySize start) const noexcept
{
  const MemorySize length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const auto buffer = getBuffer();
  MemorySize count = length - start;

  const char* result = find(buffer + start, count, ch);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

MemoryDiff String::indexOf(const String& string, MemorySize start) const noexcept
{
  MemorySize length = getLength();
  MemorySize sublength = string.getLength();

  if ((start >= length) || (sublength == 0) || (sublength > length)) {
    return -1; // not found
  }

  const char* substring = string.getBuffer();
  const char* buffer = getBuffer();
  const char* begin = &buffer[start]; // beginning of current substring
  const char* last = &begin[sublength - 1]; // end of current substring - last >= begin >= buffer

  // calculate hash of prefix of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const char* p = substring;
  const char* q = begin;
  while (q <= last) {
    hash = (128 * hash + *p++) % 16647143;
    match = (128 * match + *q++) % 16647143;
    if (q <= last) {
      mask = (128 * mask) % 16647143;
    }
  }

  // look for substring
  MemorySize count = length - sublength + 1; // number of characters left - minimum is 1
  while (true) {
    if (match == hash) { // possible substring found
      if (compare(begin, substring, sublength) == 0) {
        return begin - buffer; // return index of substring
      }
    }
    if (!--count) { // are we done - make sure we check for match for the last char
      break;
    }
    match = (match + 128 * 16647143 - mask * *begin) % 16647143;
    match = (match * 128 + *(++last)) % 16647143;
    ++begin;
  }

  return -1; // not found
}

MemoryDiff String::lastIndexOf(char ch, MemorySize start) const noexcept
{
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const char* buffer = getBuffer();
  const char* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return -1; // not found
}

MemoryDiff String::lastIndexOf(const String& string, MemorySize start) const noexcept
{
  if ((string.isEmpty()) || (string.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  const char* substring = string.getBuffer();
  const char* buffer = getBuffer();
  const char* begin = &buffer[start - string.getLength() + 1]; // beginning of current substring
  const char* last = &buffer[start]; // end of current substring - buffer <= begin <= last

  // calculate hash of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const char* p = substring;
  const char* q = begin;
  MemorySize count = string.getLength();
  while (count--) {
    hash = (128 * hash + *p++) % 16647143;
    match = (128 * match + *q++) % 16647143;
    if (count > 0) {
      mask = (128 * mask) % 16647143;
    }
  }

  // look for substring
  count = getLength() - string.getLength() + 1; // number of characters left - count >= 1
  while (true) {
    if (match == hash) { // possible substring found
      if (compare(begin, substring, string.getLength()) == 0) {
        return begin - buffer; // return index of substring
      }
    }
    if (!--count) { // are we done - make sure we check for match for the last char
      break;
    }
    match = (match + 128 * 16647143 - mask * *begin) % 16647143;
    match = (match * 128 + *last) % 16647143;
    --begin;
    --last;
  }

  return -1; // not found
}

MemorySize String::count(char ch, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

MemorySize String::count(const String& string, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

char* String::getElements()
{
  char* result = getBuffer(); // copy on write
  return result;
}

String& String::trim(char character)
{
  ReadIterator begin = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  if (begin == end) {
    return *this; // empty
  }

  while ((begin != end) && (*begin == character)) {
    ++begin;
  }
  if (begin == end) {
    clear();
    return *this;
  }
  
  --end;
  while ((end != begin) && (*end == character)) {
    --end;
  }
  ++end; // end of trimmed string
  
  const MemoryDiff newLength = end - begin;
  if (newLength == getLength()) {
    return *this; // nothing to do
  }
  
  if (begin == getBeginReadIterator()) { // nothing trimmed from start
    getBuffer(newLength); // just discard
  } else {
    // TAG: if only one reference then we can avoid new buffer
    *this = substring(begin - getBeginReadIterator(), end - getBeginReadIterator());
  }
  return *this;
}

// http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/index.html
MemoryDiff String::search(const String& substring, MemorySize start) const noexcept
{
  const MemorySize length = substring.getLength();
  if ((length == 0) || (getLength() < (start + length))) {
    return -1; // not found
  }
  
  MemorySize skip[256];
  {
    for (unsigned int i = 0; i < getArraySize(skip); ++i) {
      skip[i] = length; // maximum single code skip
    }
    ReadIterator i = substring.getBeginReadIterator();
    MemorySize current = length;
    while (current) {
      --current;
      skip[static_cast<uint8>(*i++)] = current;
    }
  }
  
  ReadIterator i = getBeginReadIterator() + start;
  const ReadIterator end = getEndReadIterator();
  const ReadIterator substringBegin = substring.getBeginReadIterator();
  ReadIterator j = substring.getEndReadIterator();
  j -= 1;
  i += length - 1;
  while (i < end) {
    if (*i != *j) { // check end matches
      i += skip[static_cast<uint8>(*i)]; // could skip pass end
    } else {
      ReadIterator k = i - 1;
      ReadIterator l = j - 1;
      while ((l > substringBegin) && (*k == *l)) {
        --k;
        --l;
      }
      if (l == substringBegin) { // have we found the entire substring
        return k - (getBeginReadIterator() + start);
      }
      
      i = k + skip[static_cast<uint8>(*k)]; // could skip pass end
      // TAG: need better skip - FIXME
    }
  }
  return -1; // not found
}

Array<String> String::split(char separator, bool group) const
{
  Array<String> result;
  
  const ReadIterator begin = getBeginReadIterator();
  const ReadIterator end = getEndReadIterator();
  
  ReadIterator i = begin;
  if (group) {
    while (i != end) {
      while ((i < end) && (*i == separator)) { // skip group of separators
        ++i;
      }
      if (i == end) {
        break;
      }
      ReadIterator j = i;
      while ((j < end) && (*j != separator)) {
        ++j;
      }
      result.append(substring(i - begin, j - begin));
    }
  } else {
    while (i != end) {
      if (*i == separator) {
        ++i; // skip separator
      }
      ReadIterator j = i;
      while ((j < end) && (*j != separator)) {
        ++j;
      }
      result.append(substring(i - begin, j - begin));
      if (j == end) {
        break;
      }
      i = j;
    }
  }
  return result;
}

String operator+(const String& left, const char* right)
{
  MemorySize r = getNullTerminatedLength(right);
  String result(left.getLength() + r);
  return result.append(left).append(right);
}

String operator+(const char* left, const String& right)
{
  MemorySize l = getNullTerminatedLength(left);
  String result(l + right.getLength());
  return result.append(left).append(right);
}

// TAG: use span instead to reuse implementation
String operator-(const String& left, const String& right)
{
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength()); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
}

String operator*(const String& src, unsigned int count)
{
  String result;
  if (src && (count > 0)) {
    if (count == 1) {
      return src;
    }
    result.ensureCapacity(src.getLength() * count);
    while (count--) {
      result.append(src);
    }
  }
  return result;
}

template<>
int compare<String>(const String& left, const String& right)
{
  return left.compareTo(right);
}

unsigned long Hash<String>::operator()(const String& value) noexcept
{
  const char* src = value.getElements();
  const char* end = src + value.getLength();
  unsigned long result = 5381;
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned char>(*src++);
  }
  return result;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value)
{
  stream.addCharacterField(value.getElements(), value.getLength());
  return stream;
}

MemorySize String::UTF8String::getLength() const noexcept
{
  return Unicode::getUTF8StringLength(src);
}

bool String::UTF8String::isValidUTF8() const noexcept
{
  const uint8* src = this->src;
  while (*src) {
    ucs4 ch = 0;
    auto bytes = Unicode::readUCS4(src, ch);
    if (bytes <= 0) {
      return false;
    }
    if (!Unicode::isUCS4(ch)) {
      return false;
    }
    src += bytes; // skip valid bytes
  }
  return true;
}

String String::getValidUTF8() const
{
  String result(getLength());
  const uint8* src = getBytes();
  while (*src) {
    ucs4 ch = 0;
    auto bytes = Unicode::readUCS4(src, ch);
    if (bytes > 0) {
      if (!Unicode::isUCS4(ch)) {
        src += bytes;
        continue; // skip disallowed ucs4
      }
      while (bytes > 0) {
        result.append(*src++); // copy bytes
        --bytes;
      }
    } else {
      ++src; // skip bad byte
    }
  }
  return result;
}

String String::substImpl(const UTF8Stringify& text, const UTF8Stringify* args, MemorySize numberOfArgs)
{
  Format::Subst s(text, args, numberOfArgs);
  return s.format();
}

String escape(const String& s)
{
  String stream(s.getLength() + 128);
  auto i = s.getUTF8BeginReadIterator();
  auto end = s.getUTF8EndReadIterator();
  stream << '"';
  for (; i < end; ++i) {
    const ucs4 ch = *i;
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
  return stream;
}

#if 0
String base64decode(const String& text)
{
  return Base64::decode(bytes.getBytes(), bytes.getLength());
}
#endif

String base64encode(const String& bytes)
{
  return Base64::encode(bytes.getBytes(), bytes.getLength());
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(String) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/string");
  TEST_IMPACT(CRITICAL)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    String a;
    TEST_ASSERT(!a);
    TEST_ASSERT(a.isEmpty());
    String b("literal");
    a = "qwerty";
    TEST_ASSERT(a);
    TEST_ASSERT(!a.isEmpty());
    auto c = a + b;
    TEST_ASSERT(c == "qwertyliteral");
    
    TEST_ASSERT(c.indexOf('l') == 6);
    TEST_ASSERT(c.indexOf("literal") == 6);
    TEST_ASSERT(c.lastIndexOf('y') == 5);
    TEST_ASSERT(c.lastIndexOf("literal") == 6);

    c.clear();
    c.append(String("Hello"));
    c.append(String(" "));
    c.append(String("World!"));
    TEST_ASSERT(c == "Hello World!");
    TEST_PRINT(c);
    c.reverse();
    TEST_PRINT(c);
    
    // TEST_ASSERT(c[6] == 'W');

    MemorySize count = 0;
    for (auto ch : c) {
      (void)ch; // dont care about unused
      ++count;
    }
    TEST_ASSERT(count == c.getLength());
    
    c.garbageCollect();
    
    // TEST_ASSERT((String("=*-") * 10).getLength() == 30);
  }
};

TEST_REGISTER(String);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
