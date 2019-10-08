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
  Exception raised by the Barrier class.
  
  @short Barrier synchronization object exception.
  @ingroup concurrency exceptions
  @see Barrier
  @version 1.0
*/
class _DK_SDU_MIP__BASE__API BarrierException : public LockException {
public:
  
  /**
    Initializes the exception object with no message.
  */
  inline BarrierException() throw() {
  }
  
  /**
    Initializes the exception object.
    
    @param message The message.
  */
  inline BarrierException(const char* message) throw()
    : LockException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline BarrierException(Type type) throw()
    : LockException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline BarrierException(const char* message, Type type) throw()
    : LockException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
