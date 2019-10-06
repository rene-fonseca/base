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
  Exception raised by mutual exclusion synchronization objects.
  
  @short Mutual exclusion exception.
  @ingroup concurrency exception
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MutualExclusionException : public LockException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline MutualExclusionException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline MutualExclusionException(const char* message) throw()
    : LockException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline MutualExclusionException(Type type) throw() : LockException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline MutualExclusionException(const char* message, Type type) throw()
    : LockException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
