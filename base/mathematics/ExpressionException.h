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

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised by ExpressionParser on errors in an expression
  representation.

  @short Expression exception.
  @see ExpressionParser
  @ingroup exceptions mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ExpressionException : public Exception {
protected:

  /** Specifies the index the error in the expression representation. */
  unsigned int index = 0;
public:

  /**
    Initializes the exception with no message.
  */
  ExpressionException() throw();

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  ExpressionException(const char* message) throw();

  /**
    Initializes the exception with the specified index and message.

    @param index The index of the error.
    @param message The message.
  */
  ExpressionException(unsigned int index, const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ExpressionException(Type type) throw() : Exception(type), index(0) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ExpressionException(const char* message, Type type) throw()
    : Exception(message, type), index(0) {
  }
  
  /**
    Returns the index of the error in the expression representation.
  */
  inline unsigned int getIndex() const throw() {
    return index;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
