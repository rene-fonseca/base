/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_H
#define _DK_SDU_MIP__BASE_STRING__STRING_H

#include <base/Object.h>
#include <base/OutOfRange.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/AllocatorEnumeration.h>
#include <limits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This structure binds together a string literal and its size. Use the macro MESSAGE to generate an object of this class for a given string literal (e.g. MESSAGE("Hello World")). Do not call the constructor manually.
*/
struct StringLiteral {
  /** The number of bytes occupied by the message including a terminator. */
  const unsigned int size;
  /** NULL-terminated message. */
  const char* message;
  /** Initializes message. Automatically invocated by the macro MESSAGE. */
  inline StringLiteral(unsigned int s, const char* m) : size(s), message(m) {}
  /** Cast to the usual message type. */
  inline operator const char*() const {return message;}
};

/** This macro generates a StringLiteral object from a string literal. */
#define MESSAGE(msg) StringLiteral(sizeof(msg), msg)

/**
  This structure binds together a wide string literal and its size. Use the macro WIDEMESSAGE to generate an object of this class for a given wide string literal (e.g. WIDEMESSAGE("Hello World")). Do not call the constructor manually.
*/
struct WideStringLiteral {
  /** The number of bytes occupied by the message including a terminator. */
  const unsigned int size;
  /** NULL-terminated message. */
  const wchar_t* message;
  /** Initializes message. Automatically invocated by the macro WIDEMESSAGE. */
  inline WideStringLiteral(unsigned int s, const wchar_t* m) : size(s), message(m) {}
  /** Cast to the usual message type. */
  inline operator const wchar_t*() const {return message;}
};

/** This macro generates a WideStringLiteral object from a string literal. */
#define WIDEMESSAGE(msg) WideStringLiteral(sizeof(L ## msg), L ## msg)

/**
  String class. The first modifing operation on a string may force the internal
  buffer to be duplicated. The implementation is currently NOT MT-safe.

  @short String.
  @author René Møller Fonseca
  @version 1.0
*/

class String : public virtual Object {
public:

  /** Specifies the string terminator. */
  static const char TERMINATOR = '\0';
  /** Specifies the granularity of the capacity. Guaranteed to be greater than 0. */
  static const unsigned int GRANULARITY = 16;
  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. Unresolved problem: size of int depends on architecture. */
  static const unsigned int MAXIMUM_LENGTH = ((INT_MAX - sizeof(TERMINATOR))/GRANULARITY)*GRANULARITY;

  class Enumeration;
  friend class Enumeration;
  class ReadOnlyEnumeration;
  friend class ReadOnlyEnumeration;

  /**
    Enumeration of all the elements of a string.
  */
  class Enumeration : public AllocatorEnumeration<char, char&, char*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified string.

      @param string The string being enumerated.
    */
    Enumeration(String& string) throw() :
      AllocatorEnumeration<char, char&, char*>(string.getMutableBuffer(), string.getMutableBuffer() + string.getLength()) {}
  };

  /**
    Non-modifying enumeration of all the elements of a string.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<char, const char&, const char*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified string.

      @param string The string being enumerated.
    */
    ReadOnlyEnumeration(const String& string) throw() :
      AllocatorEnumeration<char, const char&, const char*>(string.getReadOnlyBuffer(), string.getReadOnlyBuffer() + string.getLength()) {}
  };
private:

  /**
    Reference to an element within a map.
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
  inline char* getMutableBuffer() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns a non-modifiable buffer.
  */
  inline const char* getReadOnlyBuffer() const throw() {
    return elements->getElements();
  }

  /**
    Returns the length of a native NULL-terminated string (-1 if not terminated).
  */
  int getLengthOfString(const char* str) const throw();

  /**
    Sets the length of the string. Also terminates the string properly.
  */
  void setLength(unsigned int length) throw(MemoryException);

  /**
    Used by constructors to create the buffer.
  */
//  void createString(const char* buffer, unsigned int length, unsigned int capacity) throw(MemoryException);

  /**
    Compare two null-terminated strings.
  */
  static int compareToIgnoreCase(const char* l, const char* r) throw();
