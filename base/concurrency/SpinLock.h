/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SPIN_LOCK_H
#define _DK_SDU_MIP__BASE_THREAD__SPIN_LOCK_H

/**
  Spin lock is a synchronization object just like MutualExclusion. SpinLock
  does not use any operating system resources. The SpinLock is fast. The CPU
  cannot do anything useful while waiting for a spin lock to be released.

  @see MutualExclusion
  @author René Møller Fonseca
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
    register int previous;
    asm volatile ("xchgl %0, %1" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;
#elif defined(sparc)
    register int previous;
    asm volatile ("swap %1, %0" : "=&r" (previous), "=m" (value) : "0" (1));
    return !previous;

//    register unsigned char _res = 1;
//    asm ("ldstub [%2], %0" : "=r"(_res), "=m" (value) : "r" (value));
//    return (int)_res;
/*
In CAS based updates, _RWSTD_MT_INCREMENT macro is defined to be a Fetch_and_Add inline assembler function:
.inline fetch_and_add,4
retry:
ld [%o0],%l0
add %l0,%o1,%l1
cas [%o0],%l0,%l1
cmp %l0,%l1
bne retry
mov %l1,%o0
nop
*/
#else
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
    value = 0;
  }

};

#endif
