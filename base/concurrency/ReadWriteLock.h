/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__READ_WRITE_LOCK_H
#define _DK_SDU_MIP__BASE_THREAD__READ_WRITE_LOCK_H

#include <config.h>
#include "base/Object.h"
#include "base/Construct.h"
#include "base/Destruct.h"
#include "Lock.h"
#include <pthread.h>

/**
  The ReadWriteLock class implements a read-write lock for optimal reader
  performance on systems which have rwlock support and reverts to a simple
  mutex for those that do not. Please note that the lock/unlock mechanism is
  considered const.

  @author René Møller Fonseca
  @version 1.0
*/

class ReadWriteLock : public Object, public virtual Lock {
protected:

  /* Internal lock representation. */
#ifdef HAVE_PTHREAD_RWLOCK
  mutable pthread_rwlock_t lock;
#else
  mutable pthread_mutex_t lock;
#endif
public:

  /** Group of exceptions thrown directly by the ReadWriteLock class. */
  class ReadWriteLockException : public Exception {
  };
  /** Thrown on failure to lock the read-write lock object. */
  class Lock : public ReadWriteLockException {
  };
  /** Thrown on failure to unlock the read-write lock object. */
  class Unlock : public ReadWriteLockException {
  };

  /**
    Initializes a read-write lock in the unlocked state.
  */
  ReadWriteLock() throw(Construct);

  /**
    Lock the read-write lock for writing. The calling thread acquires the lock
    if no other thread (reader or writer) holds the lock. Otherwise, the thread
    blocks until it can be acquired. Results are undefined if the calling
    thread holds the read-write lock. Throws 'ReadWriteLock:Lock' on failure.
  */
  void exclusiveLock() const throw();

  /**
    Attempts to lock the read-write object for writing. Throws the exception
    'ReadWriteLock::Lock' on failure.

    @return True if the read-write lock was successfully acquired for writing.
  */
  bool tryExclusiveLock() const throw();

  /**
    Applies a read lock to the read-write lock object. The calling thread does
    not return until the lock can be acquired. The calling thread is allowed to
    hold several simultaneous read locks. Results are undefined if the calling
    thread already holds a write lock. Throws the exception 'ReadWriteLock:Lock'
    on failure.
  */
  void sharedLock() const throw();

  /**
    Attempts to lock the read-write object for reading. Throws the exception
    'ReadWriteLock::Lock' on failure.

    @return True if the read-write lock was successfully acquired for reading.
  */
  bool trySharedLock() const throw();

  /**
    This method unlocks the read-write lock. Throws the exception
    'ReadWriteLock::Unlock' on failure.
  */
  void releaseLock() const throw();

  /**
    Destroys the read-write lock object.
  */
  ~ReadWriteLock() throw(Destruct);
};

#endif
