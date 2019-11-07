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
#include <base/string/String.h>
#include <base/OutOfRange.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Regular expression pattern matching.
  
  @short Regular expression
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API RegExp : public Object {
public:

  /**
    Exception raised by the RegExp class.
    
    @short Regular expression exception
    @ingroup exceptions
    @see RegExp
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API RegExpException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline RegExpException() noexcept {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline RegExpException(const char* message) noexcept : Exception(message) {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline RegExpException(const Type& type) noexcept : Exception(type) {
    }
    
    /**
      Initializes the exception object.
      
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline RegExpException(const char* message, const Type& type) noexcept
      : Exception(message, type) {
    }
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };

  /**
    @short Substring descriptor.
  */
  class _COM_AZURE_DEV__BASE__API Substring : public Object {
    friend class RegExp;
  private:

    /** The offset of the substring. */
    int offset = 0;
    /** The end of the substring. */
    int end = 0;

    /**
      Initializes substring descriptor.
    */
    inline Substring(unsigned int _offset, unsigned int _end) throw()
      : offset(_offset), end(_end) {
    }
  public:

    /**
      Initializes substring descriptor as invalid.
    */
    inline Substring() throw() : offset(-1), end(-1) {
    }

    /**
      Initializes substring descriptor from other substring descriptor.
    */
    inline Substring(const Substring& copy) throw()
      : offset(copy.offset), end(copy.end) {
    }

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
    inline int getOffset() const throw() {
      return offset;
    }

    /**
      Returns the length of the substring.
    */
    inline int getLength() const throw() {
      return end - offset;
    }

    /**
      Returns true if the substring is empty.
    */
    inline bool isEmpty() const throw() {
      return end - offset;
    }

    /**
      Returns true if the substring is valid. Use this method on the substring
      returned by match(...) to check whether the string matched the regular
      expression.
    */
    inline bool isValid() const throw() {
      return (offset >= 0) && (end >= 0);
    }

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
      return (offset == 0) &&
        (static_cast<unsigned int>(end) == value.getLength());
    }
  };
private:

  /** The pattern. */
  String pattern;
  /** Internal compilation data. */
  void* compiled = nullptr;
  /** Specifies the case sensitivity. */
  bool caseSensitive = false;
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
  RegExp(
    const String& pattern, bool caseSensitivity = true) throw(MemoryException);

  /**
    Returns true if case sensitive.
  */
  inline bool isCaseSensitive() const throw() {
    return caseSensitive;
  }

  /**
    Returns true if the regular expression is valid.
  */
  inline bool isValid() const throw() {
    return compiled;
  }

  /**
    Returns the pattern.
  */
  inline const String& getPattern() const throw() {
    return pattern;
  }

  /**
    Sets the case sensitivity mode.

    @param caseSensitive Specifies the case sensitivity. True selects case
    sensitive matching.
  */
  inline void setCaseSensitivity(bool _caseSensitive) throw() {
    caseSensitive = _caseSensitive;
  }

  /**
    Sets the pattern of the regular expression.
  */
  void setPattern(const String& pattern) throw(MemoryException);

  /**
    Matches the specified string with the regular expression. Raises OutOfRange
    if the start index exceeds the end of the string to be matched.

    @param value The string to be matched.
    @param start The start index. The default is 0.
    @return The substring matched by the entire pattern.
  */
  Substring match(
    const String& value,
    unsigned int start = 0) const throw(RegExpException, OutOfRange);

  /**
    Matches the specified string with the regular expression. Raises OutOfRange
    if the start index exceeds the end of the string to be matched.

    @param value The string to be matched.
    @param arguments The array to receive the captured substrings from the matched
    string.
    @param start The start index. The default is 0.
    @return The substring matched by the entire pattern.
  */
  Substring match(
    const String& value,
    Array<Substring>& arguments,
    unsigned int start = 0) const throw(RegExpException, OutOfRange);

  /**
    Destroys the regular expression.
  */
  inline ~RegExp() throw() {
    release();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
