/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/RecursiveMutualExclusion.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

RecursiveMutualExclusion::RecursiveMutualExclusion() throw(ResourceException)
  : owner(0), numberOfLocks(0) {
}

void RecursiveMutualExclusion::exclusiveLock() const
  throw(MutualExclusionException) {
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

bool RecursiveMutualExclusion::tryExclusiveLock() const
  throw(MutualExclusionException) {
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

void RecursiveMutualExclusion::releaseLock() const
  throw(MutualExclusionException) {
  // Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  // bassert(owner == id, ConcurrencyException(this));
  if (--numberOfLocks == 0) {
    owner = 0; // relinquish ownership
    lock.releaseLock();
  }
}

RecursiveMutualExclusion::~RecursiveMutualExclusion()
  throw(MutualExclusionException) {
  bassert(
    guard.tryExclusiveLock() && lock.tryExclusiveLock(),
    MutualExclusionException(this)
  );
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
