/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_LOCK_H

#include <base/Exception.h>
#include <base/Object.h>
#include <base/ResourceException.h>
#include <base/concurrency/Lock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The ReadWriteLock class implements a read-write lock for optimal reader
  performance on systems which have rwlock support and reverts to a simple
  mutex for those that do not. Please note that the lock/unlock mechanism is
  considered a non-modifying property of a class.

  @short Read-write synchronization object
  @ingroup concurrency
  @see MutualExclusion Lock
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.4
*/

class ReadWriteLock : public virtual Lock {
protected:

  /** Internal representation of object. */
  void* representation;
public:

  /**
    Exception raised directly by the ReadWriteLock class.

    @short Read-write synchronization object exception
    @ingroup concurrency exceptions
    @author Rene Moeller Fonseca
    @version 1.0
  */
  class ReadWriteLockException : public LockException {
  public:

    /**
      Initializes the exception object with no message.
    */
    inline ReadWriteLockException() throw() {
    }

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline ReadWriteLockException(const char* message) throw()
      : LockException(message) {
    }

    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline ReadWriteLockException(Type type) throw()
      : LockException(type) {
    }

    /**
      Initializes the exception object.
    
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline ReadWriteLockException(const char* message, Type type) throw()
      : LockException(message, type) {
    }
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
