/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/String.h>
#include <base/Functor.h>
#include <string.h>
#include <ctype.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

template String<DefaultLock>;
template String<Unsafe>;
/*
#if !defined(HAVE_MEMCHR)
inline const char* memchr(const char* src, int value, unsigned int count) {
  return find<char>(src, count, bind2Second(Equal<char>(), value));
}
#endif

#if !defined(HAVE_MEMCPY)
inline char* memcpy(char* dest, const char* src, unsigned int count) {
  copy<char>(dest, src, count);
  return dest;
}
#endif

#if !defined(HAVE_MEMMOVE)
inline char* memmove(char* dest, const char* src, unsigned int count) {
  // problem if overlapping
  copy<char>(dest, src, count);
  return dest;
}
#endif
*/
#define NOTFOUND -1

template<class LOCK>
int String<LOCK>::getLengthOfString(const char* str) const throw() {
  const char* terminator = (const char*)memchr(str, TERMINATOR, MAXIMUM_LENGTH); // find terminator
  if (!terminator) { // was the terminator found
    return -1; // string is not terminated or is too long
  }
  return terminator - str;
}

template<class LOCK>
void String<LOCK>::setLength(unsigned int length) throw(MemoryException) {
  if (length != getLength()) {
    if (length > MAXIMUM_LENGTH) {
      throw MemoryException(); // choose better exception
    }
    char* buffer = getMutableBuffer(); // we are about to modify the buffer
    elements->setSize(length + 1);
    buffer[length] = TERMINATOR; // terminate string
  }
}

template<class LOCK>
String<LOCK>::String() throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(1, GRANULARITY);
  elements->getElements()[0] = TERMINATOR; // terminate
}

template<class LOCK>
String<LOCK>::String(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<char>(capacity + 1, GRANULARITY);
  elements->getElements()[0] = TERMINATOR; // terminate
}

