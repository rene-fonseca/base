/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_CONSTRUCT_H
#define _DK_SDU_MIP_BASE_CONSTRUCT_H

#include "Exception.h"

/**
  Thrown on object construction error.

  @author René Møller Fonseca
  @version 1.0
*/

class Construct : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Construct() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Construct(const char* message) throw();
};

#endif
