/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ALREADY_KEY_EXCEPTION_H
#define _DK_SDU_MIP__BASE_COLLECTION__ALREADY_KEY_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised when a key is already in used by some collection
  class. This can happen when an object is registered multiple times or when
  different objects with identical values are registered.
  
  @short Already key exception.
  @ingroup collections exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AlreadyKeyException : public Exception {
public:
  
  /**
    Initializes the exception with no message.
  */
  inline AlreadyKeyException() throw() {
  }

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  inline AlreadyKeyException(const char* message) throw() : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline AlreadyKeyException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline AlreadyKeyException(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
