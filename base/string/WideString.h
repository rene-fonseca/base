/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__WIDE_STRING_H
#define _DK_SDU_MIP__BASE_STRING__WIDE_STRING_H

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/OutOfDomain.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/MultibyteException.h>
#include <base/string/WideStringException.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/Primitives.h>

#if defined(_DK_SDU_MIP__BASE__WIDE)
  #include <wctype.h> // TAG: alien header
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class FormatOutputStream;

#if !defined(_DK_SDU_MIP__BASE__WIDE)
  typedef int wint_t;
  inline int iswalnum(wint_t) throw() {return 0;}
  inline int iswalpha(wint_t) throw() {return 0;}
  inline int iswcntrl(wint_t) throw() {return 0;}
  inline int iswdigit(wint_t) throw() {return 0;}
  inline int iswgraph(wint_t) throw() {return 0;}
  inline int iswlower(wint_t) throw() {return 0;}
  inline int iswprint(wint_t) throw() {return 0;}
  inline int iswpunct(wint_t) throw() {return 0;}
  inline int iswspace(wint_t) throw() {return 0;}
  inline int iswupper(wint_t) throw() {return 0;}
  inline int iswxdigit(wint_t) throw() {return 0;}
  inline wint_t towlower(wint_t character) throw() {return character;}
  inline wint_t towupper(wint_t character) throw() {return character;}
#endif

/**
  This class binds together a wide string literal and its length. Use the macro
  WIDEMESSAGE to generate an object of this class for a given wide string
  literal (e.g. WIDEMESSAGE("Hello World")). Do not call the constructor
  manually.

  @short Wide string literal
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/
class WideStringLiteral {
private:

  /** The number of bytes occupied by the message without the terminator. */
  const unsigned int length;
  /** NULL-terminated message. */
  const wchar_t* message;
public:

  /** Initializes message. Automatically invocated by the macro WIDEMESSAGE. */
  inline WideStringLiteral(unsigned int l, const wchar_t* m) throw() : length(l), message(m) {}
  /** Cast to the usual message type. */
  inline operator const wchar_t*() const throw() {return message;}
  /** Returns the length of the string. */
  inline unsigned int getLength() const throw() {return length;}
};

/** This macro generates a WideStringLiteral object from a string literal. */
#define WIDEMESSAGE(msg) WideStringLiteral(sizeof(L ## msg)/sizeof(wchar_t) - 1, L ## msg)



/**
  Default wide-character properties and manipulators.

  @short Wide-character description.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
class WideTraits {
public:

  /** The type of a single character. */
  typedef wchar_t Character;
  /** Specifies the terminator for null-terminated strings. */
  static const wchar_t TERMINATOR = L'\0';

  /** Returns true if the character an alphabetic character. */
  static inline bool isAlpha(Character character) throw() {return iswalpha(character);}
  /** Returns true if the character an alphabetic character or a digit. */
  static inline bool isAlphaNum(Character character) throw() {return iswalnum(character);}
  /** Returns true if the character is lowercase. */
  static inline bool isLower(Character character) throw() {return iswlower(character);}
  /** Returns true if the character is uppercase. */
  static inline bool isUpper(Character character) throw() {return iswupper(character);}
  /** Returns true if the character is a digit. */
  static inline bool isDigit(Character character) throw() {return iswdigit(character);}
  /** Returns true if the character is a hex digit. */
  static inline bool isHexDigit(Character character) throw() {return iswxdigit(character);}
  /** Returns true if the character is a white space. */
  static inline bool isSpace(Character character) throw() {return iswspace(character);}
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(Character character) throw() {return iswpunct(character);}
  /** Returns true if the character is printable. */
  static inline bool isPrintable(Character character) throw() {return iswprint(character);}
  /** Returns true if the character is a visible character. */
  static inline bool isGraph(Character character) throw() {return iswgraph(character);}
  /** Returns true if the character is a control character. */
  static inline bool isControl(Character character) throw() {return iswcntrl(character);}
  /** Returns true if the character is an ASCII character. */
  static inline bool isASCII(Character character) throw() {return !(character & ~0177);} // TAG: fix this

  /** Converts the character to lowercase. */
  static inline Character toLower(Character character) throw() {return towlower(character);}
  /** Converts the character to uppercase. */
  static inline Character toUpper(Character character) throw() {return towupper(character);}

  class ToLowerCase {
  public:
    inline Character operator()(Character value) const throw() {return towlower(value);}
  };

  class ToUpperCase {
  public:
    inline Character operator()(Character value) const throw() {return towupper(value);}
  };
};

