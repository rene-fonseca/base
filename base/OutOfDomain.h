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
  Exception used to specify that a variable has been assigned a value not in the
  valid domain. This exception if offen raised on invalid arguments.
  
  @short Out of domain exception.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OutOfDomain : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline OutOfDomain() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline OutOfDomain(const char* message) throw() : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline OutOfDomain(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline OutOfDomain(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
