/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__EXPRESSION_EXCEPTION_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__EXPRESSION_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is thrown by ExpressionParser on errors in an expression
  representation.

  @see ExpressionParser
  @ingroup exceptions mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ExpressionException : public Exception {
protected:

  /** Specifies the index the error in the expression representation. */
  unsigned int index;
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
  inline ExpressionException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ExpressionException(const char* message, Type type) throw() : Exception(message, type) {
  }
  
  /**
    Returns the index of the error in the expression representation.
  */
  unsigned int getIndex() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