public:

  /**
    Initializes an empty string.
  */
  String() throw(MemoryException);

  /**
    Initializes a string with no characters in it, initial capacity and granularity of capacity.

    @param capacity The initial capacity.
  */
  explicit String(unsigned int capacity) throw(MemoryException);

  /**
    Initializes the string from a string literal. The string literal is not copied into internal buffer. Implicit initialization is allowed.

    @param str String literal generated by the macro MESSAGE (e.g. MESSAGE("My string"))
  */
  String(const StringLiteral& str) throw(MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param str NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
  */
  String(const char* str) throw(MemoryException);

  /**
    Initializes the string from a NULL-terminated string. If the length of the
    specified string (str) exceeds the maximum length (n) only the first n
    characters are used.

    @param str NULL-terminated string. If NULL, the string is initialized with
    no characters in it.
    @param maximum Specifies the maximum length.
  */
  String(const char* str, unsigned int maximum) throw(MemoryException);

  /**
    Initializes string from other string.
  */
  inline String(const String& copy) throw() : elements(copy.elements) {}

  /**
    Assignment of string with string.
  */
  String& operator=(const String& eq) throw();

  /**
    Assignment of string with NULL-terminated string.
  */
  String& operator=(const char* str) throw(MemoryException);

  /**
    Returns the number of characters in the string.
  */
  inline unsigned int getLength() const throw() {return elements->getSize() - 1;}

  /**
    Returns true if the string contains no characters.
  */
  inline bool isEmpty() const throw() {return elements->isEmpty();}

  /**
    Returns the capacity of the string.
  */
  inline unsigned int getCapacity() const throw() {return elements->getCapacity();}

  /**
    Ensures that the capacity of the buffer is at least equal to the specified minimum.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(unsigned int capacity) throw(MemoryException);

  /**
    Releases any unused capacity of the string.
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
  inline String& append(char ch) throw(MemoryException) {return insert(getLength(), ch);}

  /**
    Appends the string to this string.

    @param str The string to be appended.
  */
  inline String& append(const String& str) throw(MemoryException) {return insert(getLength(), str);}

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
  */
  inline String& append(const char* str) throw(MemoryException) {return insert(getLength(), str);}

  /**
    Appends the NULL-terminated string to this string.

    @param str The string to be appended.
    @param maximum The maximum length of the to be appended string.
  */
  String& append(const char* str, unsigned int maximum) throw(MemoryException);

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline String& prepend(char ch) throw(MemoryException) {return insert(0, ch);}

  /**
    Prepends the string to this string.

    @param str The string to be prepended.
  */
  inline String& prepend(const String& str) throw(MemoryException) {return insert(0, str);}

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index
    exceeds the end of this string the character is inserted at the end.

    @param ch The character to be inserted.
  */
  String& insert(unsigned int index, char ch) throw(MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param str The string to be inserted.
  */
  String& insert(unsigned int index, const String& str) throw(MemoryException);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index
    exceeds the end of this string the string is inserted at the end.

    @param str The NULL-terminated string to be inserted.
  */
  String& insert(unsigned int index, const char* str) throw(MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters
    in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param str The string to replace with.
  */
  String& replace(unsigned int start, unsigned int end, const String& str) throw(MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in
    this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(const String& fromStr, const String& toStr) throw(MemoryException);

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
  char* substring(char* buffer, unsigned int start, unsigned int end) const throw();

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
  int compareTo(const char* str) const throw();

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
  int compareToIgnoreCase(const char* str) const throw();

  /**
    Returns true if this string starts with prefix.

    @param prefix The string to compare start of this string with.
  */
  bool startsWith(const String& prefix) const throw();

  /**
    Returns true if this string ends with suffix.

    @param suffix The string to compare end of this string with.
  */
  bool endsWith(const String& suffix) const throw();

  /**
    Equality operator.
  */
  inline bool operator==(const String& str) const throw() {return compareTo(str) == 0;};

  /**
    Inequality operator.
  */
  inline bool operator!=(const String& str) const throw() {return compareTo(str) != 0;};

  /**
    Less than operator.
  */
  inline bool operator<(const String& str) const throw() {return compareTo(str) < 0;};

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const String& str) const throw() {return compareTo(str) <= 0;};

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const String& str) const throw() {return compareTo(str) >= 0;};

  /**
    Greater than operator.
  */
  inline bool operator>(const String& str) const throw() {return compareTo(str) > 0;};

  /**
    Equality operator.
  */
  inline bool operator==(const char* str) const throw() {return compareTo(str) == 0;};

  /**
    Inequality operator.
  */
  inline bool operator!=(const char* str) const throw() {return compareTo(str) != 0;};

  /**
    Less than operator.
  */
  inline bool operator<(const char* str) const throw() {return compareTo(str) < 0;};

  /**
    Less than or equal operator.
  */
  inline bool operator<=(const char* str) const throw() {return compareTo(str) <= 0;};

  /**
    Greater than or equal operator.
  */
  inline bool operator>=(const char* str) const throw() {return compareTo(str) >= 0;};

  /**
    Greater than operator.
  */
  inline bool operator>(const char* str) const throw() {return compareTo(str) > 0;};

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
  inline int lastIndexOf(char ch) const throw() {return lastIndexOf(ch, getLength());};

  /**
    Returns the index of the last substring that matches the specified string before the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int lastIndexOf(const String& str, unsigned int start) const throw();
  inline int lastIndexOf(const String& str) const throw() {return lastIndexOf(str, getLength());};

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
  inline const char* getBytes() const {return getReadOnlyBuffer();};

  /**
    Cast string to null-terminated string.
  */
  operator const char*() const {return getReadOnlyBuffer();};

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
    return left.substring(0, left.getLength() - right.getLength() - 1); // return copy of left without suffix
  } else {
    return String(left); // return copy of left
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
