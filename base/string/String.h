/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_H
#define _DK_SDU_MIP__BASE_STRING__STRING_H

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/OutOfDomain.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/Type.h>
#include <ctype.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class binds together a string literal and its length. Use the macro
  MESSAGE to generate an object of this class for a given string literal (e.g.
  MESSAGE("Hello World")). Do not call the constructor manually.

  @short String literal
  @author Ren� M�ller Fonseca
  @version 1.1
*/
class StringLiteral {
private:

  /** The number of characters occupied by the message without the terminator. */
  const unsigned int length;
  /** NULL-terminated message. */
  const char* message;
public:

  /** Initializes message. Automatically invocated by the macro MESSAGE. */
  inline StringLiteral(unsigned int l, const char* m) throw() : length(l), message(m) {}
  /** Cast to the usual message type. */
  inline operator const char*() const throw() {return message;}
  /** Returns the length of the string literal. */
  inline unsigned int getLength() const throw() {return length;}
};

/** This macro generates a StringLiteral object from a string literal. */
#define MESSAGE(msg) StringLiteral(sizeof(msg) - 1, msg)



/**
  Default character manipulators.

  @author Ren� M�ller Fonseca
  @version 1.0
*/
class CharTraits {
public:

  /** The type of a single character. */
  typedef char Character;
  /** Specifies the terminator for null-terminated strings. */
  static const char TERMINATOR = '\0';

  /** Returns true if the character an alphabetic character. */
  static inline bool isAlpha(Character character) {return isalpha(character);}
  /** Returns true if the character an alphabetic character or a digit. */
  static inline bool isAlphaNum(Character character) {return isalnum(character);}
  /** Returns true if the character is lowercase. */
  static inline bool isLower(Character character) {return islower(character);}
  /** Returns true if the character is uppercase. */
  static inline bool isUpper(Character character) {return isupper(character);}
  /** Returns true if the character is a digit. */
  static inline bool isDigit(Character character) {return isdigit(character);}
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(Character character) {return isxdigit(character);}
  /** Returns true if the character is a white space. */
  static inline bool isSpace(Character character) {return isspace(character);}
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(Character character) {return ispunct(character);}
  /** Returns true if the character is printable. */
  static inline bool isPrintable(Character character) {return isprint(character);}
  /** Returns true if the character is a visible character. */
  static inline bool isGraph(Character character) {return isgraph(character);}
  /** Returns true if the character is a control character. */
  static inline bool isControl(Character character) {return iscntrl(character);}
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(Character character) {return  !(character & ~0177U);}

  /** Converts the character to lowercase. */
  static inline Character toLower(Character character) throw() {return tolower(character);}
  /** Converts the character to uppercase. */
  static inline Character toUpper(Character character) throw() {return toupper(character);}

  class ToLowerCase {
  public:
    inline Character operator()(Character value) const throw() {return tolower(value);}
  };

  class ToUpperCase {
  public:
    inline Character operator()(Character value) const throw() {return toupper(value);}
  };
};

/**
  String class. The first modifing operation on a string may force the internal
  buffer to be duplicated. The implementation is currently NOT MT-safe.

  @short String.
  @author Ren� M�ller Fonseca
  @version 1.0
*/

class String : public virtual Object {
public:

  typedef CharTraits Traits;
  /** The type of an element (character). */
  typedef CharTraits::Character Character;

  /** Specifies the string terminator. */
  static const char TERMINATOR = '\0';
  /** Specifies the granularity of the capacity. Guaranteed to be greater than 0. */
  static const unsigned int GRANULARITY = 16;
  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. */
  static const unsigned int MAXIMUM_LENGTH = ((Int::MAXIMUM - sizeof(TERMINATOR))/GRANULARITY)*GRANULARITY;

  typedef ReferenceCountedCapacityAllocator<char>::Iterator Iterator;
  typedef ReferenceCountedCapacityAllocator<char>::ReadIterator ReadIterator;
  typedef ReferenceCountedCapacityAllocator<char>::Enumerator Enumerator;
  typedef ReferenceCountedCapacityAllocator<char>::ReadEnumerator ReadEnumerator;
private:

  /** The default string. This is used to avoid multiple allocations of empty string buffers. */
  static const String DEFAULT_STRING;

  /**
    Reference to an element within a string.
  */
  class Reference {
  private:
    friend class String;
    String& string;
    unsigned int index;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    inline Reference(String& s, unsigned int i) : string(s), index(i) {}
  public:
    inline Reference& operator=(char value) throw(OutOfRange) {string.setAt(index, value); return *this;}
    inline operator char() const throw(OutOfRange) {return string.getAt(index);}
  };

  /**
    Reference counted buffer holding NULL-terminated string. The array is
    guarantied to be non-empty when the string has been initialized.
  */
  ReferenceCountedObjectPointer<ReferenceCountedCapacityAllocator<char> > elements;
protected:

