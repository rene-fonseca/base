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
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/ResourceException.h>
#include <base/OperatingSystem.h>
#include <base/concurrency/EventException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides support for threads to wait for signals.

  @short Event signaling mechanism.
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Event : public virtual Object {
private:

  /** Internal data. */
  void* context = nullptr;
public:

  /**
    Initializes the event in the non-signaled state.
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
  ~Event();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
