/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__INVALID_FORMAT_H
#define _DK_SDU_MIP__BASE__INVALID_FORMAT_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Invalid format exception is thrown when a string does not comply with a given format.

  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InvalidFormat : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidFormat() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidFormat(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  InvalidFormat(Type type) throw() : Exception(type) {}
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  InvalidFormat(const char* message, Type type) throw() : Exception(message, type) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
