/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/String.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>
#include <base/mem/DynamicMemory.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void String::initialize(
  const char* string, unsigned int length) throw(MemoryException) {
  elements =
    new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  copy<char>(elements->getElements(), string, length); // no overlap
}

String::String() throw() : elements(DEFAULT_STRING.elements) {
}

String::String(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

String::String(
  const StringLiteral& string) throw(StringException, MemoryException)
  : elements(0) {
  unsigned int length = string.getLength();
  // assert(length <= MAXIMUM_LENGTH, StringException(this)); // not required
  elements =
    new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  copy<char>(elements->getElements(), string, length); // no overlap
}

String::String(const NativeString& string) throw(MemoryException)
  : elements(0) {
  if (string.getValue()) {
    int numberOfCharacters = getLengthOfMustBeTerminated(string.getValue());
    elements = new ReferenceCountedCapacityAllocator<char>(
      numberOfCharacters + 1,
      GRANULARITY
    );
    // no overlap
    copy(elements->getElements(), string.getValue(), numberOfCharacters);
  } else {
    elements = DEFAULT_STRING.elements;
  }
}

String::String(
  const NativeString& string,
  unsigned int maximum) throw(StringException, MemoryException)
  : elements(0) {
  if (string.getValue()) {
    assert(maximum <= MAXIMUM_LENGTH, StringException(this));
    const char* terminator =
      find(string.getValue(), maximum, Traits::TERMINATOR);
    int numberOfCharacters =
      terminator ? (terminator - string.getValue()) : maximum;
    elements = new ReferenceCountedCapacityAllocator<char>(
      numberOfCharacters + 1,
      GRANULARITY
    );
    copy(elements->getElements(), string.getValue(), numberOfCharacters);
  } else {
    elements = DEFAULT_STRING.elements;
  }
}

String& String::operator=(
  const StringLiteral& string) throw(StringException, MemoryException) {
  unsigned int length = string.getLength();
  // assert(length <= MAXIMUM_LENGTH, StringException(this)); // not required
  elements =
    new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  copy<char>(elements->getElements(), string, length); // no overlap
  return *this;
}

String& String::operator=(
  const char* string) throw(StringException, MemoryException) {
  if (string) {
    int numberOfCharacters = getLengthOfMustBeTerminated(string);
    elements = new ReferenceCountedCapacityAllocator<char>(numberOfCharacters + 1, GRANULARITY);
    copy(elements->getElements(), string, numberOfCharacters); // no overlap
  } else {
    elements = DEFAULT_STRING.elements;
  }
  return *this;
}

bool String::isASCII() const throw() {
  const char* i = getBuffer();
  const char* end = i + getLength();
  while (i < end) {
    if (!ASCIITraits::isASCII(*i++)) {
      return false;
    }
  }
  return true;
}

void String::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  elements->ensureCapacity(capacity); // no need to do copyOnWrite
}

void String::optimizeCapacity() throw() {
  elements->optimizeCapacity(); // no need to do copyOnWrite
}

unsigned int String::getGranularity() const throw() {
  return elements->getGranularity();
}

void String::setGranularity(unsigned int granularity) throw() {
  elements->setGranularity(granularity);
}

void String::forceToLength(
  unsigned int length) throw(StringException, MemoryException) {
  setLength(length);
}

char String::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < getLength(), OutOfRange(this));
  return getBuffer()[index];
}

void String::setAt(unsigned int index, char value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange(this));
  if (value != TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

String& String::remove(
  unsigned int start, unsigned int end) throw(MemoryException) {
  unsigned int length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      char* buffer = getBuffer(); // we are about to modify the buffer
      move(buffer + start, buffer + end, length - end); // move end of string
      elements->setSize(length - (end - start) + 1); // remember space for terminator
    }
  }
  return *this;
}

String& String::removeFrom(unsigned int start) throw(MemoryException) {
  if (start < getLength()) { // protect against some cases
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(start + 1); // remove section from end of string
  }
  return *this;
}

