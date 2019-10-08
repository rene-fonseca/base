/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/Lock.h>
#include <stdatomic.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.
  
  @ingroup concurrency
  @short Spin lock (non-inlined)
  @see MutualExclusion SpinLock
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API NISpinLock : public Lock {
private:
  
  /** Lock. */
  _Atomic volatile mutable unsigned long value = 0;
public:
  
  /**
    Initializes spin lock to unlocked state.
  */
  NISpinLock() throw();
  
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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
