/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__INCOMPATIBLE_OPERANDS_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__INCOMPATIBLE_OPERANDS_H

#include "base/Exception.h"

/**
  This exception is thrown when specified operands are incompatible with each
  other.

  @author René Møller Fonseca
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
};

#endif
