/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SpinLock::SpinLock() throw() {
}

void SpinLock::exclusiveLock() const throw() {
  MemorySize expected = 0;
  while (!value.compareAndExchangeWeak(expected, 1)) {
    // yield
  }
}

bool SpinLock::tryExclusiveLock() const throw() {
  MemorySize expected = 0;
  return value.compareAndExchangeWeak(expected, 1);
}

void SpinLock::releaseLock() const throw() {
  value = 0;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
