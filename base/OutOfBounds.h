/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_OUT_OF_BOUNDS_H
#define _DK_SDU_MIP_BASE_OUT_OF_BOUNDS_H

#include "Exception.h"

/**
  Out of bounds exception.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class OutOfBounds : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OutOfBounds();

  /**
    Initializes the exception object.

    @param message The message.
  */
  OutOfBounds(const string& message);

  /**
    Destroys exception object.
  */
  ~OutOfBounds();
};

#endif
