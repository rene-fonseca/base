/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__NOT_IMPLEMENTED_H
#define _DK_SDU_MIP__BASE__NOT_IMPLEMENTED_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception used to specify that some context has not been fully implemented.
  This exception is an internal error.
  
  @short Not implemented exception.
  @see NotSupported
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class NotImplemented : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  inline NotImplemented() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline NotImplemented(const char* message) throw() : Exception(this) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline NotImplemented(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline NotImplemented(const char* message, Type type) throw()
    : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
