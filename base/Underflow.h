/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_UNDERFLOW_H
#define _DK_SDU_MIP_BASE_UNDERFLOW_H

#include "Exception.h"

/**
  Thrown on value less than minimum valid value.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Underflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Underflow();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Underflow(const string& message);

  /**
    Destroys exception object.
  */
  ~Underflow();
};

#endif
