/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_EOF_EXCEPTION_H
#define _DK_SDU_MIP_BASE_IO_EOF_EXCEPTION_H

#include "base/Exception.h"

/**
  Thrown on attempt to read stream beyond end of stream.

  @author René Møller Fonseca
  @version 1.0
*/

class EOFException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  EOFException();

  /**
    Initializes the exception object.

    @param message The message.
  */
  EOFException(const string& message);

  /**
    Destroys exception object.
  */
  ~EOFException();
};

#endif
