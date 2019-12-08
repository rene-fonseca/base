/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Implementation of a barrier.
  
  @short Barrier synchronization object.
  @ingroup concurrency
  @see Event MutualExclusion Semaphore
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Barrier : public Object {
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
  unsigned int count = 0;
  /** The number of waiting threads. */
  unsigned int waiting = 0;
public:
  
  /**
    Initializes the barrier with a count of 0. Raises ResourceException if
    unable to allocate the required resources.
  */
  Barrier();
  
  /**
    Returns the barrier count.
  */
  unsigned int getCount() const;
  
  /**
    Sets the barrier count.
  */
  void setCount(unsigned int value);
  
  /**
    Returns the number of waiting threads.
  */
  unsigned int getWaiting() const;
  
  /**
    The executing thread blocks until the barrier has been reached (i.e. the
    number of waiting threads equals the barrier count).
    
    @param reset Specifies whether or not to reset.
    
    @return The number of still waiting threads.
  */
  unsigned int wait(bool reset = false);
  
  /**
    Resets the barrier. This method is used to wake up all waiting threads
    before the barrier count has been reached.
  */
  void reset();
  
  /**
    Destroys the barrier.
  */
  ~Barrier();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
