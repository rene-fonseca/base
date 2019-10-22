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
  Exception raised on singleton violations. This exception is raised when a
  singleton class is instantiated and an instance already exists in the context
  (or address space).
  
  @short Singleton violation exception.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SingletonException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline SingletonException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline SingletonException(const char* message) noexcept : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SingletonException(Type type) noexcept : Exception(type) {
  }  
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SingletonException(const char* message, Type type) noexcept
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
