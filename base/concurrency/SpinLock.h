/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  inline SpinLock() throw() : value(0) {
  }

  /**
    Acquires an exclusive lock.
  */
  inline void exclusiveLock() const throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "mov ar.ccv = r0; \n\t" // free value
      "mov r29 = 1 ;; \n" // locked value
      "1: \n\t"
      "ld4 r2 = %0 ;; \n\t" // load lock value
      "cmp.eq p0, p7 = r0, r2 \n\t" // check if free
      "(p7) br.cond.dpnt 1b ;; \n\t" // try until free
      "cmpxchg4.acq r2 = %0, r29, ar.ccv ;; \n\t" // attempt to acquire the lock
      "cmp.eq p0, p6 = r0, r2 \n\t" // was lock acquired
      "(p6) br.cond.dpnt 1b ;; \n\t" // try again
      : "=m" (value) // output
      : "0" (value) // input
      : "r2", "r29" // clobbered
    );
#else
    while (!tryExclusiveLock());
#endif // architecture
  }

  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  inline bool tryExclusiveLock() const throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
    unsigned int previous;
    asm volatile ("xchgl %0, %1" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
    unsigned int previous;
    asm volatile ("xchgl %0, %1" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    bool success;
    asm volatile (
      "mov ar.ccv = r0 \n\t" // released lock value
      "mov r29 = 1 ;; \n\t" // acquired lock value
      "ld4 r3 = %1 \n\t" // preload
      "cmpxchg4.acq r2 = %1, r29, ar.ccv ;; \n\t"
      "cmp.eq p6, p7 = r2, r0 ;; \n\t" // if ne, lock acq failed
      "(p6) mov %0 = r29 \n\t" // acquire succeed
      "(p7) mov %0 = r0 \n\t" // acquire failed
      : "=r" (success), "=m" (value) // output
      : "1" (value) // input
      : "r2", "r3", "r29" // clobbered
    );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__ALPHA)
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__SPARC)
    unsigned int previous;
    asm volatile ("swap %1, %0" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__MIPS) // MIPS II - R4000 processors
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
#else
  #error Architecture is not supported
#endif // architecture
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
