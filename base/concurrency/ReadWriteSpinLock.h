/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_SPIN_LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__READ_WRITE_SPIN_LOCK_H

#include <base/concurrency/Thread.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a read-write lock for optimal reader performance on
  systems. Please note that the lock/unlock mechanism is considered a
  non-modifying property of a class.
  
  @short Read-write synchronization object.
  @ingroup concurrency
  @see MutualExclusion ReadWriteLock SpinLock
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ReadWriteSpinLock {
private:
  
  typedef SpinLock Guard;
  typedef SpinLock Lock;
  /** Guard. */
  Guard guard;
  /** Lock. */
  Lock lock;
  /** The owner holding the exclusive lock or the scheduled writer. */
  mutable Thread::Identifier writer;
  /** The number of readers currently holding the lock. */
  mutable unsigned int numberOfReaders;
public:
  
  /**
    Initializes the spin lock in unlocked state.
  */
  ReadWriteSpinLock() throw();
  
  /**
    Acquires an exclusive lock.
  */
  void exclusiveLock() const throw();
  
  /**
    Tries to acquire an exclusive lock.
    
    @return True on success.
  */
  bool tryExclusiveLock() const throw();
  
  /**
    Acquires a shared lock.
  */
  inline void sharedLock() const throw();
  
  /**
    Tries to acquire a shared lock.
    
    @return True on success.
  */
  bool trySharedLock() const throw();
  
  /**
    Releases the spin lock.
  */
  void releaseLock() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
