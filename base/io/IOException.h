/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__IO_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__IO_EXCEPTION_H

#include <base/Exception.h>

  /**
    Exception thrown on IO errors.

    @author René Møller Fonseca
    @version 1.0
  */

  class IOException : public Exception {
  public:

    /**
      Initializes the exception object with no message.
    */
  IOException() throw();

    /**
      Initializes exception object as unspecified.

      @param message The message.
    */
    IOException(const char* message) throw();
  };

#endif
