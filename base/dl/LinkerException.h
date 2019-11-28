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
  This exception is used by the DynamicLinker class.
  
  @short Exception raised by DynamicLinker.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LinkerException : public Exception {
public:
  
  /**
    Initializes the exception object with no message.
  */
  LinkerException() noexcept;
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  LinkerException(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  LinkerException(const Type& type) noexcept;
  
  /**
    Initializes the exception object.

    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  LinkerException(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
