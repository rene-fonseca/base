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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates resource allocation, deallocation, and access error.

  @short Resource exception
  @ingroup exceptions
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API ResourceException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline ResourceException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ResourceException(const char* message) throw() : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ResourceException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ResourceException(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