/**
  Wide-character string implementation. The first modifying operation on a
  string may force the internal buffer to be duplicated. The implementation is
  currently NOT MT-safe.

  <pre>
  WideString myString = WIDEMESSAGE("Hello, World!");
  WideString myOtherString = myString;
  </pre>

  @short Wide-character string.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class WideString : public virtual Object {
public:

  /** Character specific properties and manipulators. */
  typedef WideTraits Traits;
  /** The type of a single character. */
  typedef Traits::Character Character;

  /** Specifies the granularity of the capacity. Guaranteed to be greater than 0. */
  static const unsigned int GRANULARITY = 16;
  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. Unresolved problem: size of int depends on architecture. */
  static const unsigned int MAXIMUM_LENGTH = ((PrimitiveTraits<int>::MAXIMUM/sizeof(Character) - 1)/GRANULARITY)*GRANULARITY;

  /** The type of the modifying string iterator. */
  typedef ReferenceCountedCapacityAllocator<Character>::Iterator Iterator;
  /** The type of the non-modifying string iterator. */
  typedef ReferenceCountedCapacityAllocator<Character>::ReadIterator ReadIterator;
  /** The type of the modifying string enumerator. */
  typedef ReferenceCountedCapacityAllocator<Character>::Enumerator Enumerator;
  /** The type of the non-modifying string enumerator. */
  typedef ReferenceCountedCapacityAllocator<Character>::ReadEnumerator ReadEnumerator;
private:

  /** The default wide string. This is used to avoid multiple allocations of empty string buffers. */
  static const WideString DEFAULT_STRING;

  /**
    Reference to an element within a wide string.
  */
  class Reference {
  private:
    friend class WideString;
    WideString& string;
    unsigned int index;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    inline Reference(WideString& s, unsigned int i) : string(s), index(i) {}
  public:
    inline Reference& operator=(char value) throw(OutOfRange) {string.setAt(index, value); return *this;}
    inline operator Character() const throw(OutOfRange) {return string.getAt(index);}
  };

  /**
    Reference counted buffer holding the character array (possible
    NULL-terminated). The array is guarantied to be non-empty when the string
    has been initialized.
  */
  ReferenceCountedObjectPointer<ReferenceCountedCapacityAllocator<Character> > elements;

  /**
    Compare the null-terminated strings ignoring the case.
  */
  static int compareToIgnoreCase(const Character* left, const Character* right) throw();
protected:

  /**
    Returns a modifiable buffer. Forces the internal buffer to be copied if
    shared by multiple strings.
  */
  inline Character* getBuffer() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns a non-modifiable buffer.
  */
  inline const Character* getBuffer() const throw() {
    return elements->getElements();
  }

  /**
    Sets the length of the string.
  */
  inline void setLength(unsigned int length) throw(WideStringException, MemoryException) {
    assert(length <= MAXIMUM_LENGTH, WideStringException(this));
    elements.copyOnWrite(); // we are about to modify the buffer
    elements->setSize(length + 1);
  }
