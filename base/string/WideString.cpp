/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/string/WideString.h>
#include <base/Functor.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <string.h>
  #include <stdlib.h>
#endif // flavor

#include <wchar.h>
#include <limits.h> // defines MB_LEN_MAX

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace isoc {

#if defined(_DK_SDU_MIP__BASE__WIDE)
  extern "C" int wcscmp(const wchar_t*, const wchar_t*);

  extern "C" int wcsncmp(const wchar_t*, const wchar_t*, size_t);

  // WARNING: not-compliant with standard which requires returning a non-const
  extern "C" const wchar_t* wcschr(const wchar_t*, wchar_t);

  // WARNING: not-compliant with standard which requires returning a non-const
  extern "C" const wchar_t* wcsstr(const wchar_t* restrict, const wchar_t* restrict);
#else
  inline int wcscmp(const wchar_t* ws1, const wchar_t* ws2) throw() {
    while (*ws1 && *ws2) {
      if (*ws1 < *ws2) {
        return -1;
      } else if (*ws2 > *ws2) {
        return 1;
      }
      ++ws1;
      ++ws2;
    }
    if (*ws1) {
      return -1;
    } else if (*ws2) {
      return 1;
    } else {
      return 0;
    }
  }

  inline int wcsncmp(const wchar_t* ws1, const wchar_t* ws2, size_t n) throw() {
    while (*ws1 && *ws2 && n) {
      if (*ws1 < *ws2) {
        return -1;
      } else if (*ws2 > *ws2) {
        return 1;
      }
      ++ws1;
      ++ws2;
      --n;
    }
    if (n == 0) {
      return 0;
    }
    if (*ws1) {
      return -1;
    } else if (*ws2) {
      return 1;
    } else {
      return 0;
    }
  }

  // WARNING: not-compliant with standard which requires returning a non-const
  inline const wchar_t* wcschr(const wchar_t* ws, wchar_t wc) throw() {
    while (*ws) {
      if (*ws == wc) {
        return ws; // return pointer to the first occurrence
      }
      ++ws;
    }
    return 0; // not found
  }

  // WARNING: not-compliant with standard which requires returning a non-const
  inline const wchar_t* wcsstr(const wchar_t* restrict ws1, const wchar_t* restrict ws2) throw() {
    if (*ws2) {
      return ws1;
    }
    while (true) {
      while (*ws2 && (*ws2 != *ws1)) { // skip to matching first character
        ++ws2;
      }
      if (!*ws2) {
        break;
      }
      const wchar_t* match = ws2;
      const wchar_t* substr = ws1;
      while (*substr && (*substr == *match)) {
        ++substr;
        ++match;
      }
      if (!*substr) {
        return ws2;
      }
      ++ws2;
    }
    return 0; // not found
  }
#endif // end of namespace isoc
};

#if !defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
  #warning Assuming that mbstowcs is reentrant
#endif

String WideString::getMultibyteString(const wchar_t* string) throw(NullPointer, WideStringException) {
  assert(string, NullPointer(Type::getType<WideString>()));
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, WideStringException(Type::getType<WideString>())); // maximum length exceeded
  int numberOfCharacters = terminator - string;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  int multibyteLength = ::WideCharToMultiByte(CP_UTF8, // code page
                                              0, // performance and mapping flags
                                              string, // wide-character string
                                              numberOfCharacters, // number of chars in string
                                              0, // buffer for new string
                                              0, // size of buffer
                                              0, // default for unmappable chars
                                              0 // set when default char used
  );
  ASSERT(multibyteLength > 0);
  char multibyteBuffer[multibyteLength]; // TAG: use String capacity support
  ::WideCharToMultiByte(CP_UTF8, // code page
                        0, // performance and mapping flags
                        string, // wide-character string
                        numberOfCharacters, // number of chars in string
                        multibyteBuffer, // buffer for new string
                        multibyteLength, // size of buffer
                        0, // default for unmappable chars
                        0 // set when default char used
  );
  return String(multibyteBuffer, multibyteLength); // TAG: use capacity support
#else // unix
  return String(); // TAG: FIXME
#endif // flavor
}

// TAG: need getMultibyteString() with maximum length argument

WideString::WideString() throw() : elements(DEFAULT_STRING.elements) {
}

WideString::WideString(unsigned int capacity) throw(MemoryException) : elements(0) {
  elements = new ReferenceCountedCapacityAllocator<Character>(1, GRANULARITY);
  elements->ensureCapacity(capacity + 1);
}

WideString::WideString(const WideStringLiteral& str) throw(WideStringException, MemoryException) : elements(0) {
  unsigned int length = str.getLength();
  assert(length <= MAXIMUM_LENGTH, WideStringException(this)); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), str, length); // no overlap
}

WideString::WideString(const Character* string) throw(WideStringException, MemoryException) : elements(0) {
  assert(string, WideStringException(this)); // make sure string is proper (not empty)
  const Character* terminator = find(string, MAXIMUM_LENGTH, Traits::TERMINATOR); // find terminator
  assert(terminator, WideStringException(this)); // maximum length exceeded
  int numberOfCharacters = terminator - string;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const Character* string, unsigned int maximum) throw(OutOfDomain, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  assert(string, WideStringException(this)); // make sure string is proper (not empty)
  const Character* terminator = find(string, maximum, Traits::TERMINATOR); // find terminator
  int numberOfCharacters = terminator ? (terminator - string) : maximum;
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  copy(elements->getElements(), string, numberOfCharacters); // no overlap
}

