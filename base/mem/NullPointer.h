/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__NULL_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__NULL_POINTER_H

#include <base/MemoryException.h>

/**
  Thrown if NULL pointer was unexprected.

  @author René Møller Fonseca
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

#endif
