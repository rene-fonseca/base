/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Primitives.h>
#include <base/string/WideString.h>
#include <base/Functor.h>
#include <base/Architecture.h>
#include <base/string/unicode/UnicodeLookup.cpp>
#include <base/string/unicode/UnicodeFolding.cpp>
#include <base/string/Unicode.h>
#include <base/UnexpectedFailure.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: add reference to RFC 2781
// TAG: RFC 2044
// TAG: need SCSU support see http://www.unicode.org/unicode/reports/tr6/
// TAG: need support for native unicode encoding (e.g. SYSTEM_UNICODE_ENCODING)?
// TAG: need input support for something like: "asdfd\u015fsdjfhd", "&#xXXXXX", and "&#DDDDD"
// TAG: need support for width calculation. NBWSP=>0???

// TAG: GCC 3.2 bug: fails to compile
// const WideTraits::CharacterDescriptor WideTraits::characters[0]; // when 0 elements
// getArraySize(characters);

bool WideTraits::isControl(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(controlCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(controlCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < controlCharacters[index].first) {
        last = index - 1;
      } else if (character > controlCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isUpper(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(upperCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(upperCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < upperCharacters[index].first) {
        last = index - 1;
      } else if (character > upperCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isLower(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(lowerCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(lowerCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < lowerCharacters[index].first) {
        last = index - 1;
      } else if (character > lowerCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isTitle(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(titleCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(titleCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < titleCharacters[index].first) {
        last = index - 1;
      } else if (character > titleCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

unsigned int WideTraits::getFlags(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        return characters[index].flags; // found
      }
    }
  }
#endif
  return 0;
}

ucs4 WideTraits::toLower(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].lower != 0) {
          return characters[index].lower; // found and valid
        }
        break;
      }
    }
  }
#endif
  return character;
}

ucs4 WideTraits::toUpper(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].upper != 0) {
          return characters[index].upper; // found and valid
        }
        break;
      }
    }
  }
#endif
    return character;
}

ucs4 WideTraits::toTitle(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(characters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(characters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      int diff = characters[index].code - character;
      if (diff < 0) {
        last = index - 1;
      } else if (diff > 0) {
        begin = index + 1;
      } else {
        if (characters[index].title != 0) {
          return characters[index].title; // found and valid
        }
        break;
      }
    }
  }
#endif
  return character;
}

