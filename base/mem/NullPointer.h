/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__NULL_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__NULL_POINTER_H

#include <base/MemoryException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown if NULL pointer was unexprected.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class NullPointer : public MemoryException {
public:

  /**
    Initializes the exception object with no message.
  */
  NullPointer();

  /**
    Initializes the exception object.

    @param message The message.
  */
  NullPointer(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
