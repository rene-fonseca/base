/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__AMBIGUOUS_REGISTRATION_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__AMBIGUOUS_REGISTRATION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised by ExpressionParser on registration of identifiers
  that result in an ambiguity.

  @short Ambiguous registration exception.
  @ingroup exceptions mathematics
  @see ExpressionParser
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AmbiguousRegistration : public Exception {
public:

  /**
    Initializes the exception with no message.
  */
  AmbiguousRegistration() throw();

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  AmbiguousRegistration(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline AmbiguousRegistration(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline AmbiguousRegistration(const char* message, Type type) throw() : Exception(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
