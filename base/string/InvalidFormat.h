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
  Invalid format exception is raised when a string does not comply with a given
  format.

  @short Invalid formation exception.
  @ingroup exceptions
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API InvalidFormat : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline InvalidFormat() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline InvalidFormat(const char* message) throw() : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline InvalidFormat(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline InvalidFormat(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
