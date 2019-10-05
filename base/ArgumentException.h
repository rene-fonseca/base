/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ARGUMENT_EXCEPTION_H
#define _DK_SDU_MIP__BASE__ARGUMENT_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates an invalid argument.
  
  @short Argument exception.
  @see ArgumentParser
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ArgumentException : public Exception {
private:
  
  /** Argument number. */
  int argumentNumber;
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
  inline ArgumentException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ArgumentException(const char* message) throw()
    : Exception(message), argumentNumber(-1) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ArgumentException(Type type) throw()
    : Exception(type), argumentNumber(-1) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ArgumentException(const char* message, Type type) throw()
    : Exception(message, type), argumentNumber(-1) {
  }

  /**
    Returns the argument number. Negative if invalid.
  */
  inline int getArgumentNumber() const throw() {
    return argumentNumber;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
