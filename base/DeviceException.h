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

#include <base/ResourceException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is raised on device access errors.
  
  @short Device access error.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DeviceException : public ResourceException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline DeviceException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline DeviceException(const char* message) noexcept
    : ResourceException(message)
  {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline DeviceException(Type type) noexcept
    : ResourceException(type)
  {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline DeviceException(const char* message, Type type) noexcept
    : ResourceException(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
