/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H
#define _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H

#include <base/features.h>
#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>
#include <base/concurrency/Lock.h>

#if defined(__win32__)
  #include <windows.h>
#else
  #include <pthread.h>
#endif

/**
  This class implements a mutual exclusion object used to protect shared
  data structures from concurrent modifications, implementing critical sections
  and monitors. Please note that the lock/unlock mechanism is considered a
  non-modifying property of a class.

  @see Lock
  @author René Møller Fonseca
  @version 1.1
*/

class MutualExclusion : public virtual Object, public virtual Lock {
protected:

#if defined(__win32__)
  /** Internal mutex representation. */
  mutable CRITICAL_SECTION lock;
#else
  /** Internal mutex representation. */
  mutable pthread_mutex_t lock;
#endif // __win32__
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
    Destroys the mutual exclusion object. The mutual exclusion must be
    in the unlocked state prior to destruction. Throws
    'ReadWriteLockException' on failure. 
  */
  ~MutualExclusion() throw(MutualExclusionException);
};

#endif
