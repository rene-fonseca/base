/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__HOST_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_NET__HOST_NOT_FOUND_H

#include "NetworkException.h"

/**
  Specifies that the host doesn't exist or couldn't be found.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class HostNotFound : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  HostNotFound();

  /**
    Initializes the exception object.

    @param message The message.
  */
  HostNotFound(const char* message);
};

#endif
