/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__MULTIBYTE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_STRING__MULTIBYTE_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised on convertion errors from or to multibyte characters.

  @short Multibyte character conversion exception.
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MultibyteException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  MultibyteException() throw() : Exception() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  MultibyteException(const char* message) throw() : Exception(message) {}

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  MultibyteException(Type type) throw() : Exception(type) {}
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  MultibyteException(const char* message, Type type) throw() : Exception(message, type) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
