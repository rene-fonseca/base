/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/mem/Reference.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/io/IOException.h>
#include <base/string/StringException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/iterator/UTF8Iterator.h>
#include <base/collection/Hash.h>
#include <base/Literal.h>
#include <string>
#include <ctype.h> // TAG: alien header
#include <string.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template<class TYPE> class Array;
class FormatOutputStream;
class StringOutputStream;

/**
  Default character manipulators.

  @short ASCII character characteristics
  @version 1.0
*/
class _COM_AZURE_DEV__BASE__API CharTraits {
public:
  
  typedef char Char;

  /** Specifies the terminator for NULL-terminated strings. */
  static const char TERMINATOR = '\0';

  /** Returns true if the character an alphabetic character. */
  static inline bool isAlpha(char character) noexcept
  {
    return isalpha(character);
  }
  
  /** Returns true if the character an alphabetic character or a digit. */
  static inline bool isAlphaNum(char character) noexcept
  {
    return isalnum(character);
  }

  /** Returns true if the character is lowercase. */
  static inline bool isLower(char character) noexcept
  {
    return islower(character);
  }
  
  /** Returns true if the character is uppercase. */
  static inline bool isUpper(char character) noexcept
  {
    return isupper(character);
  }
  
  /** Returns true if the character is a digit. */
  static inline bool isDigit(char character) noexcept
  {
    return isdigit(character);
  }
  
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(char character) noexcept
  {
    return isxdigit(character);
  }
  
  /** Returns true if the character is a white space. */
  static inline bool isSpace(char character) noexcept
  {
    return isspace(character);
  }
  
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(char character) noexcept
  {
    return ispunct(character);
  }
  
  /** Returns true if the character is printable. */
  static inline bool isPrintable(char character) noexcept
  {
    return isprint(character);
  }
  
  /** Returns true if the character is a visible character. */
  static inline bool isGraph(char character) noexcept
  {
    return isgraph(character);
  }
  
  /** Returns true if the character is a control character. */
  static inline bool isControl(char character) noexcept
  {
    return iscntrl(character);
  }
  
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(char character) noexcept
  {
    return  !(character & ~0177U);
  }

  /** Converts the character to lowercase. */
  static inline char toLower(char character) noexcept
  {
    return tolower(character);
  }
  
  /** Converts the character to uppercase. */
  static inline char toUpper(char character) noexcept
  {
    return toupper(character);
  }

  class _COM_AZURE_DEV__BASE__API ToLowerCase {
  public:
    
    inline char operator()(char value) const noexcept
    {
      return tolower(value);
    }
  };

  class _COM_AZURE_DEV__BASE__API ToUpperCase {
  public:
    
    inline char operator()(char value) const noexcept
    {
      return toupper(value);
    }
  };
};

/**
  String class. The first modifing operation on a string may force the internal
  buffer to be duplicated. The implementation is currently NOT MT-safe.

  @short String
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API String : public virtual Object {
  friend void swapper<String>(String& a, String& b);
public:

  /** Character specific properties and manipulators. */
  typedef CharTraits Traits;
  
  /** Specifies the string terminator. */
  static const char TERMINATOR = '\0';
  /**
    Specifies the granularity of the capacity. Guaranteed to be greater than 0.
  */
  static const MemorySize GRANULARITY = 16;
  /**
    Specifies the maximum length of any string. Guarantees that an int can hold
    the length of the string.
  */
  static const MemorySize MAXIMUM_LENGTH =
    ((PrimitiveTraits<int>::MAXIMUM - sizeof(TERMINATOR))/GRANULARITY)*GRANULARITY;

  typedef ReferenceCountedCapacityAllocator<char>::Iterator Iterator;
  typedef ReferenceCountedCapacityAllocator<char>::ReadIterator ReadIterator;
  typedef ReferenceCountedCapacityAllocator<char>::Enumerator Enumerator;
  typedef ReferenceCountedCapacityAllocator<char>::ReadEnumerator ReadEnumerator;
private:

  /**
    The default string. This is used to avoid multiple allocations of empty
    string buffers.
  */
  static const String DEFAULT_STRING;

  /*
    Reference to an element within a string.
  */
  class _COM_AZURE_DEV__BASE__API Element {
    friend class String;
  private:
    
    String& string;
    MemorySize index = 0;
    
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();
    
    inline Element(String& _string, MemorySize _index) throw()
      : string(_string), index(_index) {
    }
  public:
    
    inline Element& operator=(char value) throw(OutOfRange) {
      string.setAt(index, value);
      return *this;
    }
    
    inline operator char() const throw(OutOfRange) {
      return string.getAt(index);
    }
  };

  /**
    Reference counted buffer holding NULL-terminated string. The array is
    guarantied to be non-empty when the string has been initialized.
  */
  Reference<ReferenceCountedCapacityAllocator<char> > elements;
