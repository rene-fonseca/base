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

#include <base/Exception.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception indicates an invalid argument.
  
  @short Argument exception.
  @see ArgumentParser
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ArgumentException : public Exception {
private:
  
  /** Argument number. */
  int argumentNumber = -1;
public:

  /** Causes. */
  enum Cause {
    INVALID_SYMBOL, /**< Invalid short name. */
    INVALID_OPTION, /**< Option has not been registered. */
    VALUE_MISSING, /**< Value of option is missing. */
    VALUE_PRESENT, /**< Option does not permit a value. */
    INVALID_VALUE, /**< Value of option is invalid. */
    STRICT_VIOLATION, /**< Strict option is specified more than once. */
    MANDATORY_VIOLATION, /**< Mandatory option is not specified. */
    ORPHAN /**< Orphan value. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  ArgumentException() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  ArgumentException(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  ArgumentException(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  ArgumentException(const char* message, const Type& type) noexcept;

  /**
    Returns the argument number. Negative if invalid.
  */
  inline int getArgumentNumber() const noexcept {
    return argumentNumber;
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
