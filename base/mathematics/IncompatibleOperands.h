/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__INCOMPATIBLE_OPERANDS_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__INCOMPATIBLE_OPERANDS_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised when specified operands are incompatible with each
  other.

  @ingroup exceptions mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IncompatibleOperands : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  IncompatibleOperands();

  /**
    Initializes the exception object.

    @param message The message.
  */
  IncompatibleOperands(const char* message);
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline IncompatibleOperands(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline IncompatibleOperands(const char* message, Type type) throw() : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
