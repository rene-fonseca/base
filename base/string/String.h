/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/Primitives.h>
#include <base/collection/Hash.h>
#include <base/Literal.h>
#include <string>
#include <ctype.h> // TAG: alien header

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

template<class TYPE> class Array;
class FormatOutputStream;

/**
  Default character manipulators.

  @short ASCII character characteristics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
class CharTraits {
public:
  
  /** Specifies the terminator for NULL-terminated strings. */
  static const char TERMINATOR = '\0';

  /** Returns true if the character an alphabetic character. */
  static inline bool isAlpha(char character) throw() {
    return isalpha(character);
  }
  
  /** Returns true if the character an alphabetic character or a digit. */
  static inline bool isAlphaNum(char character) throw() {
    return isalnum(character);
  }

  /** Returns true if the character is lowercase. */
  static inline bool isLower(char character) throw() {
    return islower(character);
  }
  
  /** Returns true if the character is uppercase. */
  static inline bool isUpper(char character) throw() {
    return isupper(character);
  }
  
  /** Returns true if the character is a digit. */
  static inline bool isDigit(char character) throw() {
    return isdigit(character);
  }
  
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(char character) throw() {
    return isxdigit(character);
  }
  
  /** Returns true if the character is a white space. */
  static inline bool isSpace(char character) throw() {
    return isspace(character);
  }
  
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(char character) throw() {
    return ispunct(character);
  }
  
  /** Returns true if the character is printable. */
  static inline bool isPrintable(char character) throw() {
    return isprint(character);
  }
  
  /** Returns true if the character is a visible character. */
  static inline bool isGraph(char character) throw() {
    return isgraph(character);
  }
  
  /** Returns true if the character is a control character. */
  static inline bool isControl(char character) throw() {
    return iscntrl(character);
  }
  
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(char character) throw() {
    return  !(character & ~0177U);
  }

  /** Converts the character to lowercase. */
  static inline char toLower(char character) throw() {
    return tolower(character);
  }
  
  /** Converts the character to uppercase. */
  static inline char toUpper(char character) throw() {
    return toupper(character);
  }

  class ToLowerCase {
  public:
    
    inline char operator()(char value) const throw() {
      return tolower(value);
    }
  };

  class ToUpperCase {
  public:
    
    inline char operator()(char value) const throw() {
      return toupper(value);
    }
  };
};

/**
  String class. The first modifing operation on a string may force the internal
  buffer to be duplicated. The implementation is currently NOT MT-safe.

  @short String
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class String : public virtual Object {
public:

  /** Character specific properties and manipulators. */
  typedef CharTraits Traits;
  
  /** Specifies the string terminator. */
  static const char TERMINATOR = '\0';
  /**
    Specifies the granularity of the capacity. Guaranteed to be greater than 0.
  */
  static const unsigned int GRANULARITY = 16;
  /**
    Specifies the maximum length of any string. Guarantees that an int can hold
    the length of the string.
  */
  static const unsigned int MAXIMUM_LENGTH =
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
  class Element {
    friend class String;
  private:
    
    String& string;
    unsigned int index = 0;
    
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();
    
    inline Element(String& _string, unsigned int _index) throw()
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
  void initialize(const char* string, unsigned int length) throw(MemoryException);
  
  /**
    Returns a modifiable buffer. Forces copy of internal buffer if shared by
    multiple strings.
  */
  inline char* getBuffer() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }
  
  /**
    Returns a non-modifiable buffer.
  */
  inline const char* getBuffer() const throw() {
    return elements->getElements();
  }
  
  /**
    Sets the length of the string.
  */
  inline void setLength(
    unsigned int length) throw(StringException, MemoryException) {
    bassert(
      length <= MAXIMUM_LENGTH,
      StringException(Type::getType<String>())
    );
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(length + 1);
  }

  /**
    Compare two NULL-terminated strings.
  */
  static int compareToIgnoreCase(const char* left, const char* right) throw();
