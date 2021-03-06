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
  This exception indicates that a resulting value is less than the maximum
  negative representable value using a given format/type. An underflow
  condition will occur if the result of subtracting two 32 bit signed integers
  is less than -2^31.

  @short Underflow exception.
  @see Overflow
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Underflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Underflow() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  Underflow(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  Underflow(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  Underflow(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
