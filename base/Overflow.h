/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_OVERFLOW_H
#define _DK_SDU_MIP_BASE_OVERFLOW_H

#include "Exception.h"

/**
  Thrown on value greater than maximum valid value.

  @author René Møller Fonseca
  @version 1.0
*/

class Overflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Overflow();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Overflow(const string& message);

  /**
    Destroys exception object.
  */
  ~Overflow();
};

#endif
