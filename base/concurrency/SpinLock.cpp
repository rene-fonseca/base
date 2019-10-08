/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SpinLock::SpinLock() throw() : value(0) {
}

void SpinLock::exclusiveLock() const throw() {
  unsigned long expected = 0;
  while (!value.compare_exchange_strong(expected, 1)) {
    // yield
  }
}

bool SpinLock::tryExclusiveLock() const throw() {
  unsigned long expected = 0;
  return value.compare_exchange_strong(expected, 1);
}

void SpinLock::releaseLock() const throw() {
  value = 0;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
