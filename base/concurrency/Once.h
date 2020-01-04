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

#include <base/AutomationObject.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Once implementation with delayed resource allocation.
  
  @code
  void myMethod() noexcept
  {
    static Once once;
    if (once.getToken()) {
      // ...
      once.relinquishToken();
    }
  }
  @endcode
  
  @short Once synchronization object.
  @ingroup concurrency
  @see Event MutualExclusion Semaphore
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Once : public AutomationObject {
private:
  
  typedef SpinLock Guard;
  /** Guard. */
  Guard guard;
  /** Event. */
  Event* event = nullptr;
  /** The number of waiting threads. */
  unsigned int waiting = 0;
  /** Specifies, when false, that the token has been relinquished. */
  bool mask = true;
  
  /**
    Internal implementation for the getToken() method.
  */
  bool getTokenImpl();
public:
  
  /**
    Initializes the synchronization object.
  */
  Once() noexcept;
  
  /**
    This method returns true for one thread only and only on the first
    invocation. All other threads are blocked until relinquishToken() is
    invoked by the thread for which getToken() returned true. Raises
    ResourceException if unable to allocate the required resources.
  */
  inline bool getToken()
  {
    return mask && getTokenImpl();
  }
  
  /**
    Relinquishes the token.
  */
  void relinquishToken() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
