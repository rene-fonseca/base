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

#include <base/concurrency/Lock.h>
#include <atomic>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.
  
  @short Spin lock (fast).
  @ingroup concurrency
  @see MutualExclusion NISpinLock
  @version 1.2
*/

class _DK_SDU_MIP__BASE__API SpinLock : public Lock {
private:

  mutable std::atomic<unsigned long> value;
public:
  
  /**
    Initializes spin lock to unlocked state.
  */
  SpinLock() throw();
  
  /**
    Acquires an exclusive lock.
  */
  void exclusiveLock() const throw();
  
  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  bool tryExclusiveLock() const throw();
  
  /**
    Acquires a shared lock. For some lock implementations this will acquire an
    exclusive lock.
  */
  inline void sharedLock() const throw() {
    exclusiveLock();
  }
  
  /**
    Tries to acquire a shared lock. acquire an exclusive lock.

    @return True on success.
  */
  inline bool trySharedLock() const throw() {
    return tryExclusiveLock();
  }
  
  /**
    Releases the spin lock.
  */
  void releaseLock() const throw();

  inline ~SpinLock() {
    releaseLock();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
