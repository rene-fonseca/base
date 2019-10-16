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

SpinLock::SpinLock() noexcept
  : value(LOCK_FREE)
{
}

bool SpinLock::invariant() const noexcept
{
  MemoryDiff current = value;
  return (current == LOCK_FREE) || (current == LOCK_TAKEN);
}

void SpinLock::exclusiveLock() const noexcept
{
  MemoryDiff expected = LOCK_FREE;
  while (!value.compareAndExchangeWeak(expected, LOCK_TAKEN)) {

#if defined(_DEBUG)
    MemoryDiff current = value;
    ASSERT((current == LOCK_FREE) || (current == LOCK_TAKEN));
#endif

    // yield
  }
}

bool SpinLock::tryExclusiveLock() const noexcept
{
  MemoryDiff expected = LOCK_FREE;
  return value.compareAndExchangeWeak(expected, LOCK_TAKEN);
}

void SpinLock::releaseLock() const noexcept {
  value = LOCK_FREE;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
