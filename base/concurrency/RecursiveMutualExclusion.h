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

#include <base/ResourceException.h>
#include <base/concurrency/Lock.h>
#include <base/concurrency/MutualExclusionException.h>
#include <base/Resource.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class implements a recursive mutual exclusion object.
  
  @short Recursive mutual exclusion synchronization object.
  @ingroup concurrency
  @see Lock
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API RecursiveMutualExclusion : public Resource, public Lock {
public:
  
  typedef ExclusiveSynchronize<RecursiveMutualExclusion> Sync;

  /**
    Initializes a mutual exclusion object in the unlocked state. Raises
    ResourceException if unable to initialize the object.
  */
  RecursiveMutualExclusion();

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Raises
    MutualExclusionException on undefined failure.
  */
  void exclusiveLock() const;

  /**
    Attempts to lock this mutual exclusion object. Raises
    MutualExclusionException on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  bool tryExclusiveLock() const;

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Raises
    MutualExclusionException on undefined failure.
  */
  inline void sharedLock() const
  {
    exclusiveLock();
  }

  /**
    Attempts to lock this mutual exclusion object. Raises
    MutualExclusionException on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  inline bool trySharedLock() const
  {
    return tryExclusiveLock();
  }

  /**
    This method unlocks this mutual exclusion object. The calling thread must
    have a lock on this mutual exlusion object prior to invocation otherwise
    the behavior is undefined. Raises MutualExclusionException on
    failure.
  */
  void releaseLock() const;

  /**
    Destroys the mutual exclusion object. The mutual exclusion must be in the
    unlocked state prior to destruction. Raises MutualExclusionException on
    failure.
  */
  ~RecursiveMutualExclusion();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
