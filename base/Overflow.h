/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OVERFLOW_H
#define _DK_SDU_MIP__BASE__OVERFLOW_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates that a resulting value is greater than the maximum
  positive representable value using a given format/type. An overflow condition
  will occur if the result of adding two 32 bit signed integers is greater than
  2^31-1.

  @see Underflow
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Overflow : public Exception {
public:
  
  /**
    Initializes the exception object with no message.
  */
  Overflow() throw();
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  Overflow(const char* message) throw();
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline Overflow(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline Overflow(const char* message, Type type) throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
