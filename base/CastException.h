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
  This exception is raised on invalid casts.
  
  @short Cast exception.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CastException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  CastException() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  CastException(const char* message) noexcept;
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  CastException(const Type& type) noexcept;
  
  /**
    Initializes the exception with a message and type.

    @param message The message.
    @param type The type.
  */
  CastException(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
