/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OVERFLOW_H
#define _DK_SDU_MIP__BASE__OVERFLOW_H

#include "Exception.h"

/**
  Thrown on value greater than maximum valid value.

  @author Ren� M�ller Fonseca
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
};

#endif
