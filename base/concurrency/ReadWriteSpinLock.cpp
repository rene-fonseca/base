/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ReadWriteSpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ReadWriteSpinLock::ReadWriteSpinLock() throw()
  : writer(0), numberOfReaders(0) {
}

void ReadWriteSpinLock::exclusiveLock() const throw() {
  lock.exclusiveLock();
  // id is really not required when non-recursive (bool would do)
  ExclusiveSynchronize<Guard> _guard(guard);
  writer = Thread::getIdentifier();
}

bool ReadWriteSpinLock::tryExclusiveLock() const throw() {
  if (!lock.tryExclusiveLock()) {
    return false;
  }
  // id is really not required when non-recursive (bool would do)
  ExclusiveSynchronize<Guard> _guard(guard);
  writer = Thread::getIdentifier();
  return true;
}

void ReadWriteSpinLock::sharedLock() const throw() {
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

bool ReadWriteSpinLock::trySharedLock() const throw() {
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

void ReadWriteSpinLock::releaseLock() const throw() {
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
