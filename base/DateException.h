/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DATE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__DATE_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown by the Date class.

  @author Ren� M�ller Fonseca
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
