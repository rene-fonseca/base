/***************************************************************************
    copyright            : (C) 2000 by Ren� M�ller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_H
#define _DK_SDU_MIP__BASE_STRING__STRING_H

#include "base/Object.h"
#include "base/OutOfRange.h"
#include "base/mem/Buffer.h"
#include "base/mem/ReferenceCountedObjectPointer.h"
#include "base/mem/ReferenceCountedObject.h"
#include <limits.h>

/**
  This structure binds together a string literal and its size. Use the macro MESSAGE to generate an object of this class for a given string literal. Do not call the constructor manually.
*/
struct StringLiteral {
  /** The number of bytes occupied by the message including a terminator. */
  const unsigned int size;
  /** NULL-terminated message. */
  const char* message;
  /** Initializes message. Automatically invocated by the macro MESSAGE. */
  inline StringLiteral(unsigned int s, const char* m) : size(s), message(m) {};
  /** Cast to the usual message type. */
  inline operator const char*() const {return message;};
};

/** This macro generates a StringLiteral object from a string literal. */
#define MESSAGE(msg) StringLiteral(sizeof(msg), msg)

/**
  This structure binds together a wide string literal and its size. Use the macro WIDEMESSAGE to generate an object of this class for a given wide string literal. Do not call the constructor manually.
*/
struct WideStringLiteral {
  /** The number of bytes occupied by the message including a terminator. */
  const unsigned int size;
  /** NULL-terminated message. */
  const wchar_t* message;
  /** Initializes message. Automatically invocated by the macro WIDEMESSAGE. */
  inline WideStringLiteral(unsigned int s, const wchar_t* m) : size(s), message(m) {};
  /** Cast to the usual message type. */
  inline operator const wchar_t*() const {return message;};
};

/** This macro generates a WideStringLiteral object from a string literal. */
#define WIDEMESSAGE(msg) WideStringLiteral(sizeof(L ## msg), L ## msg)

/**
  String class. The first modifing operation on a string may force a duplication of the internal buffer. The implementation is currently NOT MT-safe.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class String : public Object {
public:

  /** Specifies the string terminator. */
  static const char TERMINATOR = '\0';
  /** Specifies the granularity of the capacity. Guaranteed to be greater than 0. */
  static const unsigned int GRANULARITY = 16;
  /** Specifies the default capacity of a string. Guaranteed to be greater than 0. */
  static const unsigned int DEFAULT_CAPACITY = GRANULARITY;
  /** Specifies the maximum length of any string. Guarantees that an int can hold the length of the string. Unresolved problem: size of int depends on architecture. */
  static const unsigned int MAXIMUM_LENGTH = ((INT_MAX - sizeof(TERMINATOR))/GRANULARITY)*GRANULARITY;
private:

  class StringBuffer : public Buffer, public ReferenceCountedObject {
  public:
    StringBuffer(unsigned int capacity) throw(MemoryException) : Buffer(capacity) {};
    StringBuffer(char* buffer, unsigned int size) throw(MemoryException) : Buffer(buffer, size) {};
    StringBuffer(const StringBuffer& copy) throw(MemoryException) : Buffer(copy), ReferenceCountedObject() {};
  };

  /** Buffer holding the NULL-terminated string. */
  ReferenceCountedObjectPointer<StringBuffer> internal;
  /** Length of the string. */
  unsigned int len;
protected:

  /** Prepares buffer for modification. Forces copy of internal buffer if shared by multiple objects. */
  inline void prepareForModification() throw(MemoryException) {
    if ((internal->getReferencesToObject() > 1) || (!internal->isManaged())) { // do we need to copy internal buffer
      internal = new StringBuffer(*internal);
    }
  }
  /** Returns a modifiable buffer. Forces copy of internal buffer if shared by multiple objects. */
  inline char* getMutableBuffer() throw(MemoryException) {
    prepareForModification();
    return internal->getBytes();
  };
  /** Returns a read-only buffer. */
  inline const char* getReadOnlyBuffer() const throw() {return internal->getBytes();};
  /** Returns the length of a NULL-terminated string (-1 if not terminated). */
  int getLengthOfString(const char* str) const throw();
  /** Fills this string with characters from buffer. Does NOT validate request. */
  String& fill(const char* src, unsigned int start, unsigned int count) throw();
  /** Fills this string with characters from buffer and terminates stirng. Does NOT validate request. */
  String& fillAndTerminate(const char* src, unsigned int start, unsigned int count) throw();
  /** Sets the length of the string. Also terminates the string properly. */
  void setLength(unsigned int length) throw(MemoryException);
public:

  /**
    Initializes the string from a string literal. The string literal is not copied into internal buffer. Implicit initialization is allowed.

    @param str String literal generated by the macro MESSAGE (e.g. MESSAGE("My string"))
  */
  String(const StringLiteral& str) throw(MemoryException);

  /**
    Initializes a string with no characters in it, initial capacity and granularity of capacity.

    @param capacity The initial capacity. Default is given by DEFAULT_CAPACITY.
  */
  explicit String(unsigned int capacity = DEFAULT_CAPACITY) throw(MemoryException);

  /**
    Initializes the string from a NULL-terminated string.

    @param str NULL-terminated string. If NULL, the string is initialized with no characters in it.
  */
  String(const char* str) throw(MemoryException);

  /**
    Initializes the string from a NULL-terminated string. If the length of the specified string (str) exceeds the maximum length (n) only the first n characters are used.

    @param str NULL-terminated string. If NULL, the string is initialized with no characters in it.
    @param maximum Specifies the maximum length.
  */
  String(const char* str, unsigned int maximum) throw(MemoryException);

  /**
    Copy constructor.
  */
  inline String(const String& copy) throw(MemoryException) : internal(copy.internal), len(copy.len) {};

  /**
    Returns the number of characters in the string.
  */
  inline unsigned int length() const throw() {return len;};

  /**
    Returns true if the string contains no characters.
  */
  inline bool isEmpty() const throw() {return length() == 0;};

  /**
    Returns the capacity of the string.
  */
  inline unsigned int getCapacity() const throw() {return internal->getSize();};

  /**
    Ensures that the capacity of the buffer is at least equal to the specified minimum.

    @param capacity Specifies the minimum capacity of the string.
  */
  void ensureCapacity(unsigned int capacity) throw(MemoryException);

  /**
    Releases any unused capacity of the string.
  */
  void optimizeCapacity() throw(MemoryException);

