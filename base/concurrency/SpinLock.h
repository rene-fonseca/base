/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SPIN_LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SPIN_LOCK_H

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
#if 1
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
    register unsigned int previous;
    asm (
      "        xchgl %0, %1\n"
      : "=&r" (previous), "=m" (value) // output (value is clobbered)
      : "0" (1) // input
    ); // TAG: get redundant instruction (mov to stack)
    if (previous) {
      acquireExclusiveLock();
    }
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
    // TAG: use intrinsic
    register unsigned int previous;
    asm (
      "        xchgl %0, %1\n"
      : "=&r" (previous), "=m" (value) // output (value is clobbered)
      : "0" (1) // input
    );
    if (previous) {
      acquireExclusiveLock();
    }
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "        mov ar.ccv = r0; \n\t" // free value
      "        mov r29 = 1 ;; \n" // locked value
      "1:      ld4 r2 = %0 ;; \n\t" // load lock value
      "        cmp.eq p0, p7 = r0, r2 \n" // check if free
      "(p7)    br.cond.dpnt 1b ;; \n\t" // try until free
      "        cmpxchg4.acq r2 = %0, r29, ar.ccv ;; \n\t" // attempt to acquire the lock
      "        cmp.eq p0, p6 = r0, r2 \n" // was lock acquired
      "(p6)    br.cond.dpnt 1b ;; \n" // try again
      : "=m" (value) // output
      : "0" (value) // input
      : "r2", "r29" // clobbered
    );
#else
    if (!tryExclusiveLock()) {
      acquireExclusiveLock();
    }
#endif // architecture
  }
  
  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  inline bool tryExclusiveLock() const throw() {
#if 1
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
    register unsigned int previous;
    asm volatile (
      "        xchgl %0, %1\n"
      : "=&r" (previous), "=m" (value) // output
      : "0" (1) // input
    );
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
    unsigned int previous;
    asm volatile (
      "        xchgl %0, %1\n"
      : "=&r" (previous), "=m" (value) // output
      : "0" (1) // input
    );
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    unsigned int success;
    asm volatile (
      "        mov ar.ccv = r0 \n\t" // released lock value
      "        mov r29 = 1 ;; \n\t" // acquired lock value
      "        ld4 r3 = %1 \n\t" // preload
      "        cmpxchg4.acq r2 = %1, r29, ar.ccv ;; \n\t"
      "        cmp.eq p6, p7 = r2, r0 ;; \n" // if ne, lock acq failed
      "(p6)    mov %0 = r29 \n" // acquire succeed
      "(p7)    mov %0 = r0 \n" // acquire failed
      : "=r" (success), "=m" (value) // output
      : "1" (value) // input
      : "r2", "r3", "r29" // clobbered
    );
    return success;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__ALPHA)
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__SPARC)
    unsigned int previous;
    asm volatile (
      "        swap %1, %0\n"
      : "=&r" (previous), "=m" (value) // output
      : "0" (1) // input
    );
    return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__MIPS) // MIPS II - R4000 processors
    register unsigned int success;
    asm volatile (
      "1:      ll %0, %1\n\t" // get state of lock: 0 (unlocked), or 1 (locked)
      "        xor %0, %0, 1\n\t" // flip bit
      "        beq %0, $0, 2f\n\t" // check if already locked
      "        nop\n\t"
      "        sc %0, %1\n\t" // try to lock
      "        beq %0, $0, 1b\n\t" // retry if we did not succeed
      "        nop\n"
      "2:\n"
      : "=&r" (success), "=m" (value) // output
    );
    return success;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__S390)
    unsigned int success;
    asm volatile (
      "        sr 0,0\n" // subtract
      "        la 1,1\n" // load address
      "        cs 0,1,%1\n" // compare and swap
      "        ipm %0\n"
      "        srl %0,28\n" // shift right single logical
      : "=&d" (success), "+m" (value) // output
      :
      : "cc", "0", "1" // clobbered
    );
    return success == 0;
#else
#  error Architecture is not supported
#endif // architecture
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

#endif
