/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

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
  This exception is thrown by ExpressionParser on registration of identifiers
  that result in an ambiguity.

  @see ExpressionParser
  @author René Møller Fonseca
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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
