/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__IO_EXCEPTION_H
#define _DK_SDU_MIP__BASE__IO_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on IO error.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class IOException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  IOException();

  /**
    Initializes the exception object.

    @param message The message.
  */
  IOException(const char* message);
};

#endif
