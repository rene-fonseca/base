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

#include <base/concurrency/Lock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.
  
  @short Spin lock (fast).
  @ingroup concurrency
  @see MutualExclusion NISpinLock
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class SpinLock : public Lock {
private:
  
  volatile mutable unsigned int value;
  // TAG: reduce crosstalk through cache line
  
  /**
    Acquires the exclusive lock.
  */
  void acquireExclusiveLock() const throw();
public:
  
  /**
    Initializes spin lock to unlocked state.
  */
  inline SpinLock() throw() : value(0) {
  }
  
  /**
    Acquires an exclusive lock.
  */
  inline void exclusiveLock() const throw() {
    // TAG: use intrinsic
    if (!tryExclusiveLock()) {
      acquireExclusiveLock();
    }
  }
  
  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  inline bool tryExclusiveLock() const throw() {
    // TAG: use intrinsic
    return false;
    // #error Architecture is not supported
  }
  
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
  inline void releaseLock() const throw() {
    value = 0; // atomic operation
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
