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

#include <base/io/IOException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Exception raised on timed out IO requests.

  @short Time out exception.
  @ingroup exceptions io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API TimedOut : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  TimedOut() noexcept;

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  TimedOut(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  TimedOut(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  TimedOut(const char* message, const Type& type) noexcept;
  
  inline bool isCommonException() const noexcept override
  {
    return true;
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
