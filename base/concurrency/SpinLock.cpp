/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void SpinLock::acquireExclusiveLock() const throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
    asm volatile (
      "1:      pause\n\t" // spin
      "        cmpl %1, %0\n\t"
      "        je 1b\n\t"
      "        xchgl %1, %0\n\t" // try to acquire lock
      "        test %1, %1\n\t"
      "        jnz 1b\n" // retry until 0
      : // output
      : "m" (value), "r" (1) // input
      : "memory" // clobbered
    );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
    asm volatile (
      "1:      pause\n\t" // spin // TAG: should pause be removed here
      "        cmpl %1, %0\n\t"
      "        je 1b\n\t"
      "        xchgl %1, %0\n\t" // try to acquire lock
      "        test %1, %1\n\t"
      "        jnz 1b\n" // retry until 0
      : // output
      : "m" (value), "r" (1) // input
      : "memory" // clobbered
    );
#else
  while (!tryExclusiveLock());
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
