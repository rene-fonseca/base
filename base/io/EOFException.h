/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_IO_EOF_EXCEPTION_H
#define _DK_SDU_MIP_BASE_IO_EOF_EXCEPTION_H

#include "base/Exception.h"

/**
  Thrown on attempt to read stream beyond end of stream.

  @author Ren� M�ller Fonseca
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
