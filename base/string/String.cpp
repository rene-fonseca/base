/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/String.h>
#include <base/Functor.h>
#include <string.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const String String::DEFAULT_STRING(MESSAGE(""));

String::String() throw() : elements(DEFAULT_STRING.elements) {
}

String::String(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

String::String(const StringLiteral& str) throw(StringException, MemoryException) : elements(0) {
  unsigned int length = str.getLength();
  assert(length <= MAXIMUM_LENGTH, StringException()); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), str, length); // no overlap
}

String::String(const char* string) throw(StringException, MemoryException) : elements(0) {
  assert(string, StringException()); // make sure string is proper (not empty)
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, StringException()); // maximum length exceeded
  int numberOfCharacters = terminator - string;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

String::String(const char* string, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  assert(string, StringException()); // make sure string is proper (not empty)
  const Character* terminator = find(string, maximum, Traits::TERMINATOR); // find terminator
  int numberOfCharacters = terminator ? (terminator - string) : maximum;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

String& String::operator=(const char* string) throw(StringException, MemoryException) {
  assert(string, StringException()); // make sure string is proper (not empty)
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, StringException()); // maximum length exceeded
  int numberOfCharacters = terminator - string;
  elements = new ReferenceCountedCapacityAllocator<char>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
  return *this;
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

char String::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  return getBuffer()[index];
}

void String::setAt(unsigned int index, char value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  if (value != TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

String& String::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  int length = getLength();
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

String& String::insert(unsigned int index, char ch) throw(StringException, MemoryException) {
  int length = getLength();
  setLength(length + 1);
  Character* buffer = elements->getElements();
  if (index >= length) {
    buffer[length] = ch; // insert section at end of string
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, length - index);
    buffer[index] = ch;
  }
  return *this;
}

String& String::insert(unsigned int index, const String& str) throw(StringException, MemoryException) {
  int length = getLength();
  int strlength = str.getLength();
  setLength(length + strlength); // TAG: also protects against self insertion - but can this be circumvented
  Character* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, str.getBuffer(), strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, length - index); // move end of string
    copy(buffer + index, str.getBuffer(), strlength);
  }
  return *this;
}

String& String::insert(unsigned int index, const StringLiteral& str) throw(StringException, MemoryException) {
  int length = getLength();
  setLength(length + str.getLength());
  Character* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy<Character>(buffer + length, str, str.getLength());
  } else {
    // insert section in middle or beginning of string
    move<Character>(buffer + index + str.getLength(), buffer + index, length - index);
    copy<Character>(buffer + index, str, str.getLength());
  }
  return *this;
}

String& String::insert(unsigned int index, const char* str) throw(StringException, MemoryException) {
  int strlength = 0;
  if (str) { // is string proper (not empty)
    const Character* terminator = find(str, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
    assert(terminator, StringException()); // maximum length exceeded
    strlength = terminator - str;
  }
  int length = getLength();
  setLength(length + strlength);
  Character* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, str, strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, length - index);
    copy(buffer + index, str, strlength);
  }
  return *this;
}

String& String::append(const StringLiteral& str, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  int length = getLength();
  setLength(length + str.getLength());
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, str.getLength());
  return *this;
}

String& String::append(const char* str, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  assert(str, StringException()); // make sure string is proper (not empty)
  const Character* terminator = find(str, maximum, Traits::TERMINATOR); // find terminator
  int strlength = terminator ? (terminator - str) : maximum;
  int length = getLength();
  setLength(length + strlength);
  Character* buffer = elements->getElements();
  copy(buffer + length, str, strlength);
  return *this;
}

String& String::replace(unsigned int start, unsigned int end, const String& str) throw(StringException, MemoryException) {
  int length = getLength();
  int strlength = str.getLength();
  int lengthAfterRemove = length;
  bool moveEnd = false;
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      lengthAfterRemove = start;
    } else {
      lengthAfterRemove = length - (end - start);
      moveEnd = true;
    }
  }
  int finalLength = lengthAfterRemove + strlength;

  if (length < finalLength) { // is resulting string longer
    setLength(finalLength);
  } else {
    elements.copyOnWrite();
  }
  Character* buffer = elements->getElements();

  if (moveEnd) {
    move(buffer + start + strlength, buffer + end, length - end); // move end of string
  }

  copy(buffer + ((start < length) ? start : length), str.getBuffer(), strlength);

  if (length > finalLength) { // is resulting string shorter
    setLength(finalLength);
  }
  return *this;
}

