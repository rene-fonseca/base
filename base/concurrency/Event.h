/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__EVENT_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__EVENT_H

#include <base/Object.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/concurrency/LockException.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides support for threads to wait for signals.

  @short Event signaling mechanism
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Event : public virtual Object {
private:

  /** Internal data. */
  void* context;
public:
  
  /**
    Exception raised by the Event class.
    
    @short Event exception
    @ingroup concurrency exceptions
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class EventException : public LockException {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline EventException() throw() {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline EventException(const char* message) throw()
      : LockException(message) {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline EventException(Type type) throw() : LockException(type) {
    }
    
    /**
      Initializes the exception object.
      
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline EventException(const char* message, Type type) throw()
      : LockException(message, type) {
    }
  };

  /**
    Initializes the event in the non-signaled state.
  */
  explicit Event() throw(EventException);

  /**
    Returns true if this event is in the signaled state.
  */
  bool isSignaled() const throw(EventException);

  /**
    Resets the event. Returns when all waiting threads.
  */
  void reset() throw(EventException);

  /**
    Signal the waiting threads. Control is returned immediately.
  */
  void signal() throw(EventException);

  /**
    Waits for signal. The executing thread is suspended until event is
    signaled. Will wait forever if the event is never signaled. Raises an
    Overflow exception if the maximum number of waiting threads is exceeded.
  */
  void wait() const throw(EventException);

  /**
    Waits for the event to be signaled. The executing thread is suspended
    until the event is signaled or the specified time-out interval expires.
    May throw an 'Overflow' exception if the maximum number of waiting
    threads is exceeded.

    @param microseconds The desired time out interval in microseconds. The
    value must be in the range from 0 to 999999. The exception 'OutOfDomain'
    is raised if this range is violated.

    @return True, if the event was signaled before the time-out interval
    expired.
  */
  bool wait(unsigned int microseconds) const throw(OutOfDomain, EventException);

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  /** Returns the event handle. */
  inline OperatingSystem::Handle getHandle() const throw() { // TAG: fixme
    return Cast::pointer<OperatingSystem::Handle>(context);
  }
#endif

  /**
    Destroys the event object.
  */
  ~Event() throw(EventException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
