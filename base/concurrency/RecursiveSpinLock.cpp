/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/RecursiveSpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

RecursiveSpinLock::RecursiveSpinLock() noexcept
{
}

void RecursiveSpinLock::exclusiveLock() const noexcept
{
  Thread::Identifier id = Thread::getIdentifier();
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (owner == id) {
      ++numberOfLocks;
      return;
    }
  }
  lock.exclusiveLock(); // block
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    numberOfLocks = 1;
    owner = id;
  }
}

bool RecursiveSpinLock::tryExclusiveLock() const noexcept
{
  Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  if (owner == id) {
    ++numberOfLocks;
    return true;
  } else if (numberOfLocks == 0) {
    numberOfLocks = 1;
    owner = id;
    return true;
  } else {
    return false;
  }
}

void RecursiveSpinLock::releaseLock() const noexcept
{
  Profiler::pushSignal("RecursiveSpinLock::releaseLock()");
  // Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  // bassert(owner == id, ConcurrencyException(this));
  if (--numberOfLocks == 0) {
    owner = 0; // relinquish ownership
    lock.releaseLock();
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
