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

#include <base/net/NetworkException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Specifies that the host doesn't exist or couldn't be resolved.
  
  @short Host not found exception.
  @ingroup exceptions net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API HostNotFound : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  HostNotFound() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  HostNotFound(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  HostNotFound(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  HostNotFound(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