public:

  /**
    Returns a multibyte string from a NULL-terminated wide-string.
  */
  static String getMultibyteString(const wchar_t* string) throw(NullPointer, WideStringException);

  /**
    Initializes an empty string.
  */
  WideString() throw();

  /**
    Initializes a string with no characters in it, initial capacity and granularity of capacity.

    @param capacity The initial capacity of the internal buffer.
  */
  explicit WideString(unsigned int capacity) throw(MemoryException);

  /**
    Initializes the string from a string literal. The string literal is not copied into internal buffer. Implicit initialization is allowed.

    @param str String literal generated by the macro WIDEMESSAGE (e.g. WIDEMESSAGE("My string"))
  */
  WideString(const WideStringLiteral& string) throw(WideStringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param string NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
  */
  WideString(const Character* string) throw(WideStringException, MemoryException);

  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string (str) exceeds the maximum length (n) only the first n
    characters are used.

    @param str NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
    @param maximum Specifies the maximum length.
  */
  WideString(const Character* string, unsigned int maximum) throw(OutOfDomain, MemoryException);

  /**
    Initializes string from a null-terminated multi byte character string.
  */
  WideString(const char* string) throw(MultibyteException, MemoryException);

  WideString(const char* string, unsigned int maximum) throw(OutOfDomain, MultibyteException, MemoryException);

  /**
    Initializes string from other string.
  */
  inline WideString(const WideString& copy) throw() : elements(copy.elements) {}

  /**
    Assignment of string with string.
  */
  WideString& operator=(const WideString& eq) throw() {
    elements = eq.elements; // self assignment handled by automation pointer
    return *this;
  }

  /**
    Assignment of string literal to string.
  */
  WideString& operator=(const WideStringLiteral& eq) throw(WideStringException, MemoryException);

  /**
    Returns the number of characters in the string.
  */
  inline unsigned int getLength() const throw() {return elements->getSize() - 1;}

  /**
    Returns true if the string contains no characters.
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

// ****************************************************************************
//   TRAVERSE SECTION
// ****************************************************************************

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
    return elements->getEndIterator() - 1;
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
    return elements->getEndReadIterator() - 1;
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

// ****************************************************************************
//   CHARACTER SECTION
// ****************************************************************************

  /**
    Returns the character at the specified index in this string. Throws
    'OutOfRange' if index exceeds the length of the string.
  */
  Character getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. If the new
    character is the string terminator ('\0') then the string is cut off from
    the specified index. Throws 'OutOfRange' if index exceeds the length of
    the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setAt(unsigned int index, Character value) throw(OutOfRange);

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

// ****************************************************************************
//   SUBSTRING SECTION
// ****************************************************************************

  /**
    Removes the characters in a substring of this string.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  WideString& remove(unsigned int start, unsigned int end) throw(MemoryException);

  /**
    Removes the characters from the specified index to the end of the string.

    @param start Specifies the start of the string.
  */
  WideString& removeFrom(unsigned int start) throw(MemoryException);

  /**
    Removes the character at the specified position in this string.

    @param index Specifies the character to be removed.
  */
  inline WideString& removeCharacter(unsigned int index) throw(MemoryException) {return remove(index, index);}

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline WideString& append(Character ch) throw(WideStringException, MemoryException) {return insert(getLength(), ch);}

  /**
    Appends the string to this string.

    @param str The string to be appended.
  */
  inline WideString& append(const WideString& str) throw(WideStringException, MemoryException) {return insert(getLength(), str);}

  /**
    Appends the string literal to this string.

    @param str The string to be appended.
  */
  WideString& append(const WideStringLiteral& str) throw(WideStringException, MemoryException);

  /**
    Appends the string literal to this string.

    @param str The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  WideString& append(const WideStringLiteral& str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException);

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  WideString& append(const Character* str, unsigned int maximum) throw(OutOfDomain, WideStringException, MemoryException);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline WideString& prepend(Character ch) throw(WideStringException, MemoryException) {return insert(0, ch);}

  /**
    Prepends the string to this string.

    @param str The string to be prepended.
  */
  inline WideString& prepend(const WideString& str) throw(WideStringException, MemoryException) {return insert(0, str);}

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.

    @param ch The character to be inserted.
  */
  WideString& insert(unsigned int index, Character ch) throw(WideStringException, MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param str The string to be inserted.
  */
  WideString& insert(unsigned int index, const WideString& str) throw(WideStringException, MemoryException);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param str The NULL-terminated string to be inserted.
  */
  WideString& insert(unsigned int index, const WideStringLiteral& str) throw(WideStringException, MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param str The string to replace with.
  */
  WideString& replace(unsigned int start, unsigned int end, const WideString& str) throw(WideStringException, MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(const WideString& fromStr, const WideString& toStr) throw(WideStringException, MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. If 'end' is less than 'start' an empty string is
    returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  WideString substring(unsigned int start, unsigned int end) const throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently
    contained in this string. The substring begins at the specified index and
    extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline WideString substring(unsigned int start) const throw(MemoryException) {return substring(start, getLength());}

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline WideString& operator+=(const WideString& suffix) throw(MemoryException) {return append(suffix);}

  /**
    WideString reduction operator. Removes suffix from this string if and only if
    it ends with the suffix (e.g. ("presuf"-"suf") results in a new string
    "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  WideString& operator-=(const WideString& suffix) throw(MemoryException);

  /**
    Returns a NULL terminated string that contains a subsequence of characters
    currently contained in this string. If 'end' is less than 'start' an empty
    string is returned. Does nothing if buffer is NULL.

    @param buffer The buffer to receive the NULL terminated string.
    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.

    @return The specified buffer.
  */
  //Character* substring(Character* buffer, unsigned int start, unsigned int end) const throw();

// ****************************************************************************
//   UNARY SECTION
// ****************************************************************************

  /**
    The character sequence contained in this string is replaced by the reverse sequence.
  */
  WideString& reverse() throw();

  /**
    Converts the characters of this string into the lower case equivalents.
  */
  WideString& toLowerCase() throw();

  /**
    Converts the characters of this string into the upper case equivalents.
  */
  WideString& toUpperCase() throw();

// ****************************************************************************
//   COMPARE SECTION
// ****************************************************************************

  /**
    Compare this string with another string.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareTo(const WideString& str) const throw();

  /**
    Compare this string with null-terminated string.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareTo(const Character* str) const throw(WideStringException);

  /**
    Compares this string with other string ignoring the case of the characters.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareToIgnoreCase(const WideString& str) const throw();

  /**
    Compares this string with null-terminated string ignoring the case of the characters.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareToIgnoreCase(const Character* str) const throw(WideStringException);

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideString& prefix) const throw();

  /**
    Returns true if this string starts with the specified prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const WideStringLiteral& prefix) const throw();

  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideString& suffix) const throw();

  /**
    Returns true if this string ends with the specified suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const WideStringLiteral& suffix) const throw();

  /**
    Equality operator.
  */
  inline bool operator==(const WideString& str) const throw() {return compareTo(str) == 0;}

  /**
    Inequality operator.
  */
  inline bool operator!=(const WideString& str) const throw() {return compareTo(str) != 0;}

  /**
    Less than operator.
  */
  inline bool operator<(const WideString& str) const throw() {return compareTo(str) < 0;}

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const WideString& str) const throw() {return compareTo(str) <= 0;}

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const WideString& str) const throw() {return compareTo(str) >= 0;}

  /**
    Greater than operator.
  */
  inline bool operator>(const WideString& str) const throw() {return compareTo(str) > 0;}

// ****************************************************************************
//   FIND SECTION
// ****************************************************************************

  /**
    Returns the index of the first character that matches the specified character after the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1.
  */
  int indexOf(Character ch, unsigned int start = 0) const throw();

  /**
    Returns the index of the first substring that matches the specified string after the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is 0.
    @return Index of the first match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int indexOf(const WideString& str, unsigned int start = 0) const throw();

  /**
    Returns the index of the last character that matches the specified character before the start position.

    @param ch The character to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1.
  */
  int lastIndexOf(Character ch, unsigned int start) const throw();
  inline int lastIndexOf(Character ch) const throw() {return lastIndexOf(ch, getLength());}

  /**
    Returns the index of the last substring that matches the specified string before the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int lastIndexOf(const WideString& str, unsigned int start) const throw();
  inline int lastIndexOf(const WideString& str) const throw() {return lastIndexOf(str, getLength());}

  /**
    Returns the number of occurances of the specified character in this string.

    @param ch The character to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the character.
  */
  unsigned int count(Character ch, unsigned int start = 0) const throw();

  /**
    Counts the number of occurances of the specified substring in this string.

    @param str The substring to be counted.
    @param start The start position. Default is 0.
    @return The number of occurances of the substring.
  */
  unsigned int count(const WideString& str, unsigned int start = 0) const throw();

// ****************************************************************************
//   END SECTION
// ****************************************************************************

  /**
    Returns null-terminated wide string.
  */
  inline const Character* getElements() const throw() {
    Character* result = elements->getElements(); // no need to copy on write
    result[getLength()] = Traits::TERMINATOR;
    return result;
  }

// ****************************************************************************
//   FRIEND SECTION
// ****************************************************************************

  /**
    Writes string to format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException);
};

template<>
int compare<WideString>(const WideString& left, const WideString& right) throw();

/**
  Writes wide string to format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const WideString& value) throw(MultibyteException, IOException);

/**
  Returns a new string that is the concatenation of the two specified strings.
*/
inline WideString operator+(const WideString& left, const WideString& right) throw(MemoryException) {
  return WideString(left.getLength() + right.getLength()).append(left).append(right);
}

/**
  String reduction. Removes suffix from string if and only if it ends with the suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas ("pre"-"suf") results in "pre").
*/
inline WideString operator-(const WideString& left, const WideString& right) throw(MemoryException) {
  if (left.endsWith(right)) {
    return left.substring(0, left.getLength() - right.getLength() - 1); // return copy of left without suffix
  } else {
    return WideString(left); // return copy of left
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
