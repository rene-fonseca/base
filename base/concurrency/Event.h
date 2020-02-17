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
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/ResourceException.h>
#include <base/OperatingSystem.h>
#include <base/concurrency/EventException.h>
#include <base/Resource.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class String;

/**
  This class provides support for threads to wait for signals.

  @short Event signaling mechanism.
  @ingroup concurrency
  @version 1.3
*/

class _COM_AZURE_DEV__BASE__API Event : public Resource {
public:

  /**
    Initializes the event in the non-signaled state.
  */
  explicit Event();

  /**
    Initializes the event as invalid.
  */
  explicit inline Event(NullPtr) noexcept
  {
  }

  /**
    Returns true if this event is in the signaled state.
  */
  bool isSignaled() const;

  /**
    Resets the event. Returns when all waiting threads.
  */
  void reset();

  /**
    Signal the waiting threads. Control is returned immediately.
  */
  void signal();

  /**
    Waits for signal. The executing thread is suspended until event is
    signaled. Will wait forever if the event is never signaled. Raises an
    Overflow exception if the maximum number of waiting threads is exceeded.
  */
  void wait() const;

  /**
    Waits for the event to be signaled. The executing thread is suspended
    until the event is signaled or the specified time-out interval expires.
    May throw an 'Overflow' exception if the maximum number of waiting
    threads is exceeded.

    @param microseconds The desired time out interval in microseconds. The
    value must be in the range from 0 to 1000000000. The exception 'OutOfDomain'
    is raised if this range is violated.

    @return True, if the event was signaled before the time-out interval
    expired.
  */
  bool wait(unsigned int microseconds) const;

  /** Returns the event handle. Not supported by all platforms in which case nullptr is returned. */
  OperatingSystem::Handle getHandle() const noexcept;
  
  /**
    Destroys the event object.
  */
  ~Event();
};

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Event)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
