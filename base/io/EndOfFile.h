/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_END_OF_FILE_H
#define _DK_SDU_MIP_BASE_IO_END_OF_FILE_H

#include "IOException.h"

/**
  Specifies that the end of stream has been reached unexpectedly during input.

  @author René Møller Fonseca
  @version 1.0
*/

class EndOfFile : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  EndOfFile();

  /**
    Initializes the exception object.

    @param message The message.
  */
  EndOfFile(const string& message);
};

#endif
