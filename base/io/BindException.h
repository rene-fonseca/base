/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H

#include <base/io/IOException.h>

  /**
    Thrown if unable to bind streams together.

    @author René Møller Fonseca
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