protected:

  /**
    Initializes string.
  */
  void initialize(const char* string, MemorySize length);
  
  /**
    Returns a modifiable buffer. Forces copy of internal buffer if shared by
    multiple strings.
  */
  char* getBuffer();
  
  /** Returns buffer for modification and resizes at the same time. */
  char* getBuffer(MemorySize length) throw(StringException, MemoryException);

  /**
    Returns a non-modifiable buffer.
  */
  inline const char* getBuffer() const noexcept {
    return elements->getElements();
  }
  
  /**
    Sets the length of the string.
  */
  inline void setLength(MemorySize length) throw(StringException, MemoryException)
  {
    getBuffer(length);
  }
  
  /**
    Compare two NULL-terminated strings.
  */
  static int compareToIgnoreCase(const char* left, const char* right) throw();
public:
  
  /**
    Initializes an empty string.
  */
  String() noexcept;

  /**
    Initializes a string with no characters in it, initial capacity and
    granularity of capacity.
    
    @param capacity The initial capacity.
  */
  explicit String(MemorySize capacity);

  class _COM_AZURE_DEV__BASE__API Default {};
  
  /** Forces a non-null empty string. Avoid this. */
  String(Default d);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string String literal.
  */
  String(const Literal& string) throw(StringException, MemoryException);

  String(const std::string& string) throw(StringException, MemoryException);

  String(const std::wstring& string) throw(StringException, MemoryException);

#if 0 // TAG: bad for now due to match with structs also - or static_cast<const char*>() is required
  /**
    Initializes the string from a string literal. Implicit initialization is
    allowed.
    
    @param literal String literal.
  */
  template<MemorySize SIZE>
  inline String(const char (&literal)[SIZE])
  {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    initialize(literal, SIZE - 1);
  }
