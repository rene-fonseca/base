/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ReadWriteSpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ReadWriteSpinLock::ReadWriteSpinLock() noexcept
{
}

void ReadWriteSpinLock::exclusiveLock() const noexcept
{
  lock.exclusiveLock();
  // id is really not required when non-recursive (bool would do)
  ExclusiveSynchronize<Guard> _guard(guard);
  writer = Thread::getIdentifier();
}

bool ReadWriteSpinLock::tryExclusiveLock() const noexcept
{
  if (!lock.tryExclusiveLock()) {
    return false;
  }
  // id is really not required when non-recursive (bool would do)
  ExclusiveSynchronize<Guard> _guard(guard);
  writer = Thread::getIdentifier();
  return true;
}

void ReadWriteSpinLock::sharedLock() const noexcept
{
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (numberOfReaders) {
      ++numberOfReaders; // already locked by reader
      return;
    }
  }
  
  while (true) { // block
    bool success = lock.tryExclusiveLock();
    {
      ExclusiveSynchronize<Guard> _guard(guard);
      if (success) {
        // writer cannot be scheduled when we have the lock
        ++numberOfReaders;
        return;
      } else if (numberOfReaders) {
        ++numberOfReaders; // lock already help by reader
        return;
      }
    }
    // TAG: if concurrency level == 1 then Thread::yield()
  }
}

bool ReadWriteSpinLock::trySharedLock() const noexcept
{
  ExclusiveSynchronize<Guard> _guard(guard);
  if (numberOfReaders) {
    ++numberOfReaders;
    return true;
  } else if (writer == 0) {
    if (lock.tryExclusiveLock()) {
      ++numberOfReaders;
      return true;
    }
  }
  return false;
}

void ReadWriteSpinLock::releaseLock() const noexcept
{
  Profiler::pushSignal("ReadWriteSpinLock::releaseLock()");
  ExclusiveSynchronize<Guard> _guard(guard);
  if (writer) {
    // assumes executing thread has the lock
    writer = 0; // relinquish ownership
    lock.releaseLock();
  } else {
    // assumes executing thread is reader
    if (--numberOfReaders == 0) {
      lock.releaseLock();
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
