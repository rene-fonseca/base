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
  
  @short Spin lock (fast).
  @ingroup concurrency
  @see MutualExclusion NISpinLock
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API SpinLock : public Lock {
private:

  mutable PreferredAtomicCounter value;

  enum {
    LOCK_FREE = 0,
    LOCK_TAKEN = 1
  };
public:
  
  typedef ExclusiveSynchronize<SpinLock> Sync;

  // TAG: add sleep / yield option
  // TAG: add support for counter before sleep
  // uint64 sleep = 0;

  /**
    Initializes spin lock to unlocked state.
  */
  SpinLock() noexcept;

  /**
    Returns true if state is valid.
  */
  bool invariant() const noexcept;

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
  inline void sharedLock() const noexcept
  {
    exclusiveLock();
  }
  
  /**
    Tries to acquire a shared lock. acquire an exclusive lock.

    @return True on success.
  */
  inline bool trySharedLock() const noexcept
  {
    return tryExclusiveLock();
  }
  
  /**
    Releases the spin lock.
  */
  void releaseLock() const noexcept;

#if 0 // AtomicCounter resets to bad value
  inline ~SpinLock()
  {
    releaseLock();
  }
#endif
};

// _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(SpinLock)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
