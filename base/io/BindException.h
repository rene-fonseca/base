/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__BIND_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown if unable to bind streams together.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class BindException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  BindException();

  /**
    Initializes the exception object.

    @param message The message.
  */
  BindException(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
