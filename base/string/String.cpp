/***************************************************************************
    copyright            : (C) 2000 by Ren� M�ller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "String.h"
#include <string.h>
#include <ctype.h>

#define NOTFOUND -1
#define CAPACITY(desired) ((desired+GRANULARITY-1)/GRANULARITY*GRANULARITY)

template<class TYPE> inline TYPE min(TYPE a, TYPE b) {return (a <= b) ? a : b;};
template<class TYPE> inline TYPE max(TYPE a, TYPE b) {return (a >= b) ? a : b;};

int String::getLengthOfString(const char* str) const throw() {
  const char* terminator = (const char*)memchr(str, TERMINATOR, MAXIMUM_LENGTH); // find terminator
  if (terminator == NULL) {
    return -1; // string is not terminated or is too long
  }
  return terminator - str;
}

void String::setLength(unsigned int length) throw(MemoryException) {
  if (length != len) {
    if (length > MAXIMUM_LENGTH) {
      throw MemoryException();
    }
    char* buffer = getMutableBuffer(); // we are about to modify the buffer
    len = length;
    unsigned int requiredCapacity = CAPACITY(len + sizeof(TERMINATOR));
    if ((requiredCapacity > getCapacity())) { // do we have to expand capacity
      internal->setSize(requiredCapacity); // ok lets expand the buffer
    }
    // need something to reduce the buffer - must not conflict with ensuredCapacity
    buffer[len] = TERMINATOR; // terminate string
  }
}

void String::createString(const char* buffer, unsigned int length, unsigned int capacity) throw(MemoryException) {
  capacity = max(capacity, length + sizeof(TERMINATOR));
  capacity = max(capacity, DEFAULT_CAPACITY);
  internal = new StringBuffer(CAPACITY(capacity));
  len = length;
  memcpy(internal->getBytes(), buffer, len);
  internal->getBytes()[len] = TERMINATOR; // terminate
}

String::String(unsigned int capacity) throw(MemoryException) {
  createString(NULL, 0, capacity);
}

String::String(const StringLiteral& str) throw(MemoryException) {
  unsigned int length = str.size - sizeof(TERMINATOR);
  if (length > MAXIMUM_LENGTH) { // maximum length exceeded
    throw MemoryException();
  }
  createString(str.message, length, DEFAULT_CAPACITY);
}

String::String(const char* str) throw(MemoryException) {
  if (str != NULL) {
    int length = getLengthOfString(str);
    if (length < 0) { // maximum length exceeded
      throw MemoryException();
    }
    createString(str, length, DEFAULT_CAPACITY);
  } else {
    createString(NULL, 0, DEFAULT_CAPACITY);
  }
}

String::String(const char* str, unsigned int maximum) throw(MemoryException) {
  if (str != NULL) {
    int length = getLengthOfString(str);
    if (length < 0) { // maximum length exceeded
      throw MemoryException();
    }
    createString(str, min((unsigned int)length, maximum), DEFAULT_CAPACITY);
  } else {
    createString(NULL, 0, DEFAULT_CAPACITY);
  }
}

void String::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  unsigned int minimum = CAPACITY(length() + sizeof(TERMINATOR));
  capacity = CAPACITY(capacity);
  if ((capacity > getCapacity()) && (capacity > minimum)) {
    internal.ensureSingleReference(); // we are about to modify the buffer
    internal->setSize(capacity);
  }
}

void String::optimizeCapacity() throw(MemoryException) {
  unsigned int minimum = CAPACITY(length() + sizeof(TERMINATOR));
  if (getCapacity() > minimum) { // can we reduce the capacity
    internal.ensureSingleReference(); // we are about to modify the buffer
    internal->setSize(minimum);
  }
}

char String::getChar(unsigned int index) throw(OutOfRange) {
  if (index >= length()) {
    throw OutOfRange();
  }
  return getReadOnlyBuffer()[index];
}

void String::setChar(unsigned int index, char value) throw(OutOfRange) {
  if (index >= length()) {
    throw OutOfRange();
  }
  if (value != TERMINATOR) {
    getMutableBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

String::Character String::operator[](unsigned int index) throw(OutOfRange) {
  return Character(*this, index); // index is checked by getChar() and setChar()
}

char String::operator[](unsigned int index) const throw(OutOfRange) {
  if (index >= length()) {
    throw OutOfRange();
  }
  return getReadOnlyBuffer()[index];
}

String& String::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  if ((start <= end) && (start < length())) { // protect against some cases
    if (end >= length() - 1) {
      // remove section from end of string
      setLength(start);
    } else {
      // remove section from middle of string
      char* buffer = getMutableBuffer();
      memmove(buffer + start, buffer + end + 1, length() - end - 1); // move end of string
      setLength(length() - (end - start + 1));
    }
  }
  return *this;
}

String& String::insert(unsigned int index, char ch) throw(MemoryException) {
  setLength(length() + 1);
  char* buffer = getMutableBuffer();
  if (index >= length()) {
    // insert section at end of string
    buffer[length()] = ch;
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + 1, buffer + index, 1);
    buffer[length()] = ch;
  }
  return *this;
}

String& String::insert(unsigned int index, const String& str) throw(MemoryException) {
// problem if insert string into itself - memcpy.... -> memmove
  setLength(length() + str.length());
  char* buffer = getMutableBuffer();
  if (index >= length()) {
    // insert section at end of string
    memcpy(buffer + length(), str.getReadOnlyBuffer(), str.length());
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + str.length(), buffer + index, str.length());
    memcpy(buffer + index, str.getReadOnlyBuffer(), str.length());
  }
  return *this;
}

String& String::insert(unsigned int index, const char* str) throw(MemoryException) {
/*
  setLength(length() + str.length());
  char* buffer = getMutableBuffer();
  if (index >= length()) {
    // insert section at end of string
    memcpy(buffer + length(), str.getReadOnlyBuffer(), str.length());
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + str.length(), buffer + index, str.length());
    memcpy(buffer + index, str.getReadOnlyBuffer(), str.length());
  }*/
  return *this;
}

