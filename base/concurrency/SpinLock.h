/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SPIN_LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SPIN_LOCK_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.

  @ingroup concurrency
  @see MutualExclusion
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SpinLock {
private:

  volatile mutable unsigned int value;
public:

  /**
    Initializes spin lock to unlocked state.
  */
  inline SpinLock() throw() : value(0) {}

  /**
    Acquires an exclusive lock.
  */
  inline void exclusiveLock() const throw() {
    while (!tryExclusiveLock());
  }

  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  inline bool tryExclusiveLock() const throw() {
#if defined(i386)
    register unsigned int previous;
    asm volatile ("xchgl %0, %1" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif defined(sparc)
    register unsigned int previous;
    asm volatile ("swap %1, %0" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif defined(mips) // MIPS II - R4000 processors
    register unsigned int success;
    asm volatile (
      "1:"
      "ll %0, %1;" // get state of lock: 0 (unlocked), or 1 (locked)
      "xor %0, %0, 1;" // flip bit
      "beq %0, 0, 2f;" // check if already locked
      "nop;"
      "sc %0, %1;" // try to lock
      "beq %0, 0, 1b;" // retry if we did not succeed
      "nop;"
      "2:"
      : "=&r" (success), "=m" (value) // outputs
    );
    return success;
#endif
  }

  /**
    Acquires a shared lock. For some lock implementations this will acquire an exclusive lock.
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

#endif