// *******************************************************************************************
//   CHARACTER SECTION
// *******************************************************************************************

  /**
    Returns the character at the specified index in this string. Throws 'OutOfRange' if index exceeds the length of the string.
  */
  char getChar(unsigned int index) throw(OutOfRange);

  /**
    Sets the character at the specified index of this string. Throws 'OutOfRange' if index exceeds the length of the string.

    @param index The index of the character to set.
    @param value The new character value.
  */
  void setChar(unsigned int index, char value) throw(OutOfRange);

  /**
    Returns the character at the specified index. Throws 'OutOfRange' if index exceeds the length of the string.
  */
  char operator[](unsigned int index) const throw(OutOfRange);

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
  inline String& removeCharacter(unsigned int index) throw(MemoryException) {return remove(index, index);};

  /**
    Appends the character to this string.

    @param ch The character to be appended.
  */
  inline String& append(char ch) throw(MemoryException) {return insert(length(), ch);};

  /**
    Appends the string to this string.

    @param str The string to be appended.
  */
  inline String& append(const String& str) throw(MemoryException) {return insert(length(), str);};

  /**
    Prepends the character to this string.

    @param ch The character to be prepended.
  */
  inline String& prepend(char ch) throw(MemoryException) {return insert(0, ch);};

  /**
    Prepends the string to this string.

    @param str The string to be prepended.
  */
  inline String& prepend(const String& str) throw(MemoryException) {return insert(0, str);};

  /**
    Inserts the character into this string.

    @param index Specifies the position to insert the character. If the index exceeds the end of this string the character is inserted at the end.
    @param ch The character to be inserted.
  */
  String& insert(unsigned int index, char ch) throw(MemoryException);

  /**
    Inserts the string into this string.

    @param index Specifies the position to insert the string. If the index exceeds the end of this string the string is inserted at the end.
    @param str The string to be inserted.
  */
  String& insert(unsigned int index, const String& str) throw(MemoryException);

  /**
    Inserts NULL-terminated string into this string.

    @param index Specifies the position to insert the string. If the index exceeds the end of this string the string is inserted at the end.
    @param str The NULL-terminated string to be inserted.
  */
  String& insert(unsigned int index, const char* str) throw(MemoryException);

  /**
    Replaces the characters in a substring of this string with the characters in the specified string.

    @param start The start of the substring.
    @param end The end of the substring.
    @param str The string to replace with.
  */
  String& replace(unsigned int start, unsigned int end, const String& str) throw(MemoryException);

  /**
    Replaces all occurances of the specified substring with another string in this string. Only does one pass of this string.

    @param fromStr The substring to be replaced.
    @param toStr The new string.
    @return The number of substrings that was replaced.
  */
  unsigned int replaceAll(const String& fromStr, const String& toStr) throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently contained in this string. If 'end' is less than 'start' an empty string is returned.

    @param start Specifies the start of the substring.
    @param end Specifies the end of the substring.
  */
  String substring(unsigned int start, unsigned int end) const throw(MemoryException);

  /**
    Returns a new string that contains a subsequence of characters currently contained in this string. The substring begins at the specified index and extends to the end of the string.

    @param start Specifies the start of the substring.
  */
  inline String substring(unsigned int start) const throw(MemoryException) {return substring(start, length());};

  /**
    Appends the string to this string.

    @param suffix The string to be appended.
  */
  inline String& operator+=(const String& suffix) throw(MemoryException) {return append(suffix);};

  /**
    String reduction operator. Removes suffix from this string if and only if it ends with the suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas ("pre"-"suf") results in "pre").

    @param suffix The suffix to be removed.
  */
  String& operator-=(const String& suffix) throw(MemoryException);

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
    Compares this string with other string ignoring the case of the characters.

    @param str The string to compare this string with.
    @return Integer less than, equal to, or greater than zero if this string is found, respectively, to be less than, equal to, or greater than the specified string.
  */
  int compareToIgnoreCase(const String& str) const throw();

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
  inline int lastIndexOf(char ch) const throw() {return lastIndexOf(ch, length());};

  /**
    Returns the index of the last substring that matches the specified string before the start position.

    @param str The substring to find.
    @param start Specifies the start position of the search. Default is end of string.
    @return Index of the last match if any otherwise -1. Also returns -1 if substring is empty.
  */
  int lastIndexOf(const String& str, unsigned int start) const throw();
  inline int lastIndexOf(const String& str) const throw() {return lastIndexOf(str, length());};

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
    Converts this string to another string.
  */
  String toString() const;

  /**
    Destroys the string.
  */
  ~String() throw();
};

/**
  String concatenation operator.
*/
String operator+(const String& left, const String& right) throw(MemoryException);

/**
  String reduction operator. Removes suffix from string if and only if it ends with the suffix (e.g. ("presuf"-"suf") results in a new string "pre" whereas ("pre"-"suf") results in "pre").
*/
String operator-(const String& left, const String& right) throw(MemoryException);

#endif
