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

#include <base/concurrency/LockException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Exception raised by Event synchronization objects.
  
  @short Event exception.
  @ingroup concurrency exception
  @see Event
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API EventException : public LockException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline EventException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline EventException(const char* message) noexcept
    : LockException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline EventException(Type type) noexcept : LockException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline EventException(const char* message, Type type) noexcept
    : LockException(message, type) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
