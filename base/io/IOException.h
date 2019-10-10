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
  @defgroup io IO
*/

/**
  Exception raised on IO errors.

  @short IO exception.
  @ingroup exceptions io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IOException : public Exception {
public:

  /** Cause. */
  enum Cause {
    ABORTED, /**< Operation aborted. */
    BROKEN_STREAM, /**< Broken stream. */
    PENDING /**< I/O is pending. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  inline IOException() throw() {
  }

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  inline IOException(const char* message) throw() : Exception(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline IOException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline IOException(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
