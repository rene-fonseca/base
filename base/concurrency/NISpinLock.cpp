/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/NISpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void NISpinLock::exclusiveLock() const throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
  asm volatile (
    "1:      xchgl %1, %0\n\t" // try to acquire lock
    "        cmp %1, 0\n\t"
    "        je 3f\n"
    "2:      pause\n\t" // spin
    "        cmp %1, %0\n\t"
    "        jne 2b\n\t"
    "        jmp 1b\n"
    "3:\n"
    : "=m" (value) // output
    : "r" (1) // input
  );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
  asm volatile (
    "1:      xchgl %1, %0\n\t" // try to acquire lock
    "        cmp %1, 0\n\t"
    "        je 3f\n"
    "2:      pause\n\t" // spin // TAG: should pause be removed here
    "        cmp %1, %0\n\t"
    "        jne 2b\n\t"
    "        jmp 1b\n"
    "3:\n"
    : "=m" (value) // output
    : "r" (1) // input
  );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
  asm volatile (
    "        mov ar.ccv = r0 ;; \n\t" // free value
    "        mov r29 = 1 ;; \n" // locked value
    "1:      ld4 r2 = %0 ;; \n\t" // load lock value
    "        cmp.eq p0, p7 = r0, r2 ;; \n" // check if free
    "(p7)    br.cond.dpnt 1b ;; \n\t" // try until free
    "        cmpxchg4.acq r2 = %0, r29, ar.ccv ;; \n\t" // attempt to acquire the lock
    "        cmp.eq p0, p6 = r0, r2 ;; \n" // was lock acquired
    "(p6)    br.cond.dpnt 1b ;; \n" // try again
    : "=m" (value) // output
    : "0" (value) // input
    : "r2", "r29" // clobbered
  );
#else
  while (!tryExclusiveLock());
#endif // architecture
}

bool NISpinLock::tryExclusiveLock() const throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
  register unsigned int previous;
  asm volatile (
    "        xchgl %0, %1\n"
    : "=&r" (previous), "=m" (value)
    : "0" (1)
  );
  return (previous & 1) == 0;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
  register unsigned int previous;
  asm volatile (
    "        xchgl %0, %1\n"
    : "=&r" (previous), "=m" (value)
    : "0" (1)
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
    : "=&r" (previous), "=m" (value)
    : "0" (1)
  );
  return !previous;
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__MIPS) // MIPS II - R4000 processors
  register unsigned int success;
  asm volatile (
    "1:      ll %0, %1\n\t" // get state of lock: 0 (unlocked), or 1 (locked)
    "        xor %0, %0, 1\n\t" // flip bit
    "        beq %0, 0, 2f\n\t" // check if already locked
    "        nop\n\t"
    "        sc %0, %1\n\t" // try to lock
    "        beq %0, 0, 1b\n\t" // retry if we did not succeed
    "        nop\n"
    "2:\n"
    : "=&r" (success), "=m" (value) // output
  );
  return success;
#else
#  error Architecture is not supported
#endif // architecture
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
