/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/String.h>
#include <base/Functor.h>
#include <string.h>
#include <ctype.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

int String::getLengthOfString(const char* str) const throw() {
  const char* terminator = find(str, MAXIMUM_LENGTH, TERMINATOR); // find terminator
  if (!terminator) { // was the terminator found
    return -1; // string is not terminated or is too long
  }
  return terminator - str;
}

void String::setLength(unsigned int length) throw(MemoryException) {
  if (length != getLength()) {
    assert(length <= MAXIMUM_LENGTH, MemoryException()); // choose better exception
    char* buffer = getMutableBuffer(); // we are about to modify the buffer
    elements->setSize(length + 1);
    buffer[length] = TERMINATOR; // terminate string
  }
}

String::String() throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  elements->getElements()[0] = TERMINATOR; // terminate
}

String::String(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
  elements->getElements()[0] = TERMINATOR; // terminate
}

String::String(const StringLiteral& str) throw(MemoryException) : elements(0) {
  unsigned int length = str.size - 1;
  assert(length <= MAXIMUM_LENGTH, MemoryException()); // choose better exception
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  copy(elements->getElements(), str.message, length); // no overlap
  elements->getElements()[length] = TERMINATOR; // terminate
}

String::String(const char* str) throw(MemoryException) : elements(0) {
  int length = 0;
  if (str) { // is string proper (not empty)
    length = getLengthOfString(str);
    assert(length >= 0, MemoryException()); // maximum length exceeded
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    copy(elements->getElements(), str, length); // no overlap
  }
  elements->getElements()[length] = TERMINATOR; // terminate
}

String::String(const char* str, unsigned int maximum) throw(MemoryException) : elements(0) {
  int length = 0;
  if (str) { // is string proper
    length = getLengthOfString(str);
    assert(length >= 0, MemoryException()); // maximum length exceeded
    length = minimum((unsigned int)length, maximum);
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    copy(elements->getElements(), str, length); // no overlap
  }
  elements->getElements()[length] = TERMINATOR; // terminate
}

String& String::operator=(const String& eq) throw() {
  if (&eq != this) { // protect against self assignment
    this->elements = eq.elements;
  }
  return *this;
}

String& String::operator=(const char* str) throw(MemoryException) {
  int length = 0;
  if (str) { // is string proper (not empty)
    length = getLengthOfString(str);
    assert(length >= 0, MemoryException()); // maximum length exceeded
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    copy(elements->getElements(), str, length); // no overlap
  }
  elements->getElements()[length] = TERMINATOR; // terminate
  return *this;
}

void String::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  // no need to do copyOnWrite - or should we?
  elements->ensureCapacity(capacity);
}

void String::optimizeCapacity() throw() {
  // no need to do copyOnWrite - or should we?
  elements->optimizeCapacity();
}

unsigned int String::getGranularity() const throw() {
  return elements->getGranularity();
}

void String::setGranularity(unsigned int granularity) throw() {
  elements->setGranularity(granularity);
}

char String::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  return getReadOnlyBuffer()[index];
}

void String::setAt(unsigned int index, char value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  if (value != TERMINATOR) {
    getMutableBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

String& String::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  if ((start <= end) && (start < getLength())) { // protect against some cases
    if (end >= getLength() - 1) {
      // remove section from end of string
      setLength(start);
    } else {
      // remove section from middle of string
      char* buffer = getMutableBuffer();
      move(buffer + start, buffer + end + 1, getLength() - end - 1); // move end of string
      setLength(getLength() - (end - start + 1));
    }
  }
  return *this;
}

String& String::insert(unsigned int index, char ch) throw(MemoryException) {
  setLength(getLength() + 1);
  char* buffer = getMutableBuffer();
  if (index >= getLength()) {
    // insert section at end of string
    buffer[getLength()] = ch;
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, 1);
    buffer[getLength()] = ch;
  }
  return *this;
}

String& String::insert(unsigned int index, const String& str) throw(MemoryException) {
// problem if insert string into itself - copy.... -> move
  int length = getLength();
  int strlength = str.getLength();
  setLength(length + strlength);
  char* buffer = getMutableBuffer();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, str.getReadOnlyBuffer(), strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, strlength); // move end of string
    copy(buffer + index, str.getReadOnlyBuffer(), strlength);
  }
  return *this;
}

String& String::insert(unsigned int index, const char* str) throw(MemoryException) {
  int length = getLength();
  int strlength = getLengthOfString(str);
  assert(strlength >= 0, MemoryException()); // maximum length exceeded
  setLength(length + strlength);
  char* buffer = getMutableBuffer();
  if (index >= length) {
    // insert section at end of string
    copy(buffer + length, str, strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, strlength);
    copy(buffer + index, str, strlength);
  }
  return *this;
}

