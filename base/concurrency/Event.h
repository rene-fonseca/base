/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__EVENT_H
#define _DK_SDU_MIP__BASE_THREAD__EVENT_H

#include <base/Object.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/ResourceException.h>
#include <base/OperatingSystem.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <pthread.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Event.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

class Event : public virtual Object {
private:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  /** Event handle. */
  OperatingSystem::Handle event;
#else
  /** Indicates that the event has been signaled. */
  bool signaled;
  /** Conditional. */
  mutable pthread_cond_t condition;
  /** Internal mutex representation. */
  mutable pthread_mutex_t mutex;
#endif
public:

  /**
    Exception thrown by the Event class.
  */
  class EventException : public Exception {
  public:
    EventException(const char* message) throw() : Exception(message) {}
  };

  /**
    Initializes the event.
  */
  explicit Event() throw(ResourceException);

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
    signaled. Will wait forever if the event is never signaled. Throws an
    'Overflow' exception if the maximum number of waiting threads is exceeded.
  */
  void wait() const throw(EventException);

  /**
    Waits for the event to be signaled. The executing thread is suspended
    until the event is signaled or the specified time-out interval expires.
    May throw an 'Overflow' exception if the maximum number of waiting
    threads is exceeded.

    @param microseconds The desired time out interval in microseconds. The
    value must be in the range from 0 to 999999. The exception 'OutOfDomain'
    is thrown if this range is violated.

    @return True, if the event was signaled before the time-out interval
    expired.
  */
  bool wait(unsigned int microseconds) const throw(OutOfDomain, EventException);

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  /** Returns the event handle. */
  inline OperatingSystem::Handle getHandle() const throw() {return event;}
#endif

  /**
    Destroys the event object.
  */
  ~Event() throw(EventException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
