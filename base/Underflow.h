/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__UNDERFLOW_H
#define _DK_SDU_MIP__BASE__UNDERFLOW_H

#include "Exception.h"

/**
  Thrown on value less than minimum valid value.

  @author René Møller Fonseca
  @version 1.0
*/

class Underflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Underflow() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Underflow(const char* message) throw();
};

#endif
