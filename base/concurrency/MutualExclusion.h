/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__MUTUAL_EXCLUSION_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__MUTUAL_EXCLUSION_H

#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>
#include <base/concurrency/Lock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a mutual exclusion object used to protect shared
  data structures from concurrent modifications, implementing critical sections
  and monitors. Please note that the lock/unlock mechanism is considered a
  non-modifying property of a class.

  @ingroup concurrency
  @see Lock
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class MutualExclusion : public virtual Object, public virtual Lock {
protected:

  /** Internal representation of mutex. */
  void* mutex;
public:

  /** Exception thrown directly by the MutualExclusion class. */
  class MutualExclusionException : public Exception {
  };

  /**
    Initializes a mutual exclusion object in the unlocked state. Throws
    'ResourceException' if unable to initialize the object.
  */
  MutualExclusion() throw(ResourceException);

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Throws
    'MutualExclusionException' on undefined failure.
  */
  void exclusiveLock() const throw(MutualExclusionException);

  /**
    Attempts to lock this mutual exclusion object. Throws
    'MutualExclusionException' on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  bool tryExclusiveLock() const throw(MutualExclusionException);

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Throws
    'MutualExclusionException' on undefined failure.
  */
  inline void sharedLock() const throw(MutualExclusionException) {
    exclusiveLock();
  }

  /**
    Attempts to lock this mutual exclusion object. Throws
    'MutualExclusionException' on failure.

    @return True if the mutual exclusion was locked successfully or already
    was locked by the calling thread.
  */
  inline bool trySharedLock() const throw(MutualExclusionException) {
    return tryExclusiveLock();
  }

  /**
    This method unlocks this mutual exclusion object. The calling thread must
    have a lock on this mutual exlusion object prior to invocation otherwise
    the behaviour is undefined. Throws 'MutualExclusionException' on
    failure.
  */
  void releaseLock() const throw(MutualExclusionException);

  /**
    Destroys the mutual exclusion object. The mutual exclusion must be in the
    unlocked state prior to destruction. Throws 'MutualExclusionException' on
    failure.
  */
  ~MutualExclusion() throw(MutualExclusionException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