bool WideTraits::isSpace(ucs4 character) noexcept
{
  if (character == ' ') {
    return true;
  }
#if 0 // TAG: FIXME
  if (getArraySize(spaceCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(spaceCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < spaceCharacters[index].first) {
        last = index - 1;
      } else if (character > spaceCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

bool WideTraits::isDigit(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(digitCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(digitCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < digitCharacters[index].first) {
        last = index - 1;
      } else if (character > digitCharacters[index].last) {
        begin = index + 1;
      } else {
        return true; // found
      }
    }
  }
#endif
  return false;
}

int WideTraits::digitToValue(ucs4 character) noexcept
{
#if 0 // TAG: FIXME
  if (getArraySize(digitCharacters) > 0) {
    unsigned int begin = 0;
    unsigned int last = getArraySize(digitCharacters) - 1;
    while (begin <= last) {
      unsigned int index = (last + begin)/2;
      if (character < digitCharacters[index].first) {
        last = index - 1;
      } else if (character > digitCharacters[index].last) {
        begin = index + 1;
      } else {
        return (character - digitCharacters[index].first) + digitCharacters[index].value; // found
      }
    }
  }
#endif
  return -1;
}

void WideString::initialize(const char16_t* src, MemorySize _length)
{
  if (_length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  const MemorySize length = Unicode::UTF16ToUCS4(nullptr, reinterpret_cast<const utf16*>(src), _length);
  auto result = new ReferenceCountedAllocator<ucs4>(length + 1);
  Unicode::UTF16ToUCS4(result->getElements(), reinterpret_cast<const utf16*>(src), _length);
  elements = result;
}

void WideString::initialize(const wchar* src, MemorySize _length)
{
  if (!_length) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  // worst case length is nativeLength - allocate worst case and then release unused if possible
  const MemorySize length = Unicode::WCharToUCS4(nullptr, src, _length);
  auto result = new ReferenceCountedAllocator<ucs4>(length + 1);
  Unicode::WCharToUCS4(result->getElements(), src, _length);
  elements = result;
}

void WideString::initialize(const ucs4* src, MemorySize length)
{
  if (!length) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  elements = new ReferenceCountedAllocator<ucs4>(length + 1);
  base::copy<ucs4>(elements->getElements(), src, length); // no overlap
}

void WideString::initialize(const char* src, MemorySize _length)
{
  if (_length == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }

  const MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(nullptr, Cast::pointer<const uint8*>(src), _length);
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      elements->getElements(),
      reinterpret_cast<const uint8*>(src),
      _length
    );
  }
}

WideString::WideString() noexcept
  : elements(DEFAULT_STRING.elements)
{
  BASSERT(elements); // we never want a nullptr
}

WideString::WideString(Default d)
{
  // force non-nullptr elements
  auto e = new ReferenceCountedAllocator<ucs4>(1);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

WideString::WideString(MemorySize capacity)
{
  if (capacity == 0) {
    elements = DEFAULT_STRING.elements;
    return;
  }
  auto e = new ReferenceCountedAllocator<ucs4>(1, capacity);
  e->getElements()[0] = Traits::TERMINATOR;
  elements = e;
}

#if 0
WideString::WideString(ucs4 src)
{
  initialize(&src, 1);
}
#endif

WideString::WideString(const char* src)
{
  initialize(src, getNullTerminatedLength(src));
}

WideString::WideString(const char* src, MemorySize length)
{
  initialize(src, length);
}

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
WideString::WideString(const char8_t* src)
{
  initialize(reinterpret_cast<const char*>(src), getNullTerminatedLength(src));
}

WideString::WideString(const char8_t* src, MemorySize length)
{
  initialize(reinterpret_cast<const char*>(src), length);
}
#endif

WideString::WideString(const char16_t* src)
{
  initialize(src, getNullTerminatedLength(src));
}

WideString::WideString(const char16_t* src, MemorySize length)
{
  initialize(src, length);
}

WideString::WideString(const wchar* src)
{
  initialize(src, getNullTerminatedLength(src));
}

WideString::WideString(const wchar* src, MemorySize length)
{
  initialize(src, length);
}

WideString::WideString(const ucs4* src)
{
  initialize(src, getNullTerminatedLength(src));
}

WideString::WideString(const ucs4* src, MemorySize length)
{
  initialize(src, length);
}

WideString::WideString(const Literal& src)
{
  initialize(src.getValue(), src.getLength());
}

WideString::WideString(const WideLiteral& src)
{
  initialize(src.getValue(), src.getLength());
}

WideString::WideString(const NativeWideString& src)
{
  initialize(src.getValue(), src.getLength());
}

WideString::WideString(const NativeWideString& src, MemorySize maximum)
{
  const MemorySize nativeLength = getNullTerminatedLength(src.getValue(), maximum);
  initialize(src, nativeLength);
}

WideString::WideString(const String& string)
{
  MemorySize multibyteLength = string.getLength();
  MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(nullptr, string.getBytes(), multibyteLength);
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(elements->getElements(), string.getBytes(), multibyteLength);
  }
}

WideString::WideString(const NativeString& string)
{
  if (!string.getValue()) { // is string null
    elements = DEFAULT_STRING.elements;
    return;
  }
  
  const MemorySize multibyteLength = getNullTerminatedLength(string.getValue());

  MemorySize numberOfCharacters = Unicode::UTF8ToUCS4(
    nullptr,
    Cast::pointer<const uint8*>(string.getValue()),
    multibyteLength
  );
  elements = new ReferenceCountedAllocator<ucs4>(numberOfCharacters + 1);
  if (numberOfCharacters) {
    Unicode::UTF8ToUCS4(
      Cast::pointer<ucs4*>(elements->getElements()),
      Cast::pointer<const uint8*>(string.getValue()),
      multibyteLength
    );
  }
}

WideString::WideString(StringOutputStream& stream)
{
  operator=(stream.toString());
}

WideString::WideString(FormatOutputStream& stream)
{
  operator=(stream.toString());
}

WideString::WideString(const NativeString& src, MemorySize maximum)
{
  initialize(src.getValue(), getNullTerminatedLength(src.getValue(), maximum));
}

WideString& WideString::operator=(const Literal& assign)
{
  initialize(assign.getValue(), assign.getLength());
  return *this;
}

WideString& WideString::operator=(const WideLiteral& assign)
{
  initialize(assign.getValue(), assign.getLength());
  return *this;
}

WideString& WideString::operator=(const char* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

#if defined(_COM_AZURE_DEV__BASE__CPP_CHAR8_T)
WideString& WideString::operator=(const char8_t* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}
#endif

WideString& WideString::operator=(const wchar* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

WideString& WideString::operator=(const char16_t* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

WideString& WideString::operator=(const char32_t* assign)
{
  initialize(assign, getNullTerminatedLength(assign));
  return *this;
}

WideString& WideString::operator=(const String& assign)
{
  initialize(assign.native(), assign.getLength());
  return *this;
}

WideString& WideString::operator=(const StringOutputStream& assign)
{
  operator=(assign.getString());
  return *this;
}

bool WideString::isASCII() const noexcept
{
  const ucs4* i = getBuffer();
  const ucs4* end = i + getLength();
  while (i < end) {
    if (!ASCIITraits::isASCII(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isMultiReferenced() const noexcept
{
  return elements.isMultiReferenced();
}

void WideString::ensureCapacity(MemorySize capacity)
{
  if (!elements.isMultiReferenced()) {
    elements->ensureCapacity(capacity);
    return;
  }
  
  MemorySize length = getLength();
  auto e = new ReferenceCountedAllocator<Char>(length + 1, capacity);
  base::copy<Char>(e->getElements(), elements->getElements(), length + 1); // no overlap
  elements = e;
}

ucs4* WideString::getBuffer()
{
  elements.copyOnWrite();
  return elements->getElements();
}

ucs4* WideString::getBuffer(MemorySize length)
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
  auto e = new ReferenceCountedAllocator<Char>(length + 1); // reset capacity
  auto dest = e->getElements();
  if (length < originalLength) {
    base::copy<Char>(dest, elements->getElements(), length); // no overlap
  } else {
    base::copy<Char>(dest, elements->getElements(), originalLength); // no overlap
  }
  dest[length] = Traits::TERMINATOR;
  elements = e;

  return dest;
}

bool WideString::hasNull() const noexcept
{
  const Char* i = getBuffer();
  const Char* end = i + getLength();
  while (i != end) {
    if (*i == 0) {
      return true;
    }
    ++i;
  }
  return false;
}

void WideString::clear()
{
  elements = DEFAULT_STRING.elements;
}

WideString WideString::copy() const
{
  WideString result = *this;
  result.getBuffer();
  return result;
}

void WideString::garbageCollect()
{
  elements->garbageCollect(); // no need to do copyOnWrite
}

void WideString::forceToLength(MemorySize length)
{
  const MemorySize originalLength = getLength();
  setLength(length);
  if (originalLength < length) {
    auto ch = Traits::TERMINATOR;
    fill(getBuffer() + originalLength, length - originalLength, ch);
  }
}

ucs4 WideString::getAt(MemorySize index) const
{
  if (index >= getLength()) {
    _throw OutOfRange(this);
  }
  return getBuffer()[index];
}

void WideString::setAt(MemorySize index, ucs4 value)
{
  if (index >= getLength()) {
    _throw OutOfRange(this);
  }
  getBuffer()[index] = value; // allow terminated within string
}

WideString& WideString::remove(MemorySize start, MemorySize end)
{
  const MemorySize length = getLength();
  if ((start < end) && (start < length)) { // protect against some cases
    if (end >= length) {
      elements.copyOnWrite(); // we are about to modify the buffer
      elements->setSize(start + 1); // remove section from end of string
    } else {
      // remove section from middle of string
      ucs4* buffer = getBuffer(); // we are about to modify the buffer
      move(buffer + start, buffer + end, length - end); // move end of string
      elements->setSize(length - (end - start) + 1); // remember space for terminator
    }
    elements->getElements()[getLength()] = Traits::TERMINATOR;
  }
  return *this;
}

WideString& WideString::removeFrom(MemorySize start)
{
  if (start < getLength()) { // protect against some cases
    getBuffer(start); // just resize
  }
  return *this;
}

WideString& WideString::insert(MemorySize index, ucs4 ch)
{
  MemorySize length = getLength();
  setLength(length + 1);
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    buffer[length] = ch; // insert section at end of string
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + 1, buffer + index, length - index);
    buffer[index] = ch;
  }
  return *this;
}

WideString& WideString::insert(MemorySize index, const WideString& str)
{
  MemorySize length = getLength();
  MemorySize strlength = str.getLength();
  setLength(length + strlength); // TAG: also protects against self insertion - but can this be circumvented
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    base::copy(buffer + length, str.getBuffer(), strlength);
  } else {
    // insert section in middle or beginning of string
    move(buffer + index + strlength, buffer + index, length - index); // move end of string
    base::copy(buffer + index, str.getBuffer(), strlength);
  }
  return *this;
}

WideString& WideString::insert(MemorySize index, const ConstSpan<Char>& span)
{
  const MemorySize length = getLength();
  const MemorySize newLength = length + span.getSize();
  auto buffer = getBuffer(newLength);
  if (index >= length) {
    // insert section at end of string
    copyTo(buffer + length, span);
  } else {
    // insert section in middle or beginning of string
    move<Char>(
      buffer + index + span.getSize(),
      buffer + index,
      length - index
    );
    copyTo(buffer + index, span);
  }
  buffer[newLength] = Traits::TERMINATOR;
  return *this;
}

WideString& WideString::insert(MemorySize index, const ConstSpan<wchar>& src)
{
  const MemorySize length = getLength();
  const MemorySize srcLength = src.getSize();
  ucs4* buffer = elements->getElements();
  if (index >= length) {
    // insert section at end of string
    const MemorySize _length = Unicode::WCharToUCS4(nullptr, src.begin(), srcLength);
    setLength(length + _length);
    Unicode::WCharToUCS4(buffer + length, src.begin(), srcLength);
  } else {
    // insert section in middle or beginning of string
    const MemorySize _length = Unicode::WCharToUCS4(nullptr, src.begin(), srcLength);
    setLength(length + _length);
    move<ucs4>(buffer + index + _length, buffer + index, length - index);
    Unicode::WCharToUCS4(buffer + index, src.begin(), srcLength);
  }
  return *this;
}

WideString& WideString::insert(MemorySize index, const WideLiteral& src)
{
  return insert(index, ConstSpan<wchar>(src.getValue(), src.getLength()));
}

WideString& WideString::insert(MemorySize index, const NativeWideString& src)
{
  return insert(index, ConstSpan<wchar>(src.getValue(), src.getLength()));
}

WideString& WideString::append(const WideLiteral& src)
{
  return insert(getLength(), ConstSpan<wchar>(src.getValue(), src.getLength()));
}

WideString& WideString::append(const WideLiteral& literal, MemorySize maximum)
{
  // not good since we need to handle multi code chars return insert(getLength(), ConstSpan<wchar>(src.getValue(), minimum(src.getLength()), maximum));
  const MemorySize length = getLength();
  const MemorySize nativeLength = literal.getLength();
  ucs4* buffer = elements->getElements();
  MemorySize _length = Unicode::WCharToUCS4(nullptr, literal.getValue(), nativeLength);
  _length = minimum<MemorySize>(_length, maximum);
  setLength(length + _length);
  Unicode::WCharToUCS4(buffer + length, literal.getValue(), nativeLength);
  return *this;
}

WideString& WideString::append(const wchar* string, MemorySize maximum)
{
  bassert(
    string,
    StringException(this)
  ); // make sure string is proper (not empty)
  const MemorySize nativeLength = getNullTerminatedLength(string, maximum);
  MemorySize length = getLength();
  ucs4* buffer = elements->getElements();
  MemorySize stringLength = Unicode::WCharToUCS4(nullptr, string, nativeLength);
  stringLength = minimum<MemorySize>(stringLength, maximum);
  setLength(length + stringLength);
  Unicode::WCharToUCS4(buffer + length, string, stringLength);
  return *this;
}

WideString& WideString::append(const ConstSpan<Char>& src)
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

WideString& WideString::replace(
  MemorySize start,
  MemorySize end,
  const WideString& str)
{
  // need better implementation
  if (start <= end) {
    ensureCapacity(getLength() - (end - start + 1) + str.getLength());
    remove(start, end);
    insert(start, str);
  }
  return *this;
}

MemorySize WideString::replaceAll(const WideString& fromStr, const WideString& toStr)
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

WideString WideString::substring(MemorySize start, MemorySize end) const
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
    WideString result(lengthOfSubstring);
    result.setLength(lengthOfSubstring);
    base::copy(result.getBuffer(), getBuffer() + start, lengthOfSubstring); // buffers do not overlap
    return result;
  } else {
    // C++: could be useful to have a "This/Auto" type to get the class automatically
    return WideString(); // return empty string
  }
}

WideString& WideString::operator-=(const WideString& suffix)
{
  if (endsWith(suffix)) {
    setLength(suffix.getLength());
  }
  return *this;
}

WideString& WideString::reverse() noexcept
{
  Char* p = getBuffer();
  Char* q = &p[getLength() - 1]; // last char - empty string => q < p
  Char temp = 0;
  while (p < q) { // until middle of string has been reached
    temp = *p;
    *p = *q;
    *q = temp;
    ++p;
    --q;
  }
  return *this;
}

WideString& WideString::toLowerCase() noexcept
{
  transform(getBuffer(), getLength(), Traits::ToLowerCase());
  return *this;
}

WideString& WideString::toUpperCase() noexcept
{
  transform(getBuffer(), getLength(), Traits::ToUpperCase());
  return *this;
}

int WideString::compareTo(const WideString& string) const noexcept
{
  // both strings may contain multiple zeros
  MemorySize leftLength = getLength();
  MemorySize rightLength = string.getLength();
  const Char* left = getBuffer();
  const Char* right = string.getBuffer();
  const Char* end = left + minimum(leftLength, rightLength);
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

int WideString::compareTo(const WideLiteral& literal) const /*noexcept*/
{
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right =
      Cast::pointer<const utf16*>(literal.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code.")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence.")
        );
        value = 0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs4* right = Cast::pointer<const ucs4*>(literal.getValue());
    while (*left && *right) {
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(*right);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else {
    _throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

int WideString::compareTo(const wchar* string) const /*noexcept*/
{
  if (!string) {
    return false;
  }
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right = Cast::pointer<const utf16*>(string);
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code.")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence.")
        );
        value = 0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const ucs4* right = Cast::pointer<const ucs4*>(string);
    while (*left && *right) {
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(*right);
      if (temp != 0) {
        return temp;
      }
      ++left;
      ++right;
    }
    if (*left) {
      return 1;
    } else if (*right) {
      return -1;
    } else {
      return 0; // equal
    }
  } else {
    _throw UnexpectedFailure("Unsupported native wide character representation.", this);
  }
}

int WideString::compareToIgnoreCase(const ucs4* left, const ucs4* right) noexcept
{
  // remember caseFold(*left)
  // remember caseFold(*right)
  // while in small buffer to binary compare
  
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

int WideString::compareToIgnoreCase(const WideString& string) const noexcept
{
  return compareToIgnoreCase(getElements(), string.getElements());
}

// TAG: need support for WideLiteral
// int WideString::compareToIgnoreCase(const WideLiteral& literal) const noexcept;

bool WideString::startsWith(const WideString& prefix) const /*noexcept*/
{
  const MemorySize prefixLength = prefix.getLength();
  return (prefixLength > 0) && (prefixLength <= getLength()) &&
    (compare(getBuffer(), prefix.getBuffer(), prefixLength) == 0);
}

bool WideString::startsWith(const WideLiteral& prefix) const /*noexcept*/
{
  if (sizeof(wchar) == sizeof(utf16)) {
    const ucs4* left = getElements(); // make sure string is terminated
    const utf16* right = Cast::pointer<const utf16*>(prefix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code.")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence.")
        );
        value =
          0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return false;
      }
      ++left;
      ++right;
    }
    return *right == 0;
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    MemorySize prefixLength = prefix.getLength();
    return (prefixLength > 0) && (prefixLength <= getLength()) &&
      (
        compare<ucs4>(
          getBuffer(),
          Cast::pointer<const ucs4*>(prefix.getValue()),
          prefixLength
        ) == 0
      );
  } else {
    _throw UnexpectedFailure(
      "Unsupported native wide character representation",
      this
    );
  }
}

bool WideString::endsWith(const WideString& suffix) const /*noexcept*/
{
  const MemorySize length = getLength();
  const MemorySize suffixLength = suffix.getLength();
  return (suffixLength > 0) && (suffixLength <= length) &&
    (compare(getBuffer() + length - suffixLength, suffix.getBuffer(), suffixLength) == 0);
}

bool WideString::endsWith(const WideLiteral& suffix) const /*noexcept*/
{
  MemorySize suffixLength = suffix.getLength();
  if (sizeof(wchar) == sizeof(utf16)) {
    if (suffixLength > getLength()) {
      return false;
    }
    // make sure string is terminated
    const ucs4* left = getElements() + getLength() - suffixLength;
    const utf16* right = Cast::pointer<const utf16*>(suffix.getValue());
    while (*left && *right) {
      ucs4 value = *right;
      bassert(
        !((value >= 0xdc00) && (value <= 0xdfff)),
        StringException("Reserved code.")
      );
      if ((value >= 0xd800) && (value <= 0xdbff)) {
        const ucs4 lowSurrogate = *right++;
        bassert(
          (lowSurrogate >= 0xdc00) && (lowSurrogate <= 0xdfff),
          StringException("Invalid sequence.")
        );
        value = 0x00010000U + (value - 0xd800) * 0x0400 + (lowSurrogate - 0xdc00);
      }
      int32 temp = static_cast<int32>(*left) - static_cast<int32>(value);
      if (temp != 0) {
        return false;
      }
      ++left;
      ++right;
    }
    return *right == 0;
  } else if (sizeof(wchar) == sizeof(ucs4)) {
    return (suffixLength > 0) && (suffixLength <= getLength()) &&
      (
        compare<ucs4>(
          getBuffer() + getLength() - suffixLength,
          Cast::pointer<const ucs4*>(suffix.getValue()),
          suffixLength
        ) == 0
      );
  } else {
    _throw UnexpectedFailure(
      "Unsupported native wide character representation.",
      this
    );
  }
}

MemoryDiff WideString::indexOf(ucs4 ch, MemorySize start) const noexcept
{
  const MemorySize length = getLength();
  if (start >= length) {
    return -1; // not found
  }

  const auto buffer = getBuffer();
  MemorySize count = length - start;

  const Char* result = find(buffer + start, count, ch);
  if (result) { // did we find the value
    return result - buffer; // return index
  } else {
    return -1; // not found
  }
}

MemoryDiff WideString::indexOf(const WideString& string, MemorySize start) const noexcept
{
  MemorySize length = getLength();
  MemorySize sublength = string.getLength();

  if ((start >= length) || (sublength == 0) || (sublength > length)) {
    return -1; // not found
  }

  const ucs4* right = string.getElements();
  if (!*right) {
    return 0;
  }
  
  const ucs4* begin = getElements();
  const ucs4* left = begin + start;
  
  while (true) {
    while (*left && (*right != *left)) { // skip to matching first character
      ++left;
    }
    if (!*left) { // have we reached the end
      break;
    }
    const ucs4* match = left;
    const ucs4* substr = right;
    while (*substr && (*substr == *match)) {
      ++match;
      ++substr;
    }
    if (!*substr) {
      return left - begin;
    }
    ++left;
  }
  return -1; // not found
}

MemoryDiff WideString::lastIndexOf(ucs4 ch, MemorySize start) const noexcept
{
  // examined cases: getLength() = 0, getLength() = 1, getLength() > 1
  const Char* buffer = getBuffer();
  const Char* p = &buffer[(start < getLength()) ? start : getLength() - 1]; // validate start
  while (p >= buffer) {
    if (*p == ch) { // do we have a match
      return p - buffer; // char found at index;
    }
    --p;
  }
  return -1; // not found
}

MemoryDiff WideString::lastIndexOf(const WideString& string, MemorySize start) const noexcept
{
  if ((string.isEmpty()) || (string.getLength() > getLength())) { // eliminate some cases
    return -1; // not found
  }
  if (start >= getLength()) { // validate start - 0 <= start < getLength()
    start = getLength() - 1;
  }

  const Char* substring = string.getBuffer();
  const Char* buffer = getBuffer();
  const Char* begin = &buffer[start - string.getLength() + 1]; // beginning of current substring
  const Char* last = &buffer[start]; // end of current substring - buffer <= begin <= last

  // calculate hash of strings
  unsigned int hash = 0;
  unsigned int mask = 1;
  unsigned int match = 0;
  const Char* p = substring;
  const Char* q = begin;
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

MemorySize WideString::count(ucs4 ch, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(ch, start)) >= 0) { // until not found
    ++count;
    start = result + 1;
  }
  return count;
}

