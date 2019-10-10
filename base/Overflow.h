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
  This exception indicates that a resulting value is greater than the maximum
  positive representable value using a given format/type. An overflow condition
  will occur if the result of adding two 32 bit signed integers is greater than
  2^31-1.

  @short Overflow exception.
  @see Underflow
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Overflow : public Exception {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline Overflow() throw() {
  }
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  inline Overflow(const char* message) throw() : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline Overflow(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline Overflow(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
