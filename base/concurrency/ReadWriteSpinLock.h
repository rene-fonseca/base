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
  This class implements a read-write lock for optimal reader performance on
  systems. Please note that the lock/unlock mechanism is considered a
  non-modifying property of a class.
  
  @short Read-write synchronization object.
  @ingroup concurrency
  @see MutualExclusion ReadWriteLock SpinLock
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ReadWriteSpinLock {
private:
  
  typedef SpinLock Guard;
  typedef SpinLock Lock;
  /** Guard. */
  Guard guard;
  /** Lock. */
  Lock lock;
  /** The owner holding the exclusive lock or the scheduled writer. */
  mutable Thread::Identifier writer = 0;
  /** The number of readers currently holding the lock. */
  mutable unsigned int numberOfReaders = 0;
public:
  
  /**
    Initializes the spin lock in unlocked state.
  */
  ReadWriteSpinLock() noexcept;
  
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
    Acquires a shared lock.
  */
  inline void sharedLock() const noexcept;
  
  /**
    Tries to acquire a shared lock.
    
    @return True on success.
  */
  bool trySharedLock() const noexcept;
  
  /**
    Releases the spin lock.
  */
  void releaseLock() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
