/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__TIMED_OUT_H
#define _DK_SDU_MIP__BASE_IO__TIMED_OUT_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on timed out IO requests.

  @author René Møller Fonseca
  @version 1.0
*/

class TimedOut : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  TimedOut() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  TimedOut(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
