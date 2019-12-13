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
  Raised on object deconstruction errors. This exception is used to detect
  resource leaks.

  @short Destruction exception.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Destruct : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Destruct() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  Destruct(const char* message) noexcept;
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