String& String::replace(unsigned int start, unsigned int end, const String& str) throw(MemoryException) {
// need better implementation
  if (start <= end) {
    ensureCapacity(length() - (end - start + 1) + str.length());
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
    replace(found, found + fromStr.length() - 1, toStr); // fromStr.length() > 0
    start = found + toStr.length(); // skip toStr
  }
  return count;
}

String String::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  if ((start <= end) && (start < length())) {
    if (end >= length()) {
      end = length() - 1; // index of last char in this string
    }
    // 0 <= start <= end < length()
    unsigned int lengthOfSubstring = end - start + 1;
    String s(lengthOfSubstring);
    memcpy(s.getMutableBuffer(), getReadOnlyBuffer() + start, lengthOfSubstring); // buffers do not overlap
    s.getMutableBuffer()[lengthOfSubstring] = TERMINATOR;
    return s;
  } else {
    return String(); // return empty string
  }
}

String& String::operator-=(const String& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.length());
  }
  return *this;
}

String& String::reverse() throw() {
  char* p = getMutableBuffer();
  char* q = &p[length() - 1]; // last char - empty string => q < p
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
  if (length() == str.length()) {
    return memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), length());
  } else if (length() < str.length()) {
    int result = memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), length());
    return (result != 0) ? result : str.getReadOnlyBuffer()[length()];
  } else {
    int result = memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), str.length());
    return (result != 0) ? result : getReadOnlyBuffer()[str.length()];
  }
//  return strcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer());
}

int String::compareToIgnoreCase(const String& str) const throw() {
//  return strcasecmp(getReadOnlyBuffer(), str.getReadOnlyBuffer());
  const char* p = getReadOnlyBuffer();
  const char* q = str.getReadOnlyBuffer();
  while (*p && *q) { // continue until end of any string has been reached
    int result = tolower(*p) - tolower(*q);
    if (result != 0) { // not equal
      return result;
    }
    ++p;
    ++q;
  }
  // possible cases: only end of p (less than), only end of q (greater than), end of both (equal)
  return (tolower(*p) - tolower(*q));
}

bool String::startsWith(const String& prefix) const throw() {
  if ((prefix.isEmpty()) || (prefix.length() > length())) { // 0 < prefix.length() <= length()
    return false;
  }
  return memcmp(getReadOnlyBuffer(), prefix.getReadOnlyBuffer(), prefix.length());
//  return strncmp(getReadOnlyBuffer(), prefix.getReadOnlyBuffer(), prefix.length());
}

