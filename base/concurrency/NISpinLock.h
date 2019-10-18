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
#include <base/concurrency/AtomicCounter.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.
  
  @ingroup concurrency
  @short Spin lock (non-inlined)
  @see MutualExclusion SpinLock
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API NISpinLock : public Lock {
private:
  
  /** Lock. */
  mutable PreferredAtomicCounter value;

  enum {
    LOCK_FREE = 0,
    LOCK_TAKEN = 1
  };
public:
  
  typedef ExclusiveSynchronize<NISpinLock> Sync;

  /**
    Initializes spin lock to unlocked state.
  */
  NISpinLock() noexcept;
  
  /**
    Acquires an exclusive lock.
  */
  void exclusiveLock() const noexcept;
  
  /**
    Tries to acquire an exclusive lock.
    
    @return True on success.
  */
  bool tryExclusiveLock() const noexcept;
  
  /**
    Acquires a shared lock. For some lock implementations this will acquire an
    exclusive lock.
  */
  inline void sharedLock() const noexcept {
    exclusiveLock();
  }
  
  /**
    Tries to acquire a shared lock. acquire an exclusive lock.

    @return True on success.
  */
  inline bool trySharedLock() const noexcept {
    return tryExclusiveLock();
  }
  
  /**
    Releases the spin lock.
  */
  void releaseLock() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