  /**
    Returns a modifiable buffer. Forces copy of internal buffer if shared by multiple strings.
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
    Returns the length of a native NULL-terminated string (-1 if not terminated).
  */
  int getLengthOfString(const char* str) const throw();

  /**
    Sets the length of the string.
  */
  inline void setLength(unsigned int length) throw(StringException, MemoryException) {
    assert(length <= MAXIMUM_LENGTH, StringException());
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(length + 1);
  }

  /**
    Compare two null-terminated strings.
  */
  static int compareToIgnoreCase(const char* left, const char* right) throw();
public:

  /**
    Initializes an empty string.
  */
  String() throw();

  /**
    Initializes a string with no characters in it, initial capacity and granularity of capacity.

    @param capacity The initial capacity.
  */
  explicit String(unsigned int capacity) throw(MemoryException);

  /**
    Initializes the string from a string literal. The string literal is not copied into internal buffer. Implicit initialization is allowed.

    @param str String literal generated by the macro MESSAGE (e.g. MESSAGE("My string"))
  */
  String(const StringLiteral& str) throw(StringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param str NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
  */
  String(const char* str) throw(StringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string (str) exceeds the maximum length (n) only the first n
    characters are used.

    @param str NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
    @param maximum Specifies the maximum length.
  */
  String(const char* str, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException);

  /**
    Initializes string from other string.
  */
  inline String(const String& copy) throw() : elements(copy.elements) {}

  /**
    Assignment of string with string.
  */
  inline String& operator=(const String& eq) throw() {
    elements = eq.elements; // self assignment handled by automation pointer
    return *this;
  }

  /**
    Assignment of string with NULL-terminated string.
  */
  String& operator=(const char* str) throw(StringException, MemoryException);

  /**
    Returns the number of characters in the string.
  */
  inline unsigned int getLength() const throw() {return elements->getSize() - 1;}

  /**
    Returns true if the string does not contain characters.
  */
  inline bool isEmpty() const throw() {return elements->getSize() == 1;}

  /**
    Returns true if the string contains characters.
  */
  inline bool isProper() const throw() {return elements->getSize() > 1;}

  /**
    Returns the capacity of the string.
  */
  inline unsigned int getCapacity() const throw() {return elements->getCapacity();}

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

// *******************************************************************************************
//   TRAVERSE SECTION
// *******************************************************************************************

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

// *******************************************************************************************
//   CHARACTER SECTION
// *******************************************************************************************

  /**
    Returns the character at the specified index in this string. Throws
    'OutOfRange' if index exceeds the length of the string.
  */
  char getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\0') then the string is cut off from
    the specified index. Throws 'OutOfRange' if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(unsigned int index, char value) throw(OutOfRange);

  /**
    Returns a reference to character at the specified index. Throws
    'OutOfRange' if index exceeds the length of the string.
  */
  Reference operator[](unsigned int index) throw(OutOfRange) {
    return Reference(*this, index);
  }

  /**
    Returns the character at the specified index. Throws 'OutOfRange' if index
    exceeds the length of the string.
  */
  inline char operator[](unsigned int index) const throw(OutOfRange) {
    return getAt(index);
  }

// *******************************************************************************************
//   SUBSTRING SECTION
// *******************************************************************************************

  /**
    Removes the characters in a substring of this string.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String& remove(unsigned int start, unsigned int end) throw(MemoryException);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline String& removeCharacter(unsigned int index) throw(MemoryException) {return remove(index, index);}

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline String& append(char ch) throw(StringException, MemoryException) {return insert(getLength(), ch);}

  /**
    Appends the string to this string.

    @param str The string to be appended.
  */
  inline String& append(const String& str) throw(StringException, MemoryException) {return insert(getLength(), str);}

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
  */
  inline String& append(const char* str) throw(StringException, MemoryException) {return insert(getLength(), str);}

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  String& append(const StringLiteral& str, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException);

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  String& append(const char* str, unsigned int maximum) throw(OutOfDomain, StringException, MemoryException);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline String& prepend(char ch) throw(StringException, MemoryException) {return insert(0, ch);}

  /**
    Prepends the string to this string.

    @param str The string to be prepended.
  */
  inline String& prepend(const String& str) throw(StringException, MemoryException) {return insert(0, str);}

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.
    @param ch The character to be inserted.
  */
  String& insert(unsigned int index, char ch) throw(StringException, MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param str The string to be inserted.
  */
  String& insert(unsigned int index, const String& str) throw(StringException, MemoryException);

  /**
    Inserts the string literal into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param str The string literal to be inserted.
  */
  String& insert(unsigned int index, const StringLiteral& str) throw(StringException, MemoryException);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.
    @param str The NULL-terminated string to be inserted.
  */
  String& insert(unsigned int index, const char* str) throw(StringException, MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param str The string to replace with.
  */
  String& replace(unsigned int start, unsigned int end, const String& str) throw(StringException, MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(const String& fromStr, const String& toStr) throw(StringException, MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String substring(unsigned int start, unsigned int end) const throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline String substring(unsigned int start) const throw(MemoryException) {return substring(start, getLength());}

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline String& operator+=(const String& suffix) throw(MemoryException) {return append(suffix);}

  /**
    Appends the character to this string.

    @param suffix The character to be appended.
  */
  inline String& operator+=(Character suffix) throw(MemoryException) {return append(suffix);}

  /**
    String reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  String& operator-=(const String& suffix) throw(MemoryException);

  /**
    Returns a NULL terminated string that contains a subsequence of characters
    currently contained in this string. If 'end' is less than 'start' an empty
    string is returned. Does nothing if buffer is NULL.

    @param buffer The buffer to receive the NULL terminated string.
    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.

    @return The specified buffer.
  */
  //char* substring(char* buffer, unsigned int start, unsigned int end) const throw();

// *******************************************************************************************
//   UNARY SECTION
// *******************************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse sequence.
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

// *******************************************************************************************
//   COMPARE SECTION
// *******************************************************************************************

  /**
    Compare this string with another string.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareTo(const String& str) const throw();

  /**
    Compare this string with null-terminated string.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareTo(const char* str) const throw(StringException);

  /**
    Compares this string with other string ignoring the case of the characters.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareToIgnoreCase(const String& str) const throw();

  /**
    Compares this string with null-terminated string ignoring the case of the characters.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareToIgnoreCase(const char* str) const throw(StringException);

  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const String& prefix) const throw();

  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const StringLiteral& prefix) const throw();

  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const String& suffix) const throw();

  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const StringLiteral& suffix) const throw();

  /**
    Equality operator.
  */
  inline bool operator==(const String& str) const throw() {return compareTo(str) == 0;}

  /**
    Inequality operator.
  */
  inline bool operator!=(const String& str) const throw() {return compareTo(str) != 0;}

  /**
    Less than operator.
  */
  inline bool operator<(const String& str) const throw() {return compareTo(str) < 0;}

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const String& str) const throw() {return compareTo(str) <= 0;}

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const String& str) const throw() {return compareTo(str) >= 0;}

  /**
    Greater than operator.
  */
  inline bool operator>(const String& str) const throw() {return compareTo(str) > 0;}

  /**
    Equality operator.
  */
  inline bool operator==(const char* str) const throw() {return compareTo(str) == 0;}

  /**
    Inequality operator.
  */
  inline bool operator!=(const char* str) const throw() {return compareTo(str) != 0;}

  /**
    Less than operator.
  */
  inline bool operator<(const char* str) const throw() {return compareTo(str) < 0;}

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const char* str) const throw() {return compareTo(str) <= 0;}

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const char* str) const throw() {return compareTo(str) >= 0;}

  /**
    Greater than operator.
  */
  inline bool operator>(const char* str) const throw() {return compareTo(str) > 0;}

// *******************************************************************************************
//   FIND SECTION
// *******************************************************************************************

  /**
    Returns the index of the first character that matches the specified character after the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1.
  */
  int indexOf(char ch, unsigned int start = 0) const throw();

  /**
    Returns the index of the first substring that matches the specified string after the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int indexOf(const String& str, unsigned int start = 0) const throw();

  /**
    Returns the index of the last character that matches the specified character before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1.
  */
  int lastIndexOf(char ch, unsigned int start) const throw();
  inline int lastIndexOf(char ch) const throw() {return lastIndexOf(ch, getLength());}

  /**
    Returns the index of the last substring that matches the specified string before the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int lastIndexOf(const String& str, unsigned int start) const throw();
  inline int lastIndexOf(const String& str) const throw() {return lastIndexOf(str, getLength());}

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  unsigned int count(char ch, unsigned int start = 0) const throw();

  /**
    Counts the number of occurances of the specified substring in this string.

    @param str The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  unsigned int count(const String& str, unsigned int start = 0) const throw();

// *******************************************************************************************
//   END SECTION
// *******************************************************************************************

  /**
    Returns null-terminated string.
  */
  inline const Character* getElements() const throw() {
    elements->getElements()[elements->getSize() - 1] = Traits::TERMINATOR; // no need to copy on write
    return getBuffer();
  }

// *******************************************************************************************
//   FRIEND SECTION
// *******************************************************************************************

  /**
    Writes string to format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException);
};

template<>
int compare<String>(const String& a, const String& b) throw();

/**
  Writes string to format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const String& value) throw(IOException);

/**
  Writes a string literal to a format stream.
*/
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const StringLiteral& value) throw(IOException) {
  stream.addCharacterField(value, value.getLength());
  return stream;
}

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline String operator+(const String& left, const String& right) throw(MemoryException) {
  return String(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  String reduction. Removes suffix from string if and only if it ends with the suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas ("pre"-"suf") results in "pre").
*/
inline String operator-(const String& left, const String& right) throw(MemoryException) {
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength()); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
