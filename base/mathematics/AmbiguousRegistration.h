/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
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
