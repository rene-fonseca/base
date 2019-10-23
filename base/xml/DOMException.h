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
  DOM exception.

  @short DOM exception.
  @ingroup exceptions xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DOMException : public Exception {
public:

  /** Causes. */
  enum Cause {
    INDEX_SIZE,
    DOM_STRING_SIZE,
    HIERARCHY_REQUEST,
    WRONG_DOCUMENT,
    INVALID_CHARACTER,
    NO_DATA_ALLOWED,
    NO_MODIFICATION_ALLOWED,
    NOT_FOUND,
    NOT_SUPPORTED,
    ATTRIBUTE_IN_USE,
    INVALID_STATE,
    SYNTAX_ERROR,
    INVALID_MODIFICATION,
    NAMESPACE_ERROR,
    INVALID_ACCESS
  };
  
  /**
    Initializes the exception with no message.
  */
  inline DOMException() noexcept {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline DOMException(const char* message) noexcept : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline DOMException(Type type) noexcept : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline DOMException(const char* message, Type type) noexcept
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