bool String::endsWith(const String& suffix) const throw() {
  if ((suffix.isEmpty()) || (suffix.length() > length())) { // 0 < suffix.length() <= length()
    return false;
  }
  return memcmp(&getReadOnlyBuffer()[length() - suffix.length()], suffix.getReadOnlyBuffer(), suffix.length());
//  return strncmp(&getReadOnlyBuffer()[length() - suffix.length()], suffix.getReadOnlyBuffer(), suffix.length());
}

int String::indexOf(char ch, unsigned int start) const throw() {
  if (start >= length()) {
    return NOTFOUND;
  }
  const char* buffer = getReadOnlyBuffer();
  const char* p = &buffer[start]; // go to start location
  while (*p || (*p != ch)) { // until terminator or found
    ++p;
  }
  if (*p == ch) {
    return p - buffer; // return index of found character
  } else {
    return NOTFOUND;
  }
}

int String::indexOf(const String& str, unsigned int start) const throw() {
  if ((start >= length()) || (str.isEmpty()) || (str.length() > length())) {
    return NOTFOUND;
  }

  const char* substring = str.getReadOnlyBuffer();
  const char* buffer = getReadOnlyBuffer();
  const char* begin = &buffer[start]; // beginning of current substring
  const char* last = &begin[str.length() - 1]; // end of current substring - last >= begin >= buffer

  // calculate hash of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const char* p = substring;
  const char* q = begin;
  unsigned int count = str.length();
  while (count--) {
    hash = (128 * hash + *p++) % 16647143;
    match = (128 * match + *q++) % 16647143;
    if (count > 0) {
      mask = (128 * mask) % 16647143;
    }
  }

  // look for substring
  count = length() - str.length() + 1; // number of characters left - minimum is 1
  while (true) {
    if (match == hash) { // possible substring found
      if (memcmp(begin, substring, str.length()) == 0) {
        return begin - buffer; // return index of substring
      }
    }
    if (!--count) { // are we done - make sure we check for match for the last char
      break;
    }
    match = (match + 128 * 16647143 - mask * *begin) % 16647143;
	  match = (match * 128 + *last) % 16647143;
    ++begin;
    ++last;
  }

  return NOTFOUND;
}

int String::lastIndexOf(char ch, unsigned int start) const throw() {
  // examined cases: length() = 0, length() = 1, length() > 1
  const char* buffer = getReadOnlyBuffer();
  const char* p = &buffer[(start < length()) ? start : length() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return NOTFOUND;
}

int String::lastIndexOf(const String& str, unsigned int start) const throw() {
  if ((str.isEmpty()) || (str.length() > length())) { // eliminate some cases
    return NOTFOUND;
  }
  if (start >= length()) { // validate start - 0 <= start < length()
    start = length() - 1;
  }

  const char* substring = str.getReadOnlyBuffer();
  const char* buffer = getReadOnlyBuffer();
  const char* begin = &buffer[start - str.length() + 1]; // beginning of current substring
  const char* last = &buffer[start]; // end of current substring - buffer <= begin <= last

  // calculate hash of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const char* p = substring;
  const char* q = begin;
  unsigned int count = str.length();
  while (count--) {
    hash = (128 * hash + *p++) % 16647143;
    match = (128 * match + *q++) % 16647143;
    if (count > 0) {
      mask = (128 * mask) % 16647143;
    }
  }

  // look for substring
  count = length() - str.length() + 1; // number of characters left - count >= 1
  while (true) {
    if (match == hash) { // possible substring found
      if (memcmp(begin, substring, str.length()) == 0) {
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

  return NOTFOUND;
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
    start = result + str.length();
  }
  return count;
}

String String::toString() const {
  return String(*this);
}

String::~String() throw() {
}

String operator+(const String& left, const String& right) throw(MemoryException) {
  return String(left.length() + right.length()).append(left).append(right);
}

String operator-(const String& left, const String& right) throw(MemoryException) {
  if (left.endsWith(right)) {
    return left.substring(0, left.length() - right.length()); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException) {
  stream.addCharacterField(value.getReadOnlyBuffer(), value.length());
  return stream;
}
