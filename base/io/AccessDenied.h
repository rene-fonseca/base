/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__ACCESS_DENIED_H
#define _DK_SDU_MIP__BASE_IO__ACCESS_DENIED_H

#include <base/io/IOException.h>

  /**
    Exception thrown errors due to denied access.

    @author Ren� M�ller Fonseca
    @version 1.0
  */

  class AccessDenied : public IOException {
  public:

    /**
      Initializes the exception object with no message.
    */
    AccessDenied() throw();

    /**
      Initializes exception object as unspecified.

      @param message The message.
    */
    AccessDenied(const char* message) throw();
  };

#endif
