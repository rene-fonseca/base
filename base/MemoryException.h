/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__MEMORY_EXCEPTION_H
#define _DK_SDU_MIP__BASE__MEMORY_EXCEPTION_H

#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on memory allocation/deallocation error.

  @author René Møller Fonseca
  @version 1.0
*/

class MemoryException : public ResourceException {
public:

  /**
    Initializes the exception object with no message.
  */
  MemoryException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  MemoryException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
