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
  This exception is raised when a key is already in used by some collection
  class. This can happen when an object is registered multiple times or when
  different objects with identical values are registered.
  
  @short Already key exception.
  @ingroup collections exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AlreadyKeyException : public Exception {
public:
  
  /**
    Initializes the exception with no message.
  */
  AlreadyKeyException() noexcept;

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  AlreadyKeyException(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  AlreadyKeyException(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  AlreadyKeyException(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
