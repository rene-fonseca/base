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

#include <base/collection/ContainerException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception specifies that a key of a collection is invalid. Normally this
  means that the key has not been associated with a value.
  
  @short Invalid key exception.
  @ingroup collections exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InvalidContainer : public ContainerException {
public:
  
  /**
    Initializes the exception object with no message.
  */
  InvalidContainer() noexcept;
  
  /**
    Initializes the exception object.
    
    @param message The message.
  */
  InvalidContainer(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  InvalidContainer(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  InvalidContainer(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
