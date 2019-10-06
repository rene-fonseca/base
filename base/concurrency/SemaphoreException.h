/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/LockException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception raised by Semaphore synchronization objects.
  
  @short Semaphore exception.
  @ingroup concurrency exception
  @see Semaphore
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SemaphoreException : public LockException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline SemaphoreException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline SemaphoreException(const char* message) throw()
    : LockException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SemaphoreException(Type type) throw() : LockException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SemaphoreException(const char* message, Type type) throw()
    : LockException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
