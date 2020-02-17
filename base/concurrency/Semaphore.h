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

#include <base/concurrency/SemaphoreException.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/Primitives.h>
#include <base/OperatingSystem.h>
#include <base/Resource.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Semaphore synchronization object. Semaphores are useful when resources need
  to be synchronized between multiple threads.
  
  @short Semaphore synchronization object
  @ingroup concurrency
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Semaphore : public Resource {
public:
  
  /**
    Returns the maximum value the semaphore may have. The maximum value is
    guaranteed to be greater than or equal to 32767 and less then or equal to
    Primitive<int>::MAXIMUM.
  */
  static unsigned int getMaximum() noexcept;
  
  /**
    Initializes the semaphore object. Raises OutOfDomain if the value exceeds
    the maximum value.

    @param value The initial value. The default is 0.
  */
  Semaphore(unsigned int value = 0);

  /**
    Returns the current value of the semaphore. Warning: this method may not be
    supported on the particular platform.

    @return -1 if not supported.
  */
  int getValue() const;

  /**
    Increments the semaphore and signals any thread waiting for a change. This
    method never blocks.
  */
  void post();

  /**
    Decrements the semaphore and blocks if the semaphore is less than zero
    until another thread signals a change.
  */
  void wait() const;

  /**
    Non-blocking variant of wait.

    @return True if the semaphore was decremented.
  */
  bool tryWait() const;

  /**
    Destroys the semaphore object.
  */
  ~Semaphore();
};

// _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Semaphore)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
