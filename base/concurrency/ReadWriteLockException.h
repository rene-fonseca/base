/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_LOCK_EXCEPTION_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_LOCK_EXCEPTION_H

#include <base/concurrency/LockException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception raised by ReadWriteLock synchronization objects.
  
  @short Read write lock exception.
  @ingroup concurrency exception
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ReadWriteLockException : public LockException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline ReadWriteLockException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ReadWriteLockException(const char* message) throw()
    : LockException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ReadWriteLockException(Type type) throw() : LockException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ReadWriteLockException(const char* message, Type type) throw()
    : LockException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