String& String::insert(
  unsigned int index, char ch) throw(StringException, MemoryException) {
  unsigned int length = getLength();
  setLength(length + 1);
  char* buffer = elements->getElements();
  if (index >= length) {
    buffer[length] = ch; // insert section at end of string
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, length - index);
    buffer[index] = ch;
  }
  return *this;
}

String& String::insert(
  unsigned int index,
  const String& string) throw(StringException, MemoryException) {
  unsigned int length = getLength();
  if (length == 0) {
    elements = string.elements;
  } else {
    int strlength = string.getLength();
    setLength(length + strlength); // TAG: also protects against self insertion - but can this be circumvented
    char* buffer = elements->getElements();
    if (index >= length) {
      // insert section at end of string
      copy(buffer + length, string.getBuffer(), strlength);
    } else {
      // insert section in middle or beginning of string
      move(buffer + index + strlength, buffer + index, length - index); // move end of string
      copy(buffer + index, string.getBuffer(), strlength);
    }
  }
  return *this;
}

String& String::insert(
  unsigned int index,
  const StringLiteral& string) throw(StringException, MemoryException) {
  unsigned int length = getLength();
  setLength(length + string.getLength());
  char* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy<char>(buffer + length, string, string.getLength());
  } else {
    // insert section in middle or beginning of string
    move<char>(buffer + index + string.getLength(), buffer + index, length - index);
    copy<char>(buffer + index, string, string.getLength());
  }
  return *this;
}

String& String::insert(
  unsigned int index,
  const char* string) throw(StringException, MemoryException) {
  int strlength = 0;
  if (string) { // is string proper (not empty)
    const char* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR);
    assert(terminator, StringException(this));
    strlength = terminator - string;
  }
  unsigned int length = getLength();
  setLength(length + strlength);
  char* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, string, strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, length - index);
    copy(buffer + index, string, strlength);
  }
  return *this;
}

String& String::append(
  const StringLiteral& string) throw(StringException, MemoryException) {
  unsigned int length = getLength();
  setLength(length + string.getLength());
  char* buffer = elements->getElements();
  copy<char>(buffer + length, string, string.getLength());
  return *this;
}

String& String::append(
  const StringLiteral& string,
  unsigned int maximum) throw(StringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, StringException(this));
  const unsigned int length = getLength();
  setLength(length + minimum(string.getLength(), maximum));
  char* buffer = elements->getElements();
  copy<char>(buffer + length, string, minimum(string.getLength(), maximum));
  return *this;
}

String& String::append(
  const char* string,
  unsigned int maximum) throw(StringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, StringException(this));
  const unsigned int suffixLength = getLengthOfTerminated(string, maximum);
  const unsigned int length = getLength();
  setLength(length + suffixLength);
  char* buffer = elements->getElements();
  copy(buffer + length, string, suffixLength);
  return *this;
}

String& String::replace(
  unsigned int start,
  unsigned int end,
  const String& string) throw(StringException, MemoryException) {
  unsigned int length = getLength();
  unsigned int strlength = string.getLength();
  unsigned int lengthAfterRemove = length;
  bool moveEnd = false;
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      lengthAfterRemove = start;
    } else {
      lengthAfterRemove = length - (end - start);
      moveEnd = true;
    }
  }
  unsigned int finalLength = lengthAfterRemove + strlength;

  if (length < finalLength) { // is resulting string longer
    setLength(finalLength);
  } else {
    elements.copyOnWrite();
  }
  char* buffer = elements->getElements();

  if (moveEnd) {
    move(buffer + start + strlength, buffer + end, length - end); // move end of string
  }

  copy(buffer + ((start < length) ? start : length), string.getBuffer(), strlength);

  if (length > finalLength) { // is resulting string shorter
    setLength(finalLength);
  }
  return *this;
}

unsigned int String::replaceAll(
  const String& fromStr,
  const String& toStr) throw(StringException, MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength(), toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

String String::substring(
  unsigned int start, unsigned int end) const throw(MemoryException) {
  unsigned int length = getLength();
  if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    // 0 <= start < end <= getLength()
    unsigned int lengthOfSubstring = end - start;
    String result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    return String(); // return empty string
  }
}

