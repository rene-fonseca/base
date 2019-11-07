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
  This exception is raised when specified operands are incompatible with each
  other for some operation.
  
  @short Incompatible operands exception.
  @ingroup exceptions mathematics
  @version 1.0
*/

class IncompatibleOperands : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline IncompatibleOperands() noexcept {
  }
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  inline IncompatibleOperands(const char* message) noexcept
    : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline IncompatibleOperands(const Type& type) noexcept : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline IncompatibleOperands(const char* message, const Type& type) noexcept
    : Exception(message, type) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