template<class LOCK>
String<LOCK>::String(const StringLiteral& str) throw(MemoryException) : elements(0) {
  unsigned int length = str.size - 1;
  if (length > MAXIMUM_LENGTH) { // maximum length exceeded
    throw MemoryException();
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  memcpy(elements->getElements(), str.message, length);
  elements->getElements()[length] = TERMINATOR; // terminate
}

template<class LOCK>
String<LOCK>::String(const char* str) throw(MemoryException) : elements(0) {
  int length = 0;
  if (str) { // is string proper (not empty)
    length = getLengthOfString(str);
    if (length < 0) { // maximum length exceeded
      throw MemoryException();
    }
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    memcpy(elements->getElements(), str, length);
  }
  elements->getElements()[length] = TERMINATOR; // terminate
}

template<class LOCK>
String<LOCK>::String(const char* str, unsigned int maximum) throw(MemoryException) : elements(0) {
  int length = 0;
  if (str) { // is string proper
    length = getLengthOfString(str);
    if (length < 0) { // maximum length exceeded
      throw MemoryException();
    }
    length = minimum((unsigned int)length, maximum);
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    memcpy(elements->getElements(), str, length);
  }
  elements->getElements()[length] = TERMINATOR; // terminate
}

template<class LOCK>
String<LOCK>& String<LOCK>::operator=(const String& eq) throw() {
  if (&eq != this) { // protect against self assignment
    this->elements = eq.elements;
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::operator=(const char* str) throw(MemoryException) {
  int length = 0;
  if (str) { // is string proper (not empty)
    length = getLengthOfString(str);
    if (length < 0) { // maximum length exceeded
      throw MemoryException();
    }
  }
  elements = new ReferenceCountedCapacityAllocator<char>(length + 1, GRANULARITY);
  if (length) {
    memcpy(elements->getElements(), str, length);
  }
  elements->getElements()[length] = TERMINATOR; // terminate
  return *this;
}

template<class LOCK>
void String<LOCK>::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  // no need to do copyOnWrite - or should we?
  elements->ensureCapacity(capacity);
}

template<class LOCK>
void String<LOCK>::optimizeCapacity() throw() {
  // no need to do copyOnWrite - or should we?
  elements->optimizeCapacity();
}

template<class LOCK>
char String<LOCK>::getChar(unsigned int index) throw(OutOfRange) {
  if (index >= getLength()) {
    throw OutOfRange();
  }
  return getReadOnlyBuffer()[index];
}

template<class LOCK>
void String<LOCK>::setChar(unsigned int index, char value) throw(OutOfRange) {
  if (index >= getLength()) {
    throw OutOfRange();
  }
  if (value != TERMINATOR) {
    getMutableBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

template<class LOCK>
String<LOCK>::Character String<LOCK>::operator[](unsigned int index) throw(OutOfRange) {
  return Character(*this, index); // index is checked by getChar() and setChar()
}

template<class LOCK>
char String<LOCK>::operator[](unsigned int index) const throw(OutOfRange) {
  if (index >= getLength()) {
    throw OutOfRange();
  }
  return getReadOnlyBuffer()[index];
}

template<class LOCK>
String<LOCK>& String<LOCK>::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  if ((start <= end) && (start < getLength())) { // protect against some cases
    if (end >= getLength() - 1) {
      // remove section from end of string
      setLength(start);
    } else {
      // remove section from middle of string
      char* buffer = getMutableBuffer();
      memmove(buffer + start, buffer + end + 1, getLength() - end - 1); // move end of string
      setLength(getLength() - (end - start + 1));
    }
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::insert(unsigned int index, char ch) throw(MemoryException) {
  setLength(getLength() + 1);
  char* buffer = getMutableBuffer();
  if (index >= getLength()) {
    // insert section at end of string
    buffer[getLength()] = ch;
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + 1, buffer + index, 1);
    buffer[getLength()] = ch;
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::insert(unsigned int index, const String& str) throw(MemoryException) {
// problem if insert string into itself - memcpy.... -> memmove
  int length = getLength();
  int strlength = str.getLength();
  setLength(length + strlength);
  char* buffer = getMutableBuffer();
  if (index >= length) {
    // insert section at end of string
    memcpy(buffer + length, str.getReadOnlyBuffer(), strlength);
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + strlength, buffer + index, strlength);
    memcpy(buffer + index, str.getReadOnlyBuffer(), strlength);
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::insert(unsigned int index, const char* str) throw(MemoryException) {
  int length = getLength();
  int strlength = getLengthOfString(str);
  if (strlength < 0) { // maximum length exceeded
    throw MemoryException();
  }
  setLength(length + strlength);
  char* buffer = getMutableBuffer();
  if (index >= length) {
    // insert section at end of string
    memcpy(buffer + length, str, strlength);
  } else {
    // insert section in middle or beginning of string
    memmove(buffer + index + strlength, buffer + index, strlength);
    memcpy(buffer + index, str, strlength);
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::replace(unsigned int start, unsigned int end, const String& str) throw(MemoryException) {
// need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

template<class LOCK>
unsigned int String<LOCK>::replaceAll(const String& fromStr, const String& toStr) throw(MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength() - 1, toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

template<class LOCK>
String<LOCK> String<LOCK>::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  if ((start <= end) && (start < getLength())) {
    if (end >= getLength()) {
      end = getLength() - 1; // index of last char in this string
    }
    // 0 <= start <= end < getLength()
    unsigned int lengthOfSubstring = end - start + 1;
    String s(lengthOfSubstring);
    memcpy(s.getMutableBuffer(), getReadOnlyBuffer() + start, lengthOfSubstring); // buffers do not overlap
    s.getMutableBuffer()[lengthOfSubstring] = TERMINATOR;
    return s;
  } else {
    return String(); // return empty string
  }
}

template<class LOCK>
String<LOCK>& String<LOCK>::operator-=(const String& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::reverse() throw() {
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

template<class LOCK>
char* String<LOCK>::substring(char* buffer, unsigned int start, unsigned int end) const throw() {
  if (buffer) {
    if ((start <= end) && (start < getLength())) {
      if (end >= getLength()) {
        end = getLength() - 1; // index of last char in this string
      }
      // 0 <= start <= end < getLength()
      unsigned int lengthOfSubstring = end - start + 1;
      memcpy(buffer, getReadOnlyBuffer() + start, lengthOfSubstring); // buffers do not overlap
      buffer[lengthOfSubstring] = TERMINATOR;
    } else {
      *buffer = TERMINATOR;
    }
  }
  return buffer;
}

template<class LOCK>
String<LOCK>& String<LOCK>::toLowerCase() throw() {
  char* p = getMutableBuffer();
  while (*p != TERMINATOR) {
    *p = tolower(*p);
    ++p;
  }
  return *this;
}

template<class LOCK>
String<LOCK>& String<LOCK>::toUpperCase() throw() {
  char* p = getMutableBuffer();
  while (*p != TERMINATOR) {
    *p = toupper(*p);
    ++p;
  }
  return *this;
}

template<class LOCK>
int String<LOCK>::compareTo(const String& str) const throw() {
  if (getLength() == str.getLength()) {
    return memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), getLength());
  } else if (getLength() < str.getLength()) {
    int result = memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), getLength());
    return (result != 0) ? result : str.getReadOnlyBuffer()[getLength()];
  } else {
    int result = memcmp(getReadOnlyBuffer(), str.getReadOnlyBuffer(), str.getLength());
    return (result != 0) ? result : getReadOnlyBuffer()[str.getLength()];
  }
}

template<class LOCK>
int String<LOCK>::compareTo(const char* str) const throw() {
  return strcmp(getReadOnlyBuffer(), str);
}

template<class LOCK>
int String<LOCK>::compareToIgnoreCase(const char* l, const char* r) throw() {
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

template<class LOCK>
int String<LOCK>::compareToIgnoreCase(const String& str) const throw() {
  return compareToIgnoreCase(getReadOnlyBuffer(), str.getReadOnlyBuffer());
}

template<class LOCK>
int String<LOCK>::compareToIgnoreCase(const char* str) const throw() {
  return compareToIgnoreCase(getReadOnlyBuffer(), str);
}

template<class LOCK>
bool String<LOCK>::startsWith(const String& prefix) const throw() {
  if ((prefix.isEmpty()) || (prefix.getLength() > getLength())) { // 0 < prefix.getLength() <= getLength()
    return false;
  }
  return memcmp(getReadOnlyBuffer(), prefix.getReadOnlyBuffer(), prefix.getLength());
//  return strncmp(getReadOnlyBuffer(), prefix.getReadOnlyBuffer(), prefix.getLength());
}

template<class LOCK>
bool String<LOCK>::endsWith(const String& suffix) const throw() {
  if ((suffix.isEmpty()) || (suffix.getLength() > getLength())) { // 0 < suffix.getLength() <= getLength()
    return false;
  }
  return memcmp(&getReadOnlyBuffer()[getLength() - suffix.getLength()], suffix.getReadOnlyBuffer(), suffix.getLength());
//  return strncmp(&getReadOnlyBuffer()[getLength() - suffix.getLength()], suffix.getReadOnlyBuffer(), suffix.getLength());
}

template<class LOCK>
int String<LOCK>::indexOf(char ch, unsigned int start) const throw() {
  if (start >= getLength()) {
    return NOTFOUND;
  }
  const char* buffer = getReadOnlyBuffer();
  const char* p = &buffer[start]; // go to start location
  while ((*p != TERMINATOR) && (*p != ch)) { // until terminator or found
    ++p;
  }
  if (*p == ch) {
    return p - buffer; // return index of found character
  } else {
    return NOTFOUND;
  }
}

template<class LOCK>
int String<LOCK>::indexOf(const String& str, unsigned int start) const throw() {
  int length = getLength();
  int sublength = str.getLength();

  if ((start >= length) || (sublength == 0) || (sublength > length)) {
    return NOTFOUND;
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
      if (memcmp(begin, substring, sublength) == 0) {
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

  return NOTFOUND;
}

template<class LOCK>
int String<LOCK>::lastIndexOf(char ch, unsigned int start) const throw() {
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const char* buffer = getReadOnlyBuffer();
  const char* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return NOTFOUND;
}

template<class LOCK>
int String<LOCK>::lastIndexOf(const String& str, unsigned int start) const throw() {
  if ((str.isEmpty()) || (str.getLength() > getLength())) { // eliminate some cases
    return NOTFOUND;
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
      if (memcmp(begin, substring, str.getLength()) == 0) {
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

template<class LOCK>
unsigned int String<LOCK>::count(char ch, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

template<class LOCK>
unsigned int String<LOCK>::count(const String& str, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(str, start)) >= 0) { // until not found - works for empty str
    ++count;
    start = result + str.getLength();
  }
  return count;
}

template<class LOCK>
String<LOCK> String<LOCK>::toString() const {
  return String(*this);
}

template<class LOCK>
FormatOutputStream& String<LOCK>::operator<<(FormatOutputStream& stream) const {
  SynchronizeShared();
  stream.addCharacterField(getReadOnlyBuffer(), getLength());
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
