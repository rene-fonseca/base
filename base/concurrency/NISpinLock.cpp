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

NISpinLock::NISpinLock() throw() {
  atomic_init(&value, 0);
}

void NISpinLock::exclusiveLock() const throw() {
  unsigned long expected = 0;
  while (!atomic_compare_exchange_strong(&value, &expected, 1)) {
    // yield
  }
  // while (!tryExclusiveLock());
}

bool NISpinLock::tryExclusiveLock() const throw() {
  unsigned long expected = 0;
  return atomic_compare_exchange_strong(&value, &expected, 1);
}

void NISpinLock::releaseLock() const throw() {
  atomic_store(&value, 0);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