MemorySize WideString::count(const WideString& string, MemorySize start) const noexcept
{
  MemoryDiff result = 0;
  MemorySize count = 0;
  while ((result = indexOf(string, start)) >= 0) { // until not found - works for empty string
    ++count;
    start = result + string.getLength();
  }
  return count;
}

ucs4* WideString::getElements()
{
  Char* result = getBuffer(); // copy on write
  return result;
}

WideString& WideString::trim(ucs4 character)
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

bool WideString::isUpperCased() const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isUpper(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isLowerCased() const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isLower(*i++)) {
      return false;
    }
  }
  return true;
}

bool WideString::isTitleCased() const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  while (i != end) {
    if (!Traits::isTitle(*i++)) {
      return false;
    }
  }
  return true;
}

// http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/index.html
MemoryDiff WideString::search(const WideString& substring, MemorySize start) const noexcept
{
  const MemorySize length = substring.getLength();
  if ((length == 0) || (getLength() < (start + length))) {
    return -1; // not found
  }

  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED(); // TAG: handle any code > 0x255
  
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

Array<WideString> WideString::split(Char separator, bool group) const
{
  Array<WideString> result;
  
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

WideString operator+(const WideString& left, const char* right)
{
  MemorySize r = getNullTerminatedLength(right);
  WideString result(left.getLength() + r);
  return result.append(left).append(right);
}

WideString operator+(const char* left, const WideString& right)
{
  MemorySize l = getNullTerminatedLength(left);
  WideString result(l + right.getLength());
  return result.append(left).append(right);
}

WideString operator+(const WideString& left, const wchar* right)
{
  MemorySize r = getNullTerminatedLength(right);
  WideString result(left.getLength() + r);
  return result.append(left).append(right);
}

WideString operator+(const wchar* left, const WideString& right)
{
  MemorySize l = getNullTerminatedLength(left);
  WideString result(l + right.getLength());
  return result.append(left).append(right);
}

WideString operator-(const WideString& left, const WideString& right)
{
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength()); // return copy of left without suffix
  } else {
    return WideString(left); // return copy of left
  }
}

