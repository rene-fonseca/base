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
  This exception specifies that a container is empty.
  
  @ingroup collections exceptions
*/

class _COM_AZURE_DEV__BASE__API EmptyContainer : public ContainerException {
public:

  /**
    Initializes the exception object with no message.
  */
  EmptyContainer() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  EmptyContainer(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  EmptyContainer(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  EmptyContainer(const char* message, const Type& type) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
