/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__RECURSIVE_MUTUAL_EXCLUSION_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__RECURSIVE_MUTUAL_EXCLUSION_H

#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/Thread.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a recursive mutual exclusion object.
  
  @short Recursive mutual exclusion synchronization object.
  @ingroup concurrency
  @see Lock
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class RecursiveMutualExclusion : public Lock {
private:
  
  typedef SpinLock Guard;
  typedef MutualExclusion Lock;
  /** Guard. */
  Guard guard;
  /** Lock. */
  Lock lock;
  /** The current owner. */
  mutable Thread::Identifier owner;
  /** The number of locks held by the owner. */
  mutable unsigned int numberOfLocks;
public:
  
  /**
    Initializes a mutual exclusion object in the unlocked state. Raises
    ResourceException if unable to initialize the object.
  */
  RecursiveMutualExclusion() throw(ResourceException);

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Raises
    MutualExclusionException on undefined failure.
  */
  void exclusiveLock() const throw(MutualExclusionException);

  /**
    Attempts to lock this mutual exclusion object. Raises
    MutualExclusionException on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  bool tryExclusiveLock() const throw(MutualExclusionException);

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Raises
    MutualExclusionException on undefined failure.
  */
  inline void sharedLock() const throw(MutualExclusionException) {
    exclusiveLock();
  }

  /**
    Attempts to lock this mutual exclusion object. Raises
    MutualExclusionException on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  inline bool trySharedLock() const throw(MutualExclusionException) {
    return tryExclusiveLock();
  }

  /**
    This method unlocks this mutual exclusion object. The calling thread must
    have a lock on this mutual exlusion object prior to invocation otherwise
    the behavior is undefined. Raises MutualExclusionException on
    failure.
  */
  void releaseLock() const throw(MutualExclusionException);

  /**
    Destroys the mutual exclusion object. The mutual exclusion must be in the
    unlocked state prior to destruction. Raises MutualExclusionException on
    failure.
  */
  ~RecursiveMutualExclusion() throw(MutualExclusionException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