#endif
  
  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
  */
  String(const NativeString& string);

  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
  */
  String(const char* string);

  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string (string) exceeds the maximum length (n) only the first n
    characters are used. Raises StringException is string exceeds the maximum
    allowed length.
    
    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
    @param maximum Specifies the maximum length.
  */
  String(
    const NativeString& string,
    MemorySize maximum) throw(StringException, MemoryException);
  
  /**
    Initializes string from other string.
  */
  inline String(const String& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Initializes string from other string.
  */
  String(String&& move) noexcept
    : elements(move.elements) {
    move.elements = DEFAULT_STRING.elements; // make empty so we may avoid future copyOnWrite()
  }

  /**
    Initializes string from string output stream. Forces flush().
  */
  String(StringOutputStream& stream);

  /**
    Initializes string from string output stream via FormatOutputStream. Forces flush(). Throws exception is not a StringOutputStream.
  */
  String(FormatOutputStream& stream);

  /**
    Assignment of string to string.
  */
  inline String& operator=(const String& copy) noexcept
  {
    elements = copy.elements; // self assignment handled by automation pointer
    return *this;
  }

  String& operator=(String&& move) noexcept
  {
    if (this != &move) { // self assigment not allowed
      elements = std::move(move.elements);
      move.elements = DEFAULT_STRING.elements; // make empty so we may avoid future copyOnWrite()
    }
    return *this;
  }

  /**
    Assignment of string literal to string.
  */
  String& operator=(const Literal& eq) throw(StringException, MemoryException);

  template<MemorySize SIZE>
  inline String& operator=(const char (&literal)[SIZE])
  {
    if (Constraint<(SIZE < (MAXIMUM_LENGTH + 1))>::UNSPECIFIED) {}
    return operator=(Literal(literal));
  }
  
  /**
    Assignment of string to NULL-terminated string.
  */
  String& operator=(const NativeString& string) throw(StringException, MemoryException);
  
    /**
    Assignment of string to NULL-terminated string.
  */
  String& operator=(const char* string) throw(StringException, MemoryException);

  /**
    Returns the number of characters in the string.
  */
  inline MemorySize getLength() const noexcept
  {
    return elements->getSize() - 1; // exclude null terminator
  }

  /**
    Returns true if the string does not contain characters.
  */
  inline bool isEmpty() const noexcept
  {
    return elements->getSize() == 1;
  }

  /**
    Returns true if the string contains characters.
  */
  inline bool isProper() const noexcept
  {
    return elements->getSize() > 1;
  }

  /**
    Returns true if the string is an ASCII string.
  */
  bool isASCII() const throw();
  
  /**
    Returns the capacity of the string.
  */
  inline MemorySize getCapacity() const noexcept
  {
    return elements->getCapacity();
  }

  /**
    Ensures that the capacity of the buffer is at least equal to the specified
    minimum. This applies to all shared strings.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(MemorySize capacity);

  /** Clears the string. */
  void clear();

  /** Returns a forced copy of the string. */
  String copy() const;

  /**
    Releases any unused capacity of the string. This applies to all shared
    strings.
  */
  void garbageCollect() throw();

  /**
    Returns the granularity.
  */
  MemorySize getGranularity() const throw();

  /**
    Sets the granularity.
  */
  void setGranularity(MemorySize granularity) throw();

  /**
    Sets the length of the string without initializing the elements.
  */
  void forceToLength(MemorySize length) throw(StringException, MemoryException);

// *************************************************************************
//   TRAVERSE SECTION
// *************************************************************************

  /**
    Returns the first element of the string as a modifying iterator.
  */
  inline Iterator getBeginIterator() noexcept
  {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the string as a modifying iterator.
  */
  inline Iterator getEndIterator() noexcept
  {
    return elements->getEndIterator() - 1; // remember terminator
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const noexcept
  {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const noexcept
  {
    return elements->getEndReadIterator() - 1; // remember terminator
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator begin() const noexcept
  {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator end() const noexcept
  {
    return elements->getEndReadIterator() - 1; // remember terminator
  }

  /**
    Returns a modifying enumerator of the string.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the string.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return elements->getReadEnumerator();
  }

  /** Returns a valid UTF-8 string by discarding bad codes. */
  String getValidUTF8() const;

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline UTF8Iterator getUTF8BeginReadIterator() const noexcept
  {
    return reinterpret_cast<const uint8*>(native());
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline UTF8Iterator getUTF8EndReadIterator() const noexcept
  {
    return reinterpret_cast<const uint8*>(native()) + getLength();
  }

  /**
    Returns UTF-8 non-modifying enumerator.
  */
  inline UTF8Enumerator getUTF8ReadEnumerator() const noexcept
  {
    return UTF8Enumerator(getUTF8BeginReadIterator(), getUTF8EndReadIterator());
  }

  /** UTF-8 string. */
  class UTF8String {
  private:

    const uint8* src = nullptr;
    const MemorySize size = 0;
  public:

    inline UTF8String() noexcept
    {
    }

    UTF8String(const uint8* _src) noexcept
      : src(_src), size(getNullTerminatedLength(src))
    {
    }

    inline UTF8String(const uint8* _src, MemorySize _size = 0) noexcept
      : src(_src), size(_size)
    {
    }

    inline UTF8String(const char* _src, MemorySize _size = 0) noexcept
      : src(reinterpret_cast<const uint8*>(_src)), size(_size)
    {
    }

    inline UTF8String(const uint8* _src, const uint8* _end) noexcept
      : src(_src), size(_end - _src)
    {
    }

    inline UTF8String(const char* _src, const char* _end) noexcept
      : src(reinterpret_cast<const uint8*>(_src)), size(_end - _src)
    {
    }

    /** Returns the number of UTF-8 encoded characters. Not the number of bytes! */
    MemorySize getLength() const noexcept;

    /** Returns true if the bytes are valid UTF-8 encoded characters. And if no disallowed UCS4 codes are used. */
    bool isValidUTF8() const noexcept;

    inline UTF8Iterator begin() const noexcept
    {
      return src;
    }

    inline UTF8Iterator end() const noexcept
    {
      return src + size;
    }
  };

  /** Returns UTF-8 string. */
  UTF8String getUTF8String() const noexcept
  {
    return UTF8String(getElements(), getLength());
  }

// *************************************************************************
//   CHARACTER SECTION
// *************************************************************************

  /**
    Returns the character at the specified index in this string. Raises
    OutOfRange if index exceeds the length of the string.
  */
  char getAt(MemorySize index) const throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\\0') then the string is cut off from
    the specified index. Raises OutOfRange if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(MemorySize index, char value) throw(OutOfRange);

  /**
    Returns a reference to character at the specified index. Raises
    OutOfRange if index exceeds the length of the string.
  */
  Element operator[](MemorySize index) throw(OutOfRange) {
    return Element(*this, index);
  }

  /** Returns true if non-empty. */
  inline operator bool() const noexcept {
    return getLength() != 0;
  }

  /**
    Returns the character at the specified index. Raises OutOfRange if index
    exceeds the length of the string.
  */
  inline char operator[](MemorySize index) const throw(OutOfRange) {
    return getAt(index);
  }

// *************************************************************************
//   SUBSTRING SECTION
// *************************************************************************

  /**
    Removes the characters in a substring of this string.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String& remove(MemorySize start, MemorySize end);

  /**
    Removes the characters from the specified index to the end of the string.

    @param start Specifies the start of the string.
  */
  String& removeFrom(MemorySize start);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline String& removeAt(MemorySize index)
  {
    return remove(index, index);
  }

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline String& append(char ch) throw(StringException, MemoryException)
  {
    return insert(getLength(), ch);
  }

  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  inline String& append(const String& string) throw(StringException, MemoryException)
  {
    return insert(getLength(), string);
  }

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
  */
  inline String& append(const NativeString& string) throw(StringException, MemoryException)
  {
    return insert(getLength(), string);
  }

  /**
    Appends the native string to this string.

    @param string The native string to be appended.
  */
  inline String& append(const char* string) throw(StringException, MemoryException)
  {
    return append(NativeString(string));
  }

  /**
    Appends the string literal to this string.

    @param string The string to be appended.
  */
  String& append(const Literal& string) throw(StringException, MemoryException);

  template<MemorySize SIZE>
  inline String& append(const char (&literal)[SIZE])
    throw(StringException, MemoryException) {
    return append(Literal(literal));
  }
  
  /**
    Appends the string literal to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the string to be appended.
  */
  String& append(
    const Literal& string,
    MemorySize maximum) throw(StringException, MemoryException);

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the string to be appended.
  */
  String& append(
    const NativeString& string,
    MemorySize maximum) throw(StringException, MemoryException);

  /**
    Appens memory span. Does nothing is buffer is not proper.
  */
  String& append(const MemorySpan& buffer) throw(StringException, MemoryException);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline String& prepend(char ch) throw(StringException, MemoryException) {
    return insert(0, ch);
  }

  /**
    Prepends the string to this string.

    @param string The string to be prepended.
  */
  inline String& prepend(
    const String& string) throw(StringException, MemoryException) {
    return insert(0, string);
  }

  /**
    Inserts the memory span into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param span The characters to be inserted.
  */
  String& insert(MemorySize index, const MemorySpan& span) throw(StringException, MemoryException);

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param ch The character to be inserted.
  */
  String& insert(MemorySize index, char ch) throw(StringException, MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string to be inserted.
  */
  String& insert(
    MemorySize index,
    const String& string) throw(StringException, MemoryException);

  /**
    Inserts the string literal into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string literal to be inserted.
  */
  String& insert(
    MemorySize index,
    const Literal& string) throw(StringException, MemoryException);

  template<MemorySize SIZE>
  inline String& insert(MemorySize index, const char (&literal)[SIZE])
    throw(StringException, MemoryException) {
    return insert(index, Literal(literal));
  }
  
  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The NULL-terminated string to be inserted.
  */
  String& insert(
    MemorySize index,
    const NativeString& string) throw(StringException, MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param string The string to replace with.
  */
  String& replace(
    MemorySize start,
    MemorySize end,
    const String& string) throw(StringException, MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  MemorySize replaceAll(
    const String& fromStr,
    const String& toStr) throw(StringException, MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String substring(MemorySize start, MemorySize end) const;

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline String substring(MemorySize start) const
  {
    return substring(start, getLength());
  }

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline String& operator+=(const String& suffix)
  {
    return append(suffix);
  }

  /**
    Appends the literal to this string.

    @param suffix The character to be appended.
  */
  inline String& operator+=(const Literal& suffix)
  {
    return append(suffix);
  }

  /**
    Appends the string to this string.

    @param suffix The character to be appended.
  */
  inline String& operator+=(const char* suffix)
  {
    return append(NativeString(suffix));
  }

  /**
    Appends the character to this string.

    @param suffix The character to be appended.
  */
  inline String& operator+=(char suffix)
  {
    return append(suffix);
  }

  /**
    String reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  String& operator-=(const String& suffix);

// *************************************************************************
//   UNARY SECTION
// *************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse
    sequence.
  */
  String& reverse() noexcept;

  /**
    Converts the characters of this string into the lower case equivalents.
  */
  String& toLowerCase() noexcept;

  /**
    Converts the characters of this string into the upper case equivalents.
  */
  String& toUpperCase() noexcept;

// *************************************************************************
//   COMPARE SECTION
// *************************************************************************

  /**
    Compares this string to another string.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const String& string) const noexcept;
  
  /**
    Compares this string to with string literal.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const Literal& string) const noexcept;
  
  /**
    Compares this string to a NULL-terminated string.
    
    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string. False, if string is 0.
  */
  int compareTo(const NativeString& string) const noexcept;
  
  /**
    Compares this string with other string ignoring the case of the characters.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareToIgnoreCase(const String& string) const throw();

  /**
    Compares this string with NULL-terminated string ignoring the case of the
    characters.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareToIgnoreCase(const NativeString& string) const throw(StringException);
  
  template<MemorySize SIZE>
  inline int compareToIgnoreCase(const char (&literal)[SIZE]) const noexcept {
    return compareToIgnoreCase(NativeString(literal));
  }
  
  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const String& prefix) const noexcept;

  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const Literal& prefix) const noexcept;

  template<MemorySize SIZE>
  inline bool startsWith(const char (&literal)[SIZE]) const noexcept {
    return startsWith(Literal(literal));
  }
  
  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const String& suffix) const noexcept;

  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const Literal& suffix) const noexcept;

  template<MemorySize SIZE>
  inline bool endsWith(const char (&literal)[SIZE]) const noexcept {
    return endsWith(Literal(literal));
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const String& string) const noexcept {
    return compareTo(string) == 0;
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const Literal& string) const noexcept {
    return compareTo(string) == 0;
  }
  
  template<MemorySize SIZE>
  inline bool operator==(const char (&literal)[SIZE]) const noexcept {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) == 0;
  }
  
  /**
    Inequality operator.
  */
  inline bool operator!=(const String& string) const noexcept {
    return compareTo(string) != 0;
  }

  /**
    Equality operator.
  */
  inline bool operator!=(const Literal& string) const noexcept {
    return compareTo(string) != 0;
  }
  
  template<MemorySize SIZE>
  inline bool operator!=(const char (&literal)[SIZE]) const noexcept {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) != 0;
  }
  
  /**
    Less than operator.
  */
  inline bool operator<(const String& string) const noexcept {
    return compareTo(string) < 0;
  }

  template<MemorySize SIZE>
  inline bool operator<(const char (&literal)[SIZE]) const noexcept {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) < 0;
  }
  
  /**
    Less than or equal operator.
  */
  inline bool operator<=(const String& string) const noexcept {
    return compareTo(string) <= 0;
  }

  template<MemorySize SIZE>
  inline bool operator<=(const char (&literal)[SIZE]) const noexcept {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) <= 0;
  }
  
  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const String& string) const noexcept {
    return compareTo(string) >= 0;
  }

  template<MemorySize SIZE>
  inline bool operator>=(const char (&literal)[SIZE]) const noexcept {
    return compareTo(Literal(literal)) >= 0;
  }
  
  /**
    Greater than operator.
  */
  inline bool operator>(const String& string) const noexcept {
    return compareTo(string) > 0;
  }

  template<MemorySize SIZE>
  inline bool operator>(const char (&literal)[SIZE]) const noexcept {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) > 0;
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const NativeString& string) const noexcept {
    return compareTo(string) == 0;
  }

  /**
    Inequality operator.
  */
  inline bool operator!=(const NativeString& string) const noexcept {
    return compareTo(string) != 0;
  }

  /**
    Less than operator.
  */
  inline bool operator<(const NativeString& string) const noexcept {
    return compareTo(string) < 0;
  }

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const NativeString& string) const noexcept {
    return compareTo(string) <= 0;
  }

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const NativeString& string) const noexcept {
    return compareTo(string) >= 0;
  }

  /**
    Greater than operator.
  */
  inline bool operator>(const NativeString& string) const noexcept {
    return compareTo(string) > 0;
  }

// *************************************************************************
//   FIND SECTION
// *************************************************************************

  /**
    Returns the index of the first character that matches the specified
    character after the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1.
  */
  MemoryDiff indexOf(char ch, MemorySize start = 0) const noexcept;

  /**
    Returns the index of the first substring that matches the specified string
    after the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff indexOf(const String& string, MemorySize start = 0) const noexcept;
  
  /**
    Returns the index of the last character that matches the specified
    character before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of
    string.
    @return Index of the last match if any otherwise -1.
  */
  MemoryDiff lastIndexOf(char ch, MemorySize start) const noexcept;
  
  inline MemoryDiff lastIndexOf(char ch) const noexcept
  {
    return lastIndexOf(ch, getLength());
  }

  /**
    Returns the index of the last substring that matches the specified string
    before the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is end of
    string.
    @return Index of the last match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff lastIndexOf(const String& string, MemorySize start) const noexcept;
  
  inline MemoryDiff lastIndexOf(const String& string) const noexcept
  {
    return lastIndexOf(string, getLength());
  }

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  MemorySize count(char ch, MemorySize start = 0) const noexcept;

  /**
    Counts the number of occurances of the specified substring in this string.

    @param string The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  MemorySize count(const String& string, MemorySize start = 0) const noexcept;

  /**
    Trims the string.

    @param character The character to remove.
  */
  String& trim(char character = ' ') throw();

  /**
    Returns the index of the first substring that matches the specified string
    after the start position. The implementation is based on the "Boyer-Moore
    Fast String Searching Algorithm".
    
    @see indexOf
    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  MemoryDiff search(const String& substring, MemorySize start = 0) const throw();
  
  /**
    Returns the substrings between the specified separator.

    @param separator Separator.
    @param group Group separators. Default is false.
  */
  Array<String> split(char separator, bool group = false) const;
  
// *************************************************************************
//   END SECTION
// *************************************************************************

  /**
    Returns NULL-terminated string for modifying access.
  */
  char* getElements() noexcept;

  /** Returns true if state is valid. */
  inline bool invariant() const noexcept
  {
    const MemorySize length = getLength();
    const char* buffer = elements->getElements();
    return (buffer[length] == Traits::TERMINATOR);
  }
  
  /**
    Returns NULL-terminated string.
  */
  inline const char* getElements() const noexcept
  {
    const MemorySize length = getLength();
    const char* result = elements->getElements();
    ASSERT(result[length] == Traits::TERMINATOR);
    if (result[length] != Traits::TERMINATOR) { // TAG: remove when ready
      const MemorySize likelyLength = getNullTerminatedLength(result);
      const MemorySize lengthAgain = getLength();
      const_cast<char*>(result)[length] = Traits::TERMINATOR;
    }
    return result;
  }

  /**
    Returns the end of string.
  */
  inline const char* getEnd() const noexcept
  {
    const MemorySize length = getLength();
    const char* result = elements->getElements();
    ASSERT(result[length] == Traits::TERMINATOR);
    return result + length;
  }

  // TAG: we can return both being and end with one method MemorySpan<char> getMemorySpan() const;
  
  // TAG: remove getBytes()
  /**
    Returns the characters of the string for non-modifying access.
  */
  inline const char* getBytes() const noexcept
  {
    const char* result = elements->getElements();
    ASSERT(result[getLength()] == Traits::TERMINATOR);
    return result;
  }

  /**
    Returns the characters of the string for non-modifying access.
  */
  inline const char* native() const noexcept
  {
    const char* result = elements->getElements();
    ASSERT(result[getLength()] == Traits::TERMINATOR);
    return result;
  }
};

template<>
int compare<String>(const String& left, const String& right);

template<>
class _COM_AZURE_DEV__BASE__API Hash<String> {
public:

  unsigned long operator()(const String& value) throw();
};

/**
  Writes string to format stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline String operator+(const String& left, const String& right)
{
  return String(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline String operator+(const String& left, const char* right)
{
  return left + String(right);
}

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline String operator+(const char* left, const String& right)
{
  return String(left) + right;
}

/**
  String reduction. Removes suffix from string if and only if it ends with the
  suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas
  ("pre"-"suf") results in "pre").
*/
inline String operator-(const String& left, const String& right)
{
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength()); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
}

template<>
inline void swapper<String>(String& a, String& b)
{
  swapper(a.elements, b.elements); // self swap allowed
}

/** Converts String to UTF-8 string. */
std::string toUTF8(const String& s);

/** Converts wstring to UTF-8 string. */
std::string toUTF8(const std::wstring& s);

/** Converts const wchar* to UTF-8 string. */
std::string toUTF8(const wchar* s);

/** Converts const wchar* to UTF-8 string. */
std::string toUTF8(const wchar* s, MemorySize length);

/** Converts UTF-8 string to wstring. */
std::wstring toWide(const std::string& s);

/** Converts String to wstring. */
std::wstring toWide(const String& s);

/** Converts UTF-8 (const char*) to wstring. */
std::wstring toWide(const char* s);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
