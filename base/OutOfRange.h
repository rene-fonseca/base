/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_OUT_OF_RANGE_H
#define _DK_SDU_MIP_BASE_OUT_OF_RANGE_H

#include "Exception.h"

/**
  Exception used to specify that an index is out of range.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfRange : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OutOfRange();

  /**
    Initializes the exception object.

    @param message The message.
  */
  OutOfRange(const string& message);

  /**
    Destroys exception object.
  */
  ~OutOfRange();
};

#endif
