/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_OUT_OF_DOMAIN_H
#define _DK_SDU_MIP_BASE_OUT_OF_DOMAIN_H

#include "Exception.h"

/**
  Exception used to specify that a variable has been assigned a value not in the valid domain.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class OutOfDomain : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OutOfDomain();

  /**
    Initializes the exception object.

    @param message The message.
  */
  OutOfDomain(const string& message);

  /**
    Destroys exception object.
  */
  ~OutOfDomain();
};

#endif
