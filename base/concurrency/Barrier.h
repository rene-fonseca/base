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

#include <base/Object.h>
#include <base/concurrency/LockException.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of a barrier.
  
  @short Barrier synchronization object.
  @ingroup concurrency
  @see Event MutualExclusion Semaphore
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Barrier : public Object {
protected:
  
  typedef SpinLock Guard;
  /** Guard. */
  Guard guard;
  /** Event. */
  Event event;
  /** Reset event. */
  Event resetEvent;
  /** Mutual exclusion. */
  MutualExclusion mutex;
  /** The barrier count. */
  unsigned int count;
  /** The number of waiting threads. */
  unsigned int waiting;
public:
  
  /**
    Initializes the barrier with a count of 0. Raises ResourceException if
    unable to allocate the required resources.
  */
  Barrier() throw(ResourceException);
  
  /**
    Returns the barrier count.
  */
  unsigned int getCount() const throw();
  
  /**
    Sets the barrier count.
  */
  void setCount(unsigned int value) throw();
  
  /**
    Returns the number of waiting threads.
  */
  unsigned int getWaiting() const throw();
  
  /**
    The executing thread blocks until the barrier has been reached (i.e. the
    number of waiting threads equals the barrier count).
    
    @param reset Specifies whether or not to reset.
    
    @return The number of still waiting threads.
  */
  unsigned int wait(bool reset = false) throw(LockException);
  
  /**
    Resets the barrier. This method is used to wake up all waiting threads
    before the barrier count has been reached.
  */
  void reset() throw(LockException);
  
  /**
    Destroys the barrier.
  */
  ~Barrier();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
