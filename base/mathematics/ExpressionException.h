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
  This exception is raised by ExpressionParser on errors in an expression
  representation.

  @short Expression exception.
  @see ExpressionParser
  @ingroup exceptions mathematics
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ExpressionException : public Exception {
protected:

  /** Specifies the index the error in the expression representation. */
  unsigned int index = 0;
public:

  /**
    Initializes the exception with no message.
  */
  ExpressionException() noexcept;

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  ExpressionException(const char* message) noexcept;

  /**
    Initializes the exception with the specified index and message.

    @param index The index of the error.
    @param message The message.
  */
  ExpressionException(unsigned int index, const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ExpressionException(Type type) noexcept : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ExpressionException(const char* message, Type type) noexcept
    : Exception(message, type) {
  }
  
  /**
    Returns the index of the error in the expression representation.
  */
  inline unsigned int getIndex() const noexcept {
    return index;
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