WideString::WideString(const char* string) throw(MultibyteException, MemoryException) : elements(0) {
  int numberOfCharacters = 0;
  if (string) { // is string proper (not empty)
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  numberOfCharacters = ::MultiByteToWideChar(CP_UTF8, 0, string, -1, 0, 0); // includes terminator
  assert(numberOfCharacters > 0, MultibyteException(this));
  --numberOfCharacters;
  assert(numberOfCharacters <= MAXIMUM_LENGTH, MemoryException(this));
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  ::MultiByteToWideChar(CP_UTF8, 0, string, -1, elements->getElements(), numberOfCharacters + 1); // includes terminator
#else // unix
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(0, &current, 0, &state);
#else
    size_t result = mbstowcs(0, string, 0);
#endif
    assert(result != size_t(-1), MultibyteException(this));
    assert(result <= MAXIMUM_LENGTH, MemoryException(this)); // maximum length exceeded
    numberOfCharacters = result;
  }
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  if (numberOfCharacters) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
#else
    size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
#endif
#endif // flavor
  }
}

WideString::WideString(const char* string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException) : elements(0) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  int numberOfCharacters = 0;
  if (string) { // is string proper
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(0, &current, maximum, &state);
#else
    size_t result = mbstowcs(0, string, maximum);
#endif
    assert(result != size_t(-1), MultibyteException(this));
    assert(result <= MAXIMUM_LENGTH, MemoryException(this)); // maximum length exceeded
    numberOfCharacters = minimum(static_cast<unsigned int>(result), maximum);
  }
  elements = new ReferenceCountedCapacityAllocator<Character>(numberOfCharacters + 1, GRANULARITY);
  if (numberOfCharacters) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MBSRTOWCS)
    mbstate_t state;
    clear(state); // initial state
    const char* current = string;
    size_t result = mbsrtowcs(elements->getElements(), &current, numberOfCharacters, &state);
#else
    size_t result = mbstowcs(elements->getElements(), string, numberOfCharacters);
#endif
  }
}

WideString& WideString::operator=(const WideStringLiteral& string) throw(WideStringException, MemoryException) {
  unsigned int length = string.getLength();
  assert(length <= MAXIMUM_LENGTH, WideStringException(this)); // TAG: this is not required
  elements = new ReferenceCountedCapacityAllocator<Character>(length + 1, GRANULARITY);
  copy<Character>(elements->getElements(), string, length); // no overlap
  return *this;
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
  assert(index < getLength(), OutOfRange(this));
  return getBuffer()[index];
}

void WideString::setAt(unsigned int index, Character value) throw(OutOfRange) {
  assert(index < getLength(), OutOfRange(this));
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
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  int length = getLength();
  setLength(length + minimum(str.getLength(), maximum));
  Character* buffer = elements->getElements();
  copy<Character>(buffer + length, str, minimum(str.getLength(), maximum));
  return *this;
}

WideString& WideString::append(const Character* str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException) {
  assert(maximum <= MAXIMUM_LENGTH, OutOfDomain(this)); // maximum length exceeded
  assert(str, WideStringException(this)); // make sure string is proper (not empty)
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
  return isoc::wcscmp(getElements(), str.getElements());
}

int WideString::compareTo(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException(this));
  return isoc::wcscmp(getElements(), str);
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
  return compareToIgnoreCase(getElements(), str.getElements());
  //return _wcsicmp(getElements(), str.getElements());
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str.getElements());
#else // unix
  return compareToIgnoreCase(getElements(), str.getElements());
#endif
}

int WideString::compareToIgnoreCase(const Character* str) const throw(WideStringException) {
  assert(str, WideStringException(this));
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return compareToIgnoreCase(getElements(), str);
  //return _wcsicmp(getElements(), str);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  return wcscasecmp(getElements(), str);
#else // unix
  return compareToIgnoreCase(getElements(), str);
#endif
}

bool WideString::startsWith(const WideString& prefix) const throw() {
  return !prefix.isEmpty() && (isoc::wcsncmp(getBuffer(), prefix.getBuffer(), prefix.getLength()) == 0); // null-terminators are not required
}

bool WideString::startsWith(const WideStringLiteral& prefix) const throw() {
  return (prefix.getLength() > 0) && (isoc::wcsncmp(getBuffer(), prefix, prefix.getLength()) == 0); // null-terminator is not required
}

bool WideString::endsWith(const WideString& suffix) const throw() {
  return !suffix.isEmpty() && (isoc::wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix.getBuffer(), suffix.getLength()) == 0); // null-terminators are not required
}

bool WideString::endsWith(const WideStringLiteral& suffix) const throw() {
  return (suffix.getLength() > 0) && (isoc::wcsncmp(getBuffer() + getLength() - suffix.getLength(), suffix, suffix.getLength()) == 0); // null-terminator is not required
}

int WideString::indexOf(Character ch, unsigned int start) const throw() {
  int length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const Character* buffer = getElements();
  const Character* result = isoc::wcschr(buffer + start, ch);
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
  const Character* result = isoc::wcsstr(buffer + start, str.getElements());
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
  return isoc::wcscmp(left.getElements(), right.getElements());
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Allocator<char> buffer(MB_LEN_MAX * value.getLength()); // no terminator // TAG: what is the max. char length
  int result = ::WideCharToMultiByte(CP_UTF8, 0, value.getElements(), value.getLength(), buffer.getElements(), buffer.getSize(), 0, 0);
  ASSERT(result > 0); // no errors expected
  stream.addCharacterField(buffer.getElements(), result);
#else // unix
  Allocator<char> buffer(MB_LEN_MAX * value.getLength() + 1); // remember terminator - greedy implementation
  size_t result = wcstombs(buffer.getElements(), value.getElements(), buffer.getSize());
  assert(result != size_t(-1), MultibyteException());
  stream.addCharacterField(buffer.getElements(), result);
#endif // flavor
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
