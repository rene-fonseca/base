/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/WideString.h>
#include <base/Functor.h>
#include <stdlib.h>
#include <wchar.h>
#include <limits.h> // defines MB_LEN_MAX

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__WIDE)

#if !defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
  #warning Assumes that mbstowcs is reentrant
#endif

const WideString WideString::DEFAULT_STRING(WIDEMESSAGE(""));

WideString::WideString() throw() : elements(DEFAULT_STRING.elements) {
}

WideString::WideString(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<Character>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

WideString::WideString(const WideStringLiteral& str) throw(WideStringException, MemoryException) : elements(0) {
  unsigned int length = str.getLength();
  assert(length <= MAXIMUM_LENGTH, WideStringException()); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), str, length); // no overlap
}

WideString::WideString(const Character* string) throw(WideStringException, MemoryException) : elements(0) {
  assert(string, WideStringException()); // make sure string is proper (not empty)
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, WideStringException()); // maximum length exceeded
  int numberOfCharacters = terminator - string;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const Character* string, unsigned int maximum) throw(OutOfDomain, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  assert(string, WideStringException()); // make sure string is proper (not empty)
  const Character* terminator = find(string, maximum, Traits::TERMINATOR); // find terminator
  int numberOfCharacters = terminator ? (terminator - string) : maximum;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const char* string) throw(MultibyteException, MemoryException) : elements(0) {
  int numberOfCharacters = 0;
  if (string) { // is string proper (not empty)
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t)); // TAG: initial state - need faster alternative
    const char* current = string;
    size_t result = mbsrtowcs(0, &current, 0, &state);
#else
    size_t result = mbstowcs(0, string, 0);
#endif
    assert(result != size_t(-1), MultibyteException());
    assert(result <= MAXIMUM_LENGTH, MemoryException()); // maximum length exceeded
    numberOfCharacters = result;
  }
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  if (numberOfCharacters) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t)); // TAG: initial state - need faster alternative
    const char* current = string;
    size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
#else
    size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
#endif
  }
}

WideString::WideString(const char* string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  int numberOfCharacters = 0;
  if (string) { // is string proper
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t)); // TAG: initial state - need faster alternative
    const char* current = string;
    size_t result = mbsrtowcs(0, &current, maximum, &state);
#else
    size_t result = mbstowcs(0, string, maximum);
#endif
    assert(result != size_t(-1), MultibyteException());
    assert(result <= MAXIMUM_LENGTH, MemoryException()); // maximum length exceeded
    numberOfCharacters = minimum(static_cast<unsigned int>(result), maximum);
  }
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  if (numberOfCharacters) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t)); // TAG: initial state - need faster alternative
    const char* current = string;
    size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
#else
    size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
#endif
  }
}

void WideString::ensureCapacity(unsigned int capacity) throw(MemoryException) {
  elements->ensureCapacity(capacity); // no need to do copyOnWrite
}

void WideString::optimizeCapacity() throw() {
  elements->optimizeCapacity(); // no need to do copyOnWrite
}

unsigned int WideString::getGranularity() const throw() {
  return elements->getGranularity();
}

void WideString::setGranularity(unsigned int granularity) throw() {
  elements->setGranularity(granularity);
}

WideString::Character WideString::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  return getBuffer()[index];
}

void WideString::setAt(unsigned int index, Character value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange());
  if (value != Traits::TERMINATOR) {
    getBuffer()[index] = value;
  } else {
    setLength(index);
  }
}

WideString& WideString::remove(unsigned int start, unsigned int end) throw(MemoryException) {
  int length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      Character* buffer = getBuffer(); // we are about to modify the buffer
      move(buffer + start, buffer + end, length - end); // move end of string
      elements->setSize(length - (end - start) + 1); // remember space for terminator
    }
  }
  return *this;
}

WideString& WideString::removeFrom(unsigned int start) throw(MemoryException) {
  if (start < getLength()) { // protect against some cases
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(start + 1); // remove section from end of string
  }
  return *this;
}

WideString& WideString::insert(unsigned int index, Character ch) throw(WideStringException, MemoryException) {
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

WideString& WideString::insert(unsigned int index, const WideString& str) throw(WideStringException, MemoryException) {
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

WideString& WideString::insert(unsigned int index, const WideStringLiteral& str) throw(WideStringException, MemoryException) {
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

WideString& WideString::append(const WideStringLiteral& str) throw(WideStringException, MemoryException) {
  int length = getLength();
  setLength(length + str.getLength());
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, str.getLength());
  return *this;
}

WideString& WideString::append(const WideStringLiteral& str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  int length = getLength();
  setLength(length + minimum(str.getLength(), maximum));
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, minimum(str.getLength(), maximum));
  return *this;
}

WideString& WideString::append(const Character* str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain()); // maximum length exceeded
  assert(str, WideStringException()); // make sure string is proper (not empty)
  int strlength = 0;
  const Character* terminator = find(str, maximum, Traits::TERMINATOR); // find terminator
  strlength = terminator ? (terminator - str) : maximum;
  int length = getLength();
  setLength(length + strlength);
  Character* buffer = elements->getElements();
  copy(buffer + length, str, strlength);
  return *this;
}