String& String::operator-=(const String& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

String& String::reverse() throw() {
  char* p = getBuffer();
  char* q = &p[getLength() - 1]; // last char - empty string => q < p
  char temp;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

String& String::toLowerCase() throw() {
  transform<char>(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

String& String::toUpperCase() throw() {
  transform<char>(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int String::compareTo(const String& string) const throw() {
  // both strings may contain multiple zeros
  unsigned int leftLength = getLength();
  unsigned int rightLength = string.getLength();
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

int String::compareTo(const StringLiteral& string) const throw() {
  const char* left = getElements();
  const char* right = string;
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

int String::compareTo(const char* string) const throw() {
  if (!string) {
    return false;
  }
  const char* left = getElements();
  const char* right = string;
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

int String::compareToIgnoreCase(const char* left, const char* right) throw() {
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

int String::compareToIgnoreCase(const String& string) const throw() {
  return compareToIgnoreCase(getElements(), string.getElements());
}

int String::compareToIgnoreCase(
  const char* string) const throw(StringException) {
  assert(string, StringException(this));
  return compareToIgnoreCase(getElements(), string);
}

bool String::startsWith(const String& prefix) const throw() {
  unsigned int prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool String::startsWith(const StringLiteral& prefix) const throw() {
  unsigned int prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare<char>(getBuffer(), prefix, prefixLength) == 0);
}

bool String::endsWith(const String& suffix) const throw() {
  unsigned int length = getLength();
  unsigned int suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool String::endsWith(const StringLiteral& suffix) const throw() {
  unsigned int length = getLength();
  unsigned int suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare<char>(getBuffer() + length - suffixLength, suffix, suffixLength) == 0);
}

int String::indexOf(char ch, unsigned int start) const throw() {
  unsigned int length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const char* buffer = getBuffer();
  unsigned int count = length - start;

  const char* result = find(buffer + start, count, ch);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

int String::indexOf(const String& string, unsigned int start) const throw() {
  unsigned int length = getLength();
  unsigned int sublength = string.getLength();

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
  unsigned int count = length - sublength + 1; // number of characters left - minimum is 1
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

int String::lastIndexOf(char ch, unsigned int start) const throw() {
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

int String::lastIndexOf(
  const String& string, unsigned int start) const throw() {
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
  unsigned int count = string.getLength();
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

unsigned int String::count(char ch, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

unsigned int String::count(
  const String& string, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

char* String::getElements() throw() {
  char* result = getBuffer(); // copy on write
  result[getLength()] = Traits::TERMINATOR;
  return result;
}

String& String::trim(char character) throw() {
  ReadIterator begin = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  unsigned int length = getLength();
  bool modified = false;
  
  ReadIterator last = end;
  --last;
  if (*last == character) {
    modified = true;
    while ((begin != last) && (*last == character)) {
      --last;
    }
    elements.copyOnWrite(); // we are about to modify the buffer
    length = last - begin + 1;
  }
  end = last + 1;
  
  ReadIterator i = begin;
  if (*i == character) {
    modified = true;
    while ((i < end) && (*i == character)) {
      ++i;
    }
    char* buffer = getBuffer(); // we are about to modify the buffer
    move(buffer, buffer + (i - begin), length - (i - begin)); // move end of string
    length -= (i - begin);
  }
  if (modified) {
    elements->setSize(length + 1); // remember space for terminator
  }
  return *this;
}

Array<String> String::split(
  char separator, bool group) throw(MemoryException) {
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
int compare<String>(const String& left, const String& right) throw() {
  return left.compareTo(right);
}

unsigned long Hash<String>::operator()(const String& value) throw() {
  const char* src = value.getBytes();
  const char* end = src + value.getLength();
  unsigned long result = 5381;
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned char>(*src++);
  }
  return result;
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const String& value) throw(IOException) {
  stream.addCharacterField(value.getBytes(), value.getLength());
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
