/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__INVALID_KEY_H
#define _DK_SDU_MIP__BASE_COLLECTION__INVALID_KEY_H

#include <base/Exception.h>

/**
  This class fundamental exception.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class InvalidKey : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidKey();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidKey(const char* message);
};

#endif