public:

  /**
    Returns the length of the NULL-terminated string.

    @param string The NULL-terminated string.
    @param maximum The maximum length of the string. The default is
    MAXIMUM_LENGTH.
  */
  static inline unsigned int getLengthOfMustBeTerminated(
    const char* string,
    unsigned int maximum = MAXIMUM_LENGTH) throw(StringException) {
    bassert(string, StringException(Type::getType<String>()));
    const char* terminator = find(string, maximum, Traits::TERMINATOR);
    bassert(terminator, StringException(Type::getType<String>()));
    return static_cast<unsigned int>(terminator - string);
  }

  /**
    Returns the length of the NULL-terminated string.

    @param string The NULL-terminated string.
    @param maximum The maximum length of the string. The default is
    MAXIMUM_LENGTH.

    @return maximum if terminator is not found. 0 if string is invalid
    (i.e. 0).
  */
  static inline unsigned int getLengthOfTerminated(
    const char* string, unsigned int maximum = MAXIMUM_LENGTH) throw() {
    if (!string) {
      return 0;
    }
    const char* terminator = find(string, maximum, Traits::TERMINATOR);
    return terminator ? static_cast<unsigned int>(terminator - string) : maximum;
  }
  
  /**
    Initializes an empty string.
  */
  String() throw();

  /**
    Initializes a string with no characters in it, initial capacity and
    granularity of capacity.
    
    @param capacity The initial capacity.
  */
  explicit String(unsigned int capacity) throw(MemoryException);

  /**
    Initializes the string from a string literal. The string literal is not
    copied into internal buffer. Implicit initialization is allowed.

    @param string String literal.
  */
  String(const Literal& string) throw(StringException, MemoryException);

  String(const std::string& string) throw(StringException, MemoryException);

  String(const std::wstring& string) throw(StringException, MemoryException);

  /**
    Initializes the string from a string literal. Implicit initialization is
    allowed.
    
    @param literal String literal.
  */
  template<MemorySize SIZE>
  inline String(const char (&literal)[SIZE]) throw(MemoryException) {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    initialize(literal, SIZE - 1);
  }
  
  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
  */
  String(const NativeString& string) throw(MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized
    with no characters in it.
  */
  String(const char* string) throw(MemoryException);

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
    unsigned int maximum) throw(StringException, MemoryException);
  
  /**
    Initializes string from other string.
  */
  inline String(const String& copy) throw() : elements(copy.elements) {
  }
  
  /**
    Assignment of string to string.
  */
  inline String& operator=(const String& eq) throw() {
    elements = eq.elements; // self assignment handled by automation pointer
    return *this;
  }
  
  /**
    Assignment of string literal to string.
  */
  String& operator=(const Literal& eq) throw(StringException, MemoryException);

  template<MemorySize SIZE>
  inline String& operator=(const char (&literal)[SIZE])
    throw(MemoryException) {
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
  inline unsigned int getLength() const throw() {
    return elements->getSize() - 1;
  }

  /**
    Returns true if the string does not contain characters.
  */
  inline bool isEmpty() const throw() {
    return elements->getSize() == 1;
  }

  /**
    Returns true if the string contains characters.
  */
  inline bool isProper() const throw() {
    return elements->getSize() > 1;
  }

  /**
    Returns true if the string is an ASCII string.
  */
  bool isASCII() const throw();
  
  /**
    Returns the capacity of the string.
  */
  inline unsigned int getCapacity() const throw() {
    return elements->getCapacity();
  }

  /**
    Ensures that the capacity of the buffer is at least equal to the specified
    minimum. This applies to all shared strings.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(unsigned int capacity) throw(MemoryException);

  /**
    Releases any unused capacity of the string. This applies to all shared
    strings.
  */
  void optimizeCapacity() throw();

  /**
    Returns the granularity.
  */
  unsigned int getGranularity() const throw();

  /**
    Sets the granularity.
  */
  void setGranularity(unsigned int granularity) throw();

  /**
    Sets the length of the string without initializing the elements.
  */
  void forceToLength(
    unsigned int length) throw(StringException, MemoryException);

// *************************************************************************
//   TRAVERSE SECTION
// *************************************************************************

  /**
    Returns the first element of the string as a modifying iterator.
  */
  inline Iterator getBeginIterator() throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the string as a modifying iterator.
  */
  inline Iterator getEndIterator() throw() {
    return elements->getEndIterator() - 1; // remember terminator
  }

  /**
    Returns the first element of the string as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const throw() {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the string as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const throw() {
    return elements->getEndReadIterator() - 1; // remember terminator
  }

  /**
    Returns a modifying enumerator of the string.
  */
  inline Enumerator getEnumerator() throw() {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the string.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements->getReadEnumerator();
  }

// *************************************************************************
//   CHARACTER SECTION
// *************************************************************************

  /**
    Returns the character at the specified index in this string. Raises
    OutOfRange if index exceeds the length of the string.
  */
  char getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\\0') then the string is cut off from
    the specified index. Raises OutOfRange if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(unsigned int index, char value) throw(OutOfRange);

  /**
    Returns a reference to character at the specified index. Raises
    OutOfRange if index exceeds the length of the string.
  */
  Element operator[](unsigned int index) throw(OutOfRange) {
    return Element(*this, index);
  }

  /**
    Returns the character at the specified index. Raises OutOfRange if index
    exceeds the length of the string.
  */
  inline char operator[](unsigned int index) const throw(OutOfRange) {
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
  String& remove(unsigned int start, unsigned int end) throw(MemoryException);

  /**
    Removes the characters from the specified index to the end of the string.

    @param start Specifies the start of the string.
  */
  String& removeFrom(unsigned int start) throw(MemoryException);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline String& removeAt(unsigned int index) throw(MemoryException) {
    return remove(index, index);
  }

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline String& append(char ch) throw(StringException, MemoryException) {
    return insert(getLength(), ch);
  }

  /**
    Appends the string to this string.

    @param string The string to be appended.
  */
  inline String& append(
    const String& string) throw(StringException, MemoryException) {
    return insert(getLength(), string);
  }

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
  */
  inline String& append(
    const NativeString& string) throw(StringException, MemoryException) {
    return insert(getLength(), string);
  }
  
  /**
    Appends the string literal to this string.

    @param string The string to be appended.
  */
  String& append(
    const Literal& string) throw(StringException, MemoryException);

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
    unsigned int maximum) throw(StringException, MemoryException);

  /**
    Appends the NULL-terminated string to this string.

    @param string The string to be appended.
    @param maximum The maximum length of the string to be appended.
  */
  String& append(
    const NativeString& string,
    unsigned int maximum) throw(StringException, MemoryException);

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
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param ch The character to be inserted.
  */
  String& insert(
    unsigned int index, char ch) throw(StringException, MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string to be inserted.
  */
  String& insert(
    unsigned int index,
    const String& string) throw(StringException, MemoryException);

  /**
    Inserts the string literal into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param string The string literal to be inserted.
  */
  String& insert(
    unsigned int index,
    const Literal& string) throw(StringException, MemoryException);

  template<MemorySize SIZE>
  inline String& insert(unsigned int index, const char (&literal)[SIZE])
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
    unsigned int index,
    const NativeString& string) throw(StringException, MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param string The string to replace with.
  */
  String& replace(
    unsigned int start,
    unsigned int end,
    const String& string) throw(StringException, MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(
    const String& fromStr,
    const String& toStr) throw(StringException, MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String substring(
    unsigned int start, unsigned int end) const throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline String substring(unsigned int start) const throw(MemoryException) {
    return substring(start, getLength());
  }

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline String& operator+=(const String& suffix) throw(MemoryException) {
    return append(suffix);
  }

  /**
    Appends the character to this string.

    @param suffix The character to be appended.
  */
  inline String& operator+=(char suffix) throw(MemoryException) {
    return append(suffix);
  }

  /**
    String reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  String& operator-=(const String& suffix) throw(MemoryException);

// *************************************************************************
//   UNARY SECTION
// *************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse
    sequence.
  */
  String& reverse() throw();

  /**
    Converts the characters of this string into the lower case equivalents.
  */
  String& toLowerCase() throw();

  /**
    Converts the characters of this string into the upper case equivalents.
  */
  String& toUpperCase() throw();

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
  int compareTo(const String& string) const throw();
  
  /**
    Compares this string to with string literal.

    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string.
  */
  int compareTo(const Literal& string) const throw();
  
  /**
    Compares this string to a NULL-terminated string.
    
    @param string The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is
    found, respectively, to be less than, equal to, or greater than the
    specified string. False, if string is 0.
  */
  int compareTo(const NativeString& string) const throw();
  
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
  int compareToIgnoreCase(
    const NativeString& string) const throw(StringException);
  
  template<MemorySize SIZE>
  inline int compareToIgnoreCase(const char (&literal)[SIZE]) const throw() {
    return compareToIgnoreCase(NativeString(literal));
  }
  
  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const String& prefix) const throw();

  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const Literal& prefix) const throw();

  template<MemorySize SIZE>
  inline bool startsWith(const char (&literal)[SIZE]) const throw() {
    return startsWith(Literal(literal));
  }
  
  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const String& suffix) const throw();

  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const Literal& suffix) const throw();

  template<MemorySize SIZE>
  inline bool endsWith(const char (&literal)[SIZE]) const throw() {
    return endsWith(Literal(literal));
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const String& string) const throw() {
    return compareTo(string) == 0;
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const Literal& string) const throw() {
    return compareTo(string) == 0;
  }
  
  template<MemorySize SIZE>
  inline bool operator==(const char (&literal)[SIZE]) const throw() {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) == 0;
  }
  
  /**
    Inequality operator.
  */
  inline bool operator!=(const String& string) const throw() {
    return compareTo(string) != 0;
  }

  /**
    Equality operator.
  */
  inline bool operator!=(const Literal& string) const throw() {
    return compareTo(string) != 0;
  }
  
  template<MemorySize SIZE>
  inline bool operator!=(const char (&literal)[SIZE]) const throw() {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) != 0;
  }
  
  /**
    Less than operator.
  */
  inline bool operator<(const String& string) const throw() {
    return compareTo(string) < 0;
  }

  template<MemorySize SIZE>
  inline bool operator<(const char (&literal)[SIZE]) const throw() {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) < 0;
  }
  
  /**
    Less than or equal operator.
  */
  inline bool operator<=(const String& string) const throw() {
    return compareTo(string) <= 0;
  }

  template<MemorySize SIZE>
  inline bool operator<=(const char (&literal)[SIZE]) const throw() {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) <= 0;
  }
  
  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const String& string) const throw() {
    return compareTo(string) >= 0;
  }

  template<MemorySize SIZE>
  inline bool operator>=(const char (&literal)[SIZE]) const throw() {
    return compareTo(Literal(literal)) >= 0;
  }
  
  /**
    Greater than operator.
  */
  inline bool operator>(const String& string) const throw() {
    return compareTo(string) > 0;
  }

  template<MemorySize SIZE>
  inline bool operator>(const char (&literal)[SIZE]) const throw() {
    if (Constraint<(SIZE > 0)>::UNSPECIFIED) {}
    return compareTo(Literal(literal)) > 0;
  }
  
  /**
    Equality operator.
  */
  inline bool operator==(const NativeString& string) const throw() {
    return compareTo(string) == 0;
  }

  /**
    Inequality operator.
  */
  inline bool operator!=(const NativeString& string) const throw() {
    return compareTo(string) != 0;
  }

  /**
    Less than operator.
  */
  inline bool operator<(const NativeString& string) const throw() {
    return compareTo(string) < 0;
  }

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const NativeString& string) const throw() {
    return compareTo(string) <= 0;
  }

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const NativeString& string) const throw() {
    return compareTo(string) >= 0;
  }

  /**
    Greater than operator.
  */
  inline bool operator>(const NativeString& string) const throw() {
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
  int indexOf(char ch, unsigned int start = 0) const throw();

  /**
    Returns the index of the first substring that matches the specified string
    after the start position.

    @param string The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if
    substring is empty.
  */
  int indexOf(const String& string, unsigned int start = 0) const throw();
  
  /**
    Returns the index of the last character that matches the specified
    character before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of
    string.
    @return Index of the last match if any otherwise -1.
  */
  int lastIndexOf(char ch, unsigned int start) const throw();
  
  inline int lastIndexOf(char ch) const throw() {
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
  int lastIndexOf(const String& string, unsigned int start) const throw();
  
  inline int lastIndexOf(const String& string) const throw() {
    return lastIndexOf(string, getLength());
  }

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  unsigned int count(char ch, unsigned int start = 0) const throw();

  /**
    Counts the number of occurances of the specified substring in this string.

    @param string The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  unsigned int count(
    const String& string, unsigned int start = 0) const throw();

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
  int search(const String& substring, unsigned int start = 0) const throw();
  
  /**
    Returns the substrings between the specified separator.

    @param separator Separator.
    @param group Group separators. Default is false.
  */
  Array<String> split(
    char separator, bool group = false) throw(MemoryException);
  
// *************************************************************************
//   END SECTION
// *************************************************************************

  /**
    Returns NULL-terminated string for modifying access.
  */
  char* getElements() throw();

  /**
    Returns NULL-terminated string.
  */
  inline const char* getElements() const throw() {
    // special case: no need to copy on write 'cause we only add terminator
    char* result = const_cast<char*>(elements->getElements()); // TAG: fix cast
    result[getLength()] = Traits::TERMINATOR;
    return result;
  }

  /**
    Returns the characters of the string for non-modifying access. The elements
    may not be NULL-terminated. Avoid this method if you can.
  */
  inline const char* getBytes() const throw() {
    return elements->getElements();
  }

  /**
    Returns the characters of the string for non-modifying access. The elements
    may not be NULL-terminated. Avoid this method if you can.
  */
  inline const char* native() const throw() {
    // special case: no need to copy on write 'cause we only add terminator
    char* result = const_cast<char*>(elements->getElements()); // TAG: fix cast
    result[getLength()] = Traits::TERMINATOR;
    return result;
  }
};

template<>
int compare<String>(const String& left, const String& right) throw();

template<>
class Hash<String> {
public:

  unsigned long operator()(const String& value) throw();
};

/**
  Writes string to format stream.
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const String& value) throw(IOException);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline String operator+(
  const String& left,
  const String& right) throw(StringException, MemoryException) {
  return
    String(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  String reduction. Removes suffix from string if and only if it ends with the
  suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas
  ("pre"-"suf") results in "pre").
*/
inline String operator-(
  const String& left, const String& right) throw(MemoryException) {
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength()); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
