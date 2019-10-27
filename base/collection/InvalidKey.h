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
  This exception specifies that a key of a collection is invalid. Normally this
  means that the key has not been associated with a value.
  
  @short Invalid key exception.
  @ingroup collections exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InvalidKey : public Exception {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline InvalidKey() noexcept {
  }
  
  /**
    Initializes the exception object.
    
    @param message The message.
  */
  inline InvalidKey(const char* message) noexcept
    : Exception(message)
  {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline InvalidKey(Type type) noexcept
    : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline InvalidKey(const char* message, Type type) noexcept
    : Exception(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
