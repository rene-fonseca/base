/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__MEMORY_CORRUPTION_H
#define _DK_SDU_MIP__BASE__MEMORY_CORRUPTION_H

#include <base/MemoryException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on memory corruption.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MemoryCorruption : public MemoryException {
public:

  /**
    Initializes the exception object with no message.
  */
  MemoryCorruption() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  MemoryCorruption(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
