/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OUT_OF_RANGE_H
#define _DK_SDU_MIP__BASE__OUT_OF_RANGE_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  OutOfRange() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  OutOfRange(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
