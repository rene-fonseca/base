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
#include <base/concurrency/Thread.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Thread;

/**
  Base class of active objects. An active object can be run by a thread object.

  @short Base class of active objects.
  @ingroup concurrency
  @see Thread
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Runnable : public virtual Object {
protected:

  /** Specifies that the active object should be terminated. */
  bool terminated = false;
public:
  
  /**
    Initializes runnable object.
  */
  inline Runnable() noexcept
  {
  }
  
  /**
    Entry point for the thread.
  */
  virtual void run() = 0;
  
  /**
    Returns true if the active object should be terminated.
  */
  inline bool isTerminated() noexcept
  {
    return terminated;
  }
  
  /**
    Invocated when a child thread is completed. Watch out for MT-safety.

    @param child The child thread.
  */
  virtual void onChild(Thread* child) noexcept
  {
  }
  
  /**
    Invocated when the thread is asked to terminate. Watch out for MT-safety.
  */
  virtual void onTermination() noexcept
  {
    terminated = true;
  }
  
  /**
    Destroys the runnable object.
  */
  virtual ~Runnable() {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