unsigned int String::replaceAll(const String& fromStr, const String& toStr) throw(StringException, MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength(), toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

String String::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  int length = getLength();
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

//char* String::substring(char* buffer, unsigned int start, unsigned int end) const throw() {
//  if (buffer) {
//    if ((start <= end) && (start < getLength())) {
//      if (end >= getLength()) {
//        end = getLength() - 1; // index of last char in this string
//      }
//      // 0 <= start <= end < getLength()
//      unsigned int lengthOfSubstring = end - start + 1;
//      copy(buffer, getBuffer() + start, lengthOfSubstring); // buffers do not overlap
//      buffer[lengthOfSubstring] = TERMINATOR;
//    } else {
//      *buffer = TERMINATOR;
//    }
//  }
//  return buffer;
//}

String& String::toLowerCase() throw() {
  transform<char>(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

String& String::toUpperCase() throw() {
  transform<char>(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int String::compareTo(const String& str) const throw() {
  return strcmp(getElements(), str.getElements());
}

int String::compareTo(const char* str) const throw(StringException) {
  assert(str, StringException());
  return strcmp(getElements(), str);
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

int String::compareToIgnoreCase(const String& str) const throw() {
  return compareToIgnoreCase(getElements(), str.getElements());
}

int String::compareToIgnoreCase(const char* str) const throw(StringException) {
  assert(str, StringException());
  return compareToIgnoreCase(getElements(), str);
}

bool String::startsWith(const String& prefix) const throw() {
  int prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) && (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool String::startsWith(const StringLiteral& prefix) const throw() {
  int prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) && (compare<Character>(getBuffer(), prefix, prefixLength) == 0);
}

bool String::endsWith(const String& suffix) const throw() {
  int length = getLength();
  int suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) && (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool String::endsWith(const StringLiteral& suffix) const throw() {
  int length = getLength();
  int suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) && (compare<Character>(getBuffer() + length - suffixLength, suffix, suffixLength) == 0);
}

int String::indexOf(char ch, unsigned int start) const throw() {
  int length = getLength();
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

int String::indexOf(const String& str, unsigned int start) const throw() {
  int length = getLength();
  int sublength = str.getLength();

  if ((start >= length) || (sublength == 0) || (sublength > length)) {
    return -1; // not found
  }

  const char* substring = str.getBuffer();
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

int String::lastIndexOf(const String& str, unsigned int start) const throw() {
  if ((str.isEmpty()) || (str.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  const char* substring = str.getBuffer();
  const char* buffer = getBuffer();
  const char* begin = &buffer[start - str.getLength() + 1]; // beginning of current substring
  const char* last = &buffer[start]; // end of current substring - buffer <= begin <= last

  // calculate hash of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const char* p = substring;
  const char* q = begin;
  unsigned int count = str.getLength();
  while (count--) {
    hash = (128 * hash + *p++) % 16647143;
    match = (128 * match + *q++) % 16647143;
    if (count > 0) {
      mask = (128 * mask) % 16647143;
    }
  }

  // look for substring
  count = getLength() - str.getLength() + 1; // number of characters left - count >= 1
  while (true) {
    if (match == hash) { // possible substring found
      if (compare(begin, substring, str.getLength()) == 0) {
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

unsigned int String::count(const String& str, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(str, start)) >= 0) { // until not found - works for empty str
    ++count;
    start = result + str.getLength();
  }
  return count;
}

template<>
int compare<String>(const String& a, const String& b) throw() {
  return strcmp(a.getElements(), b.getElements());
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException) {
  stream.addCharacterField(value.getBuffer(), value.getLength());
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
