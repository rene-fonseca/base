/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DATE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__DATE_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown by the Date class.

  @author René Møller Fonseca
  @version 1.0
*/

class DateException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  DateException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  DateException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
