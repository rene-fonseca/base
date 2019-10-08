/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/concurrency/SemaphoreException.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/Primitives.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Semaphore synchronization object. Semaphores are useful when resources need
  to be synchronized between multiple threads.
  
  @short Semaphore synchronization object
  @ingroup concurrency
  @version 1.1
*/

class _DK_SDU_MIP__BASE__API Semaphore : public virtual Object {
private:

  /** Internal semaphore state. */
  void* semaphore = nullptr;
public:
  
  /**
    Returns the maximum value the semaphore may have. The maximum value is
    guaranteed to be greater than or equal to 32767 and less then or equal to
    Primitive<int>::MAXIMUM.
  */
  static unsigned int getMaximum() throw();
  
  /**
    Initializes the semaphore object. Raises OutOfDomain if the value exceeds
    the maximum value.

    @param value The initial value. The default is 0.
  */
  Semaphore(unsigned int value = 0) throw(OutOfDomain, SemaphoreException);

  /**
    Returns the current value of the semaphore. Warning: this method may not be
    supported on the particular platform.

    @return -1 if not supported.
  */
  int getValue() const throw(SemaphoreException);

  /**
    Increments the semaphore and signals any thread waiting for a change. This
    method never blocks.
  */
  void post() throw(Overflow, SemaphoreException);

  /**
    Decrements the semaphore and blocks if the semaphore is less than zero
    until another thread signals a change.
  */
  void wait() const throw(SemaphoreException);

  /**
    Non-blocking variant of wait.

    @return True if the semaphore was decremented.
  */
  bool tryWait() const throw(SemaphoreException);

  /**
    Destroys the semaphore object.
  */
  ~Semaphore();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
