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

#include <base/concurrency/Thread.h>
#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.
  
  @ingroup concurrency
  @short Recursive spin lock.
  @see MutualExclusion SpinLock
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API RecursiveSpinLock : public Lock {
private:

  typedef SpinLock Guard;
  typedef SpinLock Lock;
  /** Guard. */
  Guard guard;
  /** Lock. */
  Lock lock;
  /** The current owner. */
  mutable Thread::Identifier owner = 0;
  /** The number of locks held by the owner. */
  mutable unsigned int numberOfLocks = 0;
public:

  typedef ExclusiveSynchronize<RecursiveSpinLock> Sync;

  /**
    Initializes the spin lock in unlocked state.
  */
  RecursiveSpinLock() noexcept;
  
  /**
    Returns true if the executing thread is the owner of the lock.
  */
  inline bool isOwner() const noexcept
  {
    return Thread::getIdentifier() == owner;
  }

  /**
    Returns the number of locks currently held by the owner of the lock.
    Garbage is returned unless the executing thread is the owner.
  */
  inline unsigned int getNumberOfLocks() const noexcept
  {
    return numberOfLocks;
  }
  
  /**
    Acquires an exclusive lock.
  */
  void exclusiveLock() const noexcept;
  
  /**
    Tries to acquire an exclusive lock. Guaranteed to succeed if the executing
    thread already has ownership.
    
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
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
