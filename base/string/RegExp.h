/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__REGEX_H
#define _DK_SDU_MIP__BASE_STRING__REGEX_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/OutOfRange.h>
#include <base/collection/Array.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Regular expression
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class RegExp : public Object {
public:

  /**
    Exception thrown by the RegExp class.
  */
  class RegExpException : public Exception {
  public:

    inline RegExpException() throw() {}

    inline RegExpException(const char* message) throw() : Exception(message) {}
  };

  /**
    @short Substring descriptor.
  */
  class Substring : public Object {
    friend class RegExp;
  private:

    /** The offset of the substring. */
    int offset;
    /** The end of the substring. */
    int end;

    /**
      Initializes substring descriptor.
    */
    inline Substring(unsigned int o, unsigned int e) throw() : offset(o), end(e) {}
  public:

    /**
      Initializes substring descriptor as invalid.
    */
    inline Substring() throw() : offset(-1), end(-1) {}

    /**
      Initializes substring descriptor from other substring descriptor.
    */
    inline Substring(const Substring& copy) throw() : offset(copy.offset), end(copy.end) {}

    /**
      Assignment of substring by substring.
    */
    inline Substring operator=(const Substring& eq) throw() {
      offset = eq.offset;
      end = eq.end;
      return *this;
    }

    /**
      Returns the offset of the substring.
    */
    inline int getOffset() const throw() {return offset;}

    /**
      Returns the length of the substring.
    */
    inline int getLength() const throw() {return end - offset;}

    /**
      Returns true if the substring is empty.
    */
    inline bool isEmpty() const throw() {return end - offset;}

    /**
      Returns true if the substring is valid. Use this method on the substring
      returned by match(...) to check whether the string matched the regular
      expression.
    */
    inline bool isValid() const throw() {return (offset >= 0) && (end >= 0);}

    /**
      Returns the substring of the specified string as described by this object.
    */
    inline String substring(const String& value) throw(MemoryException) {
      return value.substring(offset, end);
    }

    /**
      Returns true if this object describes the specified string completely.
    */
    inline bool isComplete(const String& value) const throw() {
      return (offset == 0) && (static_cast<unsigned int>(end) == value.getLength());
    }
  };
private:

  /** The pattern. */
  String pattern;
  /** Internal compilation data. */
  void* compiled;
  /** Specifies the case sensitivity. */
  bool caseSensitive;
protected:

  /** Compiles the regular expression. */
  void compile() throw(MemoryException);
  /** Releases any resources. */
  void release() throw();
public:

  /**
    Initializes the regular expression in case sensitive mode.
  */
  RegExp() throw();

  /**
    Initializes the regular expression with the specified pattern.

    @param pattern The pattern.
    @param caseSensitive Specifies the case sensitivity. Default is true.
  */
  RegExp(const String& pattern, bool caseSensitivity = true) throw(MemoryException);

  /**
    Returns true if case sensitive.
  */
  inline bool isCaseSensitive() const throw() {return caseSensitive;}

  /**
    Returns true if the regular expression is valid.
  */
  inline bool isValid() const throw() {return compiled;}

  /**
    Returns the pattern.
  */
  inline String getPattern() const throw() {return pattern;}

  /**
    Sets the case sensitivity mode.

    @param caseSensitive Specifies the case sensitivity. True selects case sensitive matching.
  */
  inline void setCaseSensitivity(bool value) throw() {this->caseSensitive = caseSensitive;}

  /**
    Sets the pattern of the regular expression.
  */
  void setPattern(const String& pattern) throw(MemoryException);

  /**
    Matches the specified string with the regular expression. Throws OutOfRange
    if the start index exceeds the end of the string to be matched.

    @param value The string to be matched.
    @param start The start index. The default is 0.
    @return The substring matched by the entire pattern.
  */
  Substring match(const String& value, unsigned int start = 0) throw(RegExpException, OutOfRange);

  /**
    Matches the specified string with the regular expression. Throws OutOfRange
    if the start index exceeds the end of the string to be matched.

    @param value The string to be matched.
    @param arguments The array to receive the captured substrings from the matched string.
    @param start The start index. The default is 0.
    @return The substring matched by the entire pattern.
  */
  Substring match(const String& value, Array<Substring>& arguments, unsigned int start = 0) throw(RegExpException, OutOfRange);

  /**
    Destroys the regular expression.
  */
  inline ~RegExp() throw() {release();}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
