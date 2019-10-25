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
#include <base/Functor.h>
#include <base/mem/DynamicMemory.h>
#include <base/collection/Array.h>
#include <base/UnitTest.h>
#include <locale>
#include <codecvt>
#include <string.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: make sure to force copy on any change

void String::initialize(const char* src, MemorySize length)
{
  if (length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  auto e = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  auto dest = e->getElements();
  base::copy<char>(dest, src, length); // no overlap
  dest[length] = Traits::TERMINATOR;
  elements = e;
}

String::String() noexcept
  : elements(DEFAULT_STRING.elements)
{
  ASSERT(elements); // we never want a nullptr
}

String::String(Default d)
{
  // force non-nullptr elements
  auto e = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

String::String(MemorySize capacity)
{
  if (capacity == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  auto e = new ReferenceCountedCapacityAllocator<char>(1, capacity, GRANULARITY);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

String::String(const char* src)
{
  initialize(src, getNullTerminatedLength(src));
}

String::String(const std::string& string) throw(StringException, MemoryException)
{
  initialize(string.c_str(), string.size());
}

String::String(const std::wstring& string) throw(StringException, MemoryException)
{
  const std::string utf8 = toUTF8(string);
  initialize(utf8.c_str(), utf8.size());
}

String::String(StringOutputStream& stream)
{
  operator=(stream.toString());
}

String::String(FormatOutputStream& stream)
{
  operator=(stream.toString());
}

String& String::operator=(const char* src) throw(StringException, MemoryException)
{
  initialize(src, getNullTerminatedLength(src));
  return *this;
}

String::String(const Literal& literal) throw(StringException, MemoryException)
{
  initialize(literal.getValue(), literal.getLength());
}

String::String(const NativeString& src)
{
  initialize(src.getValue(), src.getLength());
}

String::String(const NativeString& src, MemorySize maximum) throw(StringException, MemoryException)
{
  bassert(maximum <= MAXIMUM_LENGTH, StringException(this));
  initialize(src.getValue(), getNullTerminatedLength(src.getValue(), maximum));
}

String& String::operator=(const Literal& literal) throw(StringException, MemoryException) 
{
  initialize(literal.getValue(), literal.getLength());
  return *this;
}

String& String::operator=(const NativeString& src) throw(StringException, MemoryException)
{
  initialize(src.getValue(), src.getLength());
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

void String::ensureCapacity(MemorySize capacity)
{
  if (!elements.isMultiReferenced()) {
    elements->ensureCapacity(capacity);
    return;
  }
  
  MemorySize length = getLength();
  auto e = new ReferenceCountedCapacityAllocator<char>(length + 1, capacity, GRANULARITY);
  base::copy<char>(e->getElements(), elements->getElements(), length + 1); // no overlap
  elements = e;
}

char* String::getBuffer()
{
  elements.copyOnWrite();
  return elements->getElements();
}

char* String::getBuffer(MemorySize length) throw(StringException, MemoryException)
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
  auto e = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY); // reset capacity
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

void String::garbageCollect() throw()
{
  elements->garbageCollect(); // no need to do copyOnWrite
}

MemorySize String::getGranularity() const noexcept
{
  return elements->getGranularity();
}

void String::setGranularity(MemorySize granularity) throw()
{
  elements->setGranularity(granularity);
}

void String::forceToLength(MemorySize length) throw(StringException, MemoryException)
{
  const MemorySize originalLength = getLength();
  setLength(length);
  if (originalLength < length) {
    fill(getBuffer() + originalLength, length - originalLength, Traits::TERMINATOR);
  }
}

char String::getAt(MemorySize index) const throw(OutOfRange)
{
  bassert(index < getLength(), OutOfRange(this));
  return getBuffer()[index];
}

void String::setAt(MemorySize index, char value) throw(OutOfRange)
{
  bassert(index < getLength(), OutOfRange(this));
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

String& String::insert(
  MemorySize index, char ch) throw(StringException, MemoryException)
{
  return insert(index, MemorySpan(&ch, 1));
}

String& String::insert(
  MemorySize index,
  const String& src) throw(StringException, MemoryException)
{
  if (getLength() == 0) {
    elements = src.elements; // copy string
    return *this;
  }
  return insert(index, MemorySpan(src.getElements(), src.getLength()));
}

String& String::insert(MemorySize index, const MemorySpan& span) throw(StringException, MemoryException)
{
  const MemorySize length = getLength();
  const MemorySize newLength = length + span.getSize();
  auto buffer = getBuffer(newLength);
  if (index >= length) {
    // insert section at end of string
    span.copyTo(buffer + length);
  } else {
    // insert section in middle or beginning of string
    move<char>(
      buffer + index + span.getSize(),
      buffer + index,
      length - index
    );
    span.copyTo(buffer + index);
  }
  buffer[newLength] = Traits::TERMINATOR;
  return *this;
}

String& String::insert(MemorySize index, const Literal& literal) throw(StringException, MemoryException)
{
  return insert(index, MemorySpan(literal.getValue(), literal.getLength()));
}

String& String::insert(
  MemorySize index,
  const NativeString& src) throw(StringException, MemoryException)
{
  return insert(index, MemorySpan(src.getValue(), src.getLength()));
}

String& String::append(const Literal& literal) throw(StringException, MemoryException)
{
  return append(MemorySpan(literal.getValue(), literal.getLength()));
}

String& String::append(const Literal& literal, MemorySize maximum) throw(StringException, MemoryException)
{
  bassert(maximum <= MAXIMUM_LENGTH, StringException(this));
  return append(MemorySpan(literal.getValue(), minimum(literal.getLength(), maximum)));
}

String& String::append(const NativeString& string, MemorySize maximum) throw(StringException, MemoryException)
{
  bassert(maximum <= MAXIMUM_LENGTH, StringException(this));
  return append(MemorySpan(string.getValue(), getNullTerminatedLength(string.getValue(), maximum)));
}

String& String::append(const MemorySpan& src) throw(StringException, MemoryException)
{
  if (src.isProper()) {
    const MemorySize suffixLength = src.getSize();
    if (suffixLength > 0) {
      const MemorySize length = getLength();
      const MemorySize newLength = length + suffixLength;
      auto buffer = getBuffer(newLength);
      src.copyTo(reinterpret_cast<uint8*>(buffer) + length);
      buffer[newLength] = Traits::TERMINATOR;
    }
  }
  return *this;
}

String& String::replace(
  MemorySize start,
  MemorySize end,
  const String& string) throw(StringException, MemoryException)
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

MemorySize String::replaceAll(
  const String& fromStr,
  const String& toStr) throw(StringException, MemoryException) {
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
  if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    ASSERT((0 <= start) && (start < end) && (end <= getLength()));
    const MemorySize lengthOfSubstring = end - start;
    String result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    base::copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
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

String& String::reverse() throw()
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

String& String::toLowerCase() throw()
{
  transform<char>(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

String& String::toUpperCase() throw()
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

int String::compareToIgnoreCase(const char* left, const char* right) throw()
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

int String::compareToIgnoreCase(const NativeString& string) const throw(StringException)
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
  MemorySize length = getLength();
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
  MemorySize result = 0;
  MemorySize count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

MemorySize String::count(const String& string, MemorySize start) const noexcept
{
  MemorySize result = 0;
  MemorySize count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

char* String::getElements() throw()
{
  auto result = getBuffer(); // copy on write
  ASSERT(result[getLength()] == Traits::TERMINATOR);
  result[getLength()] = Traits::TERMINATOR; // TAG: remove this when ready
  return result;
}

String& String::trim(char character) throw()
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
    String::ReadIterator i = substring.getBeginReadIterator();
    MemorySize current = length;
    while (current) {
      --current;
      skip[static_cast<uint8>(*i++)] = current;
    }
  }
  
  String::ReadIterator i = getBeginReadIterator() + start;
  const String::ReadIterator end = getEndReadIterator();
  const String::ReadIterator substringBegin = substring.getBeginReadIterator();
  String::ReadIterator j = substring.getEndReadIterator();
  j -= 1;
  i += length - 1;
  while (i < end) {
    if (*i != *j) { // check end matches
      i += skip[static_cast<uint8>(*i)]; // could skip pass end
    } else {
      String::ReadIterator k = i - 1;
      String::ReadIterator l = j - 1;
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

template<>
int compare<String>(const String& left, const String& right) throw()
{
  return left.compareTo(right);
}

unsigned long Hash<String>::operator()(const String& value) throw()
{
  const char* src = value.getElements();
  const char* end = src + value.getLength();
  unsigned long result = 5381;
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned char>(*src++);
  }
  return result;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException)
{
  stream.addCharacterField(value.getElements(), value.getLength());
  return stream;
}

/** Converts wstring to UTF-8 string. */
std::string toUTF8(const String& s)
{
  return std::string(s.getElements(), s.getLength());
}

/** Converts wstring to UTF-8 string. */
std::string toUTF8(const std::wstring& s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.to_bytes(s.c_str(), s.c_str() + s.size());
}

/** Converts wstring to UTF-8 string. */
std::string toUTF8(const wchar* s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.to_bytes(s);
}

std::string toUTF8(const wchar* s, MemorySize length)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.to_bytes(s, s + length);
}

/** Converts UTF-8 string to wstring. */
std::wstring toWide(const std::string& s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s.c_str(), s.c_str() + s.size());
}

/** Converts String to wstring. */
std::wstring toWide(const String& s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s.getElements(), s.getElements() + s.getLength());
}

/** Converts UTF-8 (const char*) to wstring. */
std::wstring toWide(const char* s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s);
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(String) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    String a;
    TEST_ASSERT(a.isEmpty());
    String b("literal");
    a = "qwerty";
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
