/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H

#include "IOException.h"

  /**
    Specifies that the file doesn't exist or couldn't be opened.

    @author René Møller Fonseca
    @version 1.0
  */

  class FileNotFound : public IOException {
  public:

    /**
      Initializes the exception object with no message.
    */
    FileNotFound();

    /**
      Initializes the exception object.

      @param message The message.
    */
    FileNotFound(const char* message);
  };

#endif