WideString operator*(const WideString& src, unsigned int count)
{
  WideString result;
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
int compare<WideString>(const WideString& left, const WideString& right)
{
  return left.compareTo(right);
}

unsigned long Hash<WideString>::operator()(const WideString& value) noexcept
{
  const ucs4* src = value.getElements();
  const ucs4* end = src + value.getLength();
  unsigned long result = 5381;
  while (src < end) {
    result = /*33*/ 31 /*65599*/ * result + static_cast<unsigned int>(*src++);
  }
  return result;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value)
{
  stream.addCharacterField(value.getElements(), value.getLength());
  return stream;
}

bool WideString::isValidUnicode(bool iso) const noexcept
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  if (iso) {
    while (i != end) {
      if (*i > Unicode::MAX_ISO) {
        return false;
      }
      ++i;
    }
  } else {
    while (i != end) {
      if (*i > Unicode::MAX) {
        return false;
      }
      ++i;
    }
  }
  return true;
}

WideString WideString::getValidUnicode(bool iso) const
{
  ReadIterator i = getBeginReadIterator();
  ReadIterator end = getEndReadIterator();
  if (iso) {
    while (i != end) {
      if (*i > Unicode::MAX_ISO) {
        break;
      }
      ++i;
    }
  } else {
    while (i != end) {
      if (*i > Unicode::MAX) {
        break;
      }
      ++i;
    }
  }
  if (i == end) {
    return *this;
  }
 
  WideString result(getLength());
  for (ReadIterator j = getBeginReadIterator(); j != i; ++j) {
    result.append(*j);
  }
  
  if (iso) {
    while (i != end) {
      if (*i <= Unicode::MAX_ISO) {
        result.append(*i);
      }
      ++i;
    }
  } else {
    while (i != end) {
      if (*i <= Unicode::MAX) {
        result.append(*i);
      }
      ++i;
    }
  }

  return result;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(WideString) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/string");
  TEST_IMPACT(CRITICAL)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    WideString a;
    TEST_ASSERT(!a);
    TEST_ASSERT(a.isEmpty());
    WideString b("literal");
    a = "qwerty";
    TEST_ASSERT(a);
    TEST_ASSERT(!a.isEmpty());
    auto c = a + b;
    TEST_ASSERT(c == "qwertyliteral");
    
    TEST_ASSERT(c.indexOf('l') == 6);
    TEST_ASSERT(c.indexOf("literal") == 6);
    TEST_ASSERT(c.lastIndexOf('y') == 5);
    // TEST_ASSERT(c.lastIndexOf("literal") == 6);

    a = L"From wide string";
    String s = a;
    WideString w = s;
    TEST_ASSERT(w == WideString(L"From wide string"));
    
    a = u8"123";
    a = u"123";
    a = U"123";

#if 0
    c = WideString();
    // c.clear();
    c.append(L"Hello");
    c.append(L" ");
    c.append(L"World!");
    TEST_ASSERT(c == L"Hello World!");
    TEST_PRINT(c);
    c.reverse();
    TEST_PRINT(c);
#endif
    
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

TEST_REGISTER(WideString);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
