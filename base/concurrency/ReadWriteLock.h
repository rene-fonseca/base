/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__READ_WRITE_LOCK_H
#define _DK_SDU_MIP__BASE_THREAD__READ_WRITE_LOCK_H

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
  The ReadWriteLock class implements a read-write lock for optimal reader
  performance on systems which have rwlock support and reverts to a simple
  mutex for those that do not. Please note that the lock/unlock mechanism is
  considered a non-modifying property of a class.

  @see MutualExclusion Lock
  @author René Møller Fonseca
  @version 1.1
*/

class ReadWriteLock : public virtual Object, public virtual Lock {
protected:

  /** Internal mutex representation. */
#if defined(__win32__)
  mutable CRITICAL_SECTION lock;
#elif defined(HAVE_PTHREAD_RWLOCK)
  mutable pthread_rwlock_t lock;
#else
  mutable pthread_mutex_t lock;
#endif
public:

  /** Exception thrown directly by the ReadWriteLock class. */
  class ReadWriteLockException : public Exception {
  };

  /**
    Initializes a read-write lock in the unlocked state. Throws
    'ResourceException' if unable to initialize the object.
  */
  ReadWriteLock() throw(ResourceException);

  /**
    Acquires an exclusive lock (write-lock) on the read-write lock. The
    calling thread acquires the lock if no other thread (reader or writer)
    currently holds the lock. Otherwise, the thread blocks until the lock can
    be acquired. Results are undefined if the calling thread holds the
    read-write lock. Throws 'ReadWriteLockException' on failure.
  */
  void exclusiveLock() const throw(ReadWriteLockException);

  /**
    Attempts to acquire an exclusive lock on the read-write lock. Throws the
    exception 'ReadWriteLockException' on failure.

    @return True if the exclusive lock was successfully acquired.
  */
  bool tryExclusiveLock() const throw(ReadWriteLockException);

  /**
    Acquires a shared lock (read-lock) on the read-write lock. The calling
    thread blocks until the lock can be acquired. Results are undefined if the
    calling thread already holds a lock. Throws the exception
    'ReadWriteLockException' on failure.
  */
  void sharedLock() const throw(ReadWriteLockException);

  /**
    Attempts to acquire a shared lock on the read-write lock. Throws the
    exception 'ReadWriteLockException' on failure.

    @return True if the shared lock was successfully acquired.
  */
  bool trySharedLock() const throw(ReadWriteLockException);

  /**
    This method unlocks the read-write lock. Results are undefines if the
    calling thread does not hold the lock. Throws the exception
    'ReadWriteLockException' on failure.
  */
  void releaseLock() const throw(ReadWriteLockException);

  /**
    Destroys the read-write lock object. The lock must be in the unlocked
    state prior to destruction.Throws 'ReadWriteLockException' on failure. 
  */
  ~ReadWriteLock() throw(ReadWriteLockException);
};

#endif
