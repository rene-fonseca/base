/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H

#include <base/io/IOException.h>

  /**
    Thrown if unable to bind streams together.

    @author Ren� M�ller Fonseca
    @version 1.0
  */

  class BindException : public IOException {
  public:

    /**
      Initializes the exception object with no message.
    */
    BindException();

    /**
      Initializes the exception object.

      @param message The message.
    */
    BindException(const char* message);
  };

#endif