String& String::append(const char* str, unsigned int maximum) throw(MemoryException) {
  int strlength = 0;
  if (str) { // is string proper
    strlength = getLengthOfString(str);
    assert(strlength >= 0, MemoryException()); // maximum length exceeded
    strlength = minimum((unsigned int)strlength, maximum);
  }
  int length = getLength();
  setLength(length + strlength);
  char* buffer = getMutableBuffer();
  copy(buffer + length, str, strlength);
  return *this;
}

String& String::replace(unsigned int start, unsigned int end, const String& str) throw(MemoryException) {
// need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

unsigned int String::replaceAll(const String& fromStr, const String& toStr) throw(MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength() - 1, toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

String String::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  if ((start <= end) && (start < getLength())) {
    if (end >= getLength()) {
      end = getLength() - 1; // index of last char in this string
    }
    // 0 <= start <= end < getLength()
    unsigned int lengthOfSubstring = end - start + 1;
    String result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    copy(result.getMutableBuffer(), getReadOnlyBuffer() + start, lengthOfSubstring); // buffers do not overlap
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
  char* p = getMutableBuffer();
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

char* String::substring(char* buffer, unsigned int start, unsigned int end) const throw() {
  if (buffer) {
    if ((start <= end) && (start < getLength())) {
      if (end >= getLength()) {
        end = getLength() - 1; // index of last char in this string
      }
      // 0 <= start <= end < getLength()
      unsigned int lengthOfSubstring = end - start + 1;
      copy(buffer, getReadOnlyBuffer() + start, lengthOfSubstring); // buffers do not overlap
      buffer[lengthOfSubstring] = TERMINATOR;
    } else {
      *buffer = TERMINATOR;
    }
  }
  return buffer;
}

String& String::toLowerCase() throw() {
  char* p = getMutableBuffer();
  while (*p != TERMINATOR) {
    *p = tolower(*p);
    ++p;
  }
  return *this;
}

String& String::toUpperCase() throw() {
  char* p = getMutableBuffer();
  while (*p != TERMINATOR) {
    *p = toupper(*p);
    ++p;
  }
  return *this;
}

int String::compareTo(const String& str) const throw() {
  return strcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer());
}

int String::compareTo(const char* str) const throw() {
  return strcmp(getReadOnlyBuffer(), str);
}

int String::compareToIgnoreCase(const char* l, const char* r) throw() {
  while (*l && *r) { // continue until end of any string has been reached
    int result = tolower(*l) - tolower(*r);
    if (result != 0) { // not equal
      return result;
    }
    ++l;
    ++r;
  }
  // possible cases: only end of 'l' (less than), only end of 'r' (greater than), end of both (equal)
  return (tolower(*l) - tolower(*r));
}

int String::compareToIgnoreCase(const String& str) const throw() {
  return compareToIgnoreCase(getReadOnlyBuffer(), str.getReadOnlyBuffer());
}

int String::compareToIgnoreCase(const char* str) const throw() {
  return compareToIgnoreCase(getReadOnlyBuffer(), str);
}

bool String::startsWith(const String& prefix) const throw() {
  if ((prefix.isEmpty()) || (prefix.getLength() > getLength())) { // 0 < prefix.getLength() <= getLength()
    return false;
  }
  return compare(getReadOnlyBuffer(), prefix.getReadOnlyBuffer(), prefix.getLength()) == 0;
}

bool String::endsWith(const String& suffix) const throw() {
  if ((suffix.isEmpty()) || (suffix.getLength() > getLength())) { // 0 < suffix.getLength() <= getLength()
    return false;
  }
  return compare(getReadOnlyBuffer() + getLength() - suffix.getLength(), suffix.getReadOnlyBuffer(), suffix.getLength()) == 0;
}

int String::indexOf(char ch, unsigned int start) const throw() {
  if (start >= getLength()) {
    return -1; // not found
  }

  const char* buffer = getReadOnlyBuffer();
  unsigned int count = getLength() - start;

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

  const char* substring = str.getReadOnlyBuffer();
  const char* buffer = getReadOnlyBuffer();
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
  const char* buffer = getReadOnlyBuffer();
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

  const char* substring = str.getReadOnlyBuffer();
  const char* buffer = getReadOnlyBuffer();
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
  return strcmp(a.getBytes(), b.getBytes());
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException) {
  stream.addCharacterField(value.getReadOnlyBuffer(), value.getLength());
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
