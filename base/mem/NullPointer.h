/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  Raised if invalid (NULL) pointer was unexprected/prohibited.

  @short Invalid pointer exception
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline NullPointer(Type type) throw() : MemoryException(type) {
  }

  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline NullPointer(const char* message, Type type) throw()
    : MemoryException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
