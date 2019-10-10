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
  inline CastException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline CastException(const char* message) throw() : Exception(message) {
  }
  
  /**
    Initializes the exception with a type.
    
    @param type The type.
  */
  inline CastException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception with a message and type.

    @param message The message.
    @param type The type.
  */
  inline CastException(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
