/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/NISpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void NISpinLock::exclusiveLock() const throw() {
  // TAG: use intrinsic
#if 0 && (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
#elif 0 && (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
#elif 0 && (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
#else
  while (!tryExclusiveLock());
#endif // architecture
}

bool NISpinLock::tryExclusiveLock() const throw() {
  // TAG: use intrinsic
/*
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64)
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
#else
#  error Architecture is not supported
#endif // architecture
*/
  return false;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
