/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__IO_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__IO_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on IO errors.

  @author Ren� M�ller Fonseca
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
