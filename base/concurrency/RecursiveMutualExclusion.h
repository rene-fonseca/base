/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/Thread.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class implements a recursive mutual exclusion object.
  
  @short Recursive mutual exclusion synchronization object.
  @ingroup concurrency
  @see Lock
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API RecursiveMutualExclusion : public Lock {
private:
  
  typedef SpinLock Guard;
  typedef MutualExclusion Lock;
  /** Guard. */
  Guard guard;
  /** Lock. */
  Lock lock;
  /** The current owner. */
  mutable Thread::Identifier owner = 0;
  /** The number of locks held by the owner. */
  mutable unsigned int numberOfLocks = 0;
public:
  
  typedef ExclusiveSynchronize<RecursiveMutualExclusion> Sync;

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
  inline void sharedLock() const throw(MutualExclusionException)
  {
    exclusiveLock();
  }

  /**
    Attempts to lock this mutual exclusion object. Raises
    MutualExclusionException on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  inline bool trySharedLock() const throw(MutualExclusionException)
  {
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
  ~RecursiveMutualExclusion();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
