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
  This exception is raised on concurrency violations (i.e. multiple threads
  accessing a shared resource concurrently in violation with specified
  constraints). The normal behavior is to block a thread context until a shared
  resource becomes available but occasionally it is more appropriate to simply
  prohibit concurrent invocations. ConcurrencyException is considered an
  internal error just like parsing invalid arguments to a method and thus the
  exception should never be handled in a try-catch statement.
  
  @short Concurrency exception.
  @ingroup exception
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ConcurrencyException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline ConcurrencyException() noexcept
  {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ConcurrencyException(const char* message) noexcept
    : Exception(message)
  {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ConcurrencyException(const Type& type) noexcept
    : Exception(type)
  {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ConcurrencyException(const char* message, const Type& type) noexcept
    : Exception(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