WideString& WideString::replace(unsigned int start, unsigned int end, const WideString& str) throw(WideStringException, MemoryException) {
  // need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

unsigned int WideString::replaceAll(const WideString& fromStr, const WideString& toStr) throw(WideStringException, MemoryException) {
  unsigned int count = 0;
  unsigned int start = 0;
  int found;
  while ((found = indexOf(fromStr, start)) >= 0) { // continue until no more fromStr's
    replace(found, found + fromStr.getLength() - 1, toStr); // fromStr.getLength() > 0
    start = found + toStr.getLength(); // skip toStr
  }
  return count;
}

WideString WideString::substring(unsigned int start, unsigned int end) const throw(MemoryException) {
  int length = getLength();
  if ((start < end) && (start < length)) {
    if (end > length) {
      end = length; // force to end of string
    }
    // 0 <= start < end <= getLength()
    unsigned int lengthOfSubstring = end - start;
    WideString result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    return WideString(); // return empty string
  }
}

WideString& WideString::operator-=(const WideString& suffix) throw(MemoryException) {
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

WideString& WideString::reverse() throw() {
  Character* p = getBuffer();
  Character* q = &p[getLength() - 1]; // last char - empty string => q < p
  Character temp;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

//WideString::Character* WideString::substring(Character* buffer, unsigned int start, unsigned int end) const throw() {
//  if (buffer) {
//    if ((start <= end) && (start < getLength())) {
//      if (end >= getLength()) {
//        end = getLength() - 1; // index of last char in this string
//      }
//      // 0 <= start <= end < getLength()
//      unsigned int lengthOfSubstring = end - start + 1;
//      copy(buffer, getBuffer() + start, lengthOfSubstring); // buffers do not overlap
//      buffer[lengthOfSubstring] = Traits::TERMINATOR;
//    } else {
//      *buffer = Traits::TERMINATOR;
//    }
//  }
//  return buffer;
//}

WideString& WideString::toLowerCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

WideString& WideString::toUpperCase() throw() {
  transform(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int WideString::compareTo(const WideString& str) const throw() {
  return wcscmp(getElements(), str.getElements());
}

int WideString::compareTo(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException());
  return wcscmp(getElements(), str);
}

int WideString::compareToIgnoreCase(const Character* left, const Character* right) throw() {
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

int WideString::compareToIgnoreCase(const WideString& str) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return _wcsicmp(getElements(), str.getElements());
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str.getElements());
#else // Unix
  return compareToIgnoreCase(getElements(), str.getElements());
#endif
}

int WideString::compareToIgnoreCase(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException());
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return _wcsicmp(getElements(), str);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str);
#else // Unix
  return compareToIgnoreCase(getElements(), str);
#endif
}

bool WideString::startsWith(const WideString& prefix) const throw() {
  return !prefix.isEmpty() && (wcsncmp(getBuffer(), prefix.getBuffer(), prefix.getLength()) == 0); // null-terminators are not required
}

bool WideString::startsWith(const WideStringLiteral& prefix) const throw() {
  return (prefix.getLength() > 0) && (wcsncmp(getBuffer(), prefix, prefix.getLength()) == 0); // null-terminator is not required
}

bool WideString::endsWith(const WideString& suffix) const throw() {
  return !suffix.isEmpty() && (wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix.getBuffer(), suffix.getLength()) == 0); // null-terminators are not required
}

bool WideString::endsWith(const WideStringLiteral& suffix) const throw() {
  return (suffix.getLength() > 0) && (wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix, suffix.getLength()) == 0); // null-terminator is not required
}

int WideString::indexOf(Character ch, unsigned int start) const throw() {
  int length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const Character* buffer = getElements();
  const Character* result = wcschr(buffer + start, ch);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

int WideString::indexOf(const WideString& str, unsigned int start) const throw() {
  if (start >= getLength()) {
    return -1; // not found
  }

  const Character* buffer = getElements();
  const Character* result = wcsstr(buffer + start, str.getElements());
  if (result) {
    return result - buffer;
  } else {
    return -1; // not found
  }
}

int WideString::lastIndexOf(Character ch, unsigned int start) const throw() {
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const Character* buffer = getBuffer();
  const Character* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return -1; // not found
}

int WideString::lastIndexOf(const WideString& str, unsigned int start) const throw() {
  if ((str.isEmpty()) || (str.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  // TAG: todo

  return -1; // not found
}

unsigned int WideString::count(Character ch, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

unsigned int WideString::count(const WideString& str, unsigned int start) const throw() {
  int result;
  unsigned int count = 0;
  while ((result = indexOf(str, start)) >= 0) { // until not found - works for empty str
    ++count;
    start = result + str.getLength();
  }
  return count;
}

template<>
int compare<WideString>(const WideString& left, const WideString& right) throw() {
  return wcscmp(left.getElements(), right.getElements());
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException) {
  Allocator<char> buffer(MB_LEN_MAX * value.getLength() + 1); // remember terminator - greedy implementation
  size_t result = wcstombs(buffer.getElements(), value.getElements(), buffer.getSize());
  assert(result != size_t(-1), MultibyteException());
  stream.addCharacterField(buffer.getElements(), result);
  return stream;
}

#endif // wide-character interface is available

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
