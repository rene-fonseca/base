/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__MEMORY_EXCEPTION_H
#define _DK_SDU_MIP__BASE__MEMORY_EXCEPTION_H

#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Raised on memory allocation, deallocation, and access error.

  @short Memory exception
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  MemoryException(Type type) throw() : ResourceException(type) {}
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  MemoryException(const char* message, Type type) throw() : ResourceException(message, type) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
