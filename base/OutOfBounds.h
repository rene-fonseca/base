/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OUT_OF_BOUNDS_H
#define _DK_SDU_MIP__BASE__OUT_OF_BOUNDS_H

#include "Exception.h"

/**
  Out of bounds exception.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfBounds : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OutOfBounds() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  OutOfBounds(const char* message) throw();
};

#endif
