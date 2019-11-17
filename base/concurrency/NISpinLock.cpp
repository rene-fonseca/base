/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/NISpinLock.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

NISpinLock::NISpinLock() noexcept
  : value(LOCK_FREE)
{
}

void NISpinLock::exclusiveLock() const noexcept
{
  Profiler::WaitTask profile("NISpinLock::exclusiveLock()");

  while (true) {
    MemoryDiff expected = LOCK_FREE;
    if (value.compareAndExchangeWeak(expected, LOCK_TAKEN)) {
      break;
    }

#if defined(_DEBUG)
    MemoryDiff current = value;
    BASSERT((current == LOCK_FREE) || (current == LOCK_TAKEN));
#endif

    // yield
  }
}

bool NISpinLock::tryExclusiveLock() const noexcept
{
  MemoryDiff expected = LOCK_FREE;
  return value.compareAndExchangeWeak(expected, LOCK_TAKEN);
}

void NISpinLock::releaseLock() const noexcept
{
  Profiler::pushSignal("NISpinLock::releaseLock()");
  value = LOCK_FREE;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
