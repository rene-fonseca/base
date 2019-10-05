/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__LOCK_H

#include <base/concurrency/LockException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This interface is implemented by classes used to synchronize contexts of
  execution by locking/unlocking.
  
  @short Lock interface
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Lock {
private:

  /* Disable the default copy constructor. */
  Lock(const Lock& copy) throw();
  /* Disable the default assignment operator. */
  Lock& operator=(const Lock& eq) throw();
public:
  
  /**
    Initializes lock.
  */
  inline Lock() throw() {
  }
  
  /**
    Acquires an exclusive lock.
  */
  virtual void exclusiveLock() const throw(LockException) = 0;
  
  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  virtual bool tryExclusiveLock() const throw(LockException) = 0;
  
  /**
    Acquires a shared lock. For some lock implementations this will acquire an
    exclusive lock.
  */
  virtual void sharedLock() const throw(LockException) = 0;
  
  /**
    Tries to acquire a shared lock. For some lock implementations this will
    acquire an exclusive lock.

    @return True on success.
  */
  virtual bool trySharedLock() const throw(LockException) = 0;
  
  /**
    Releases the lock.
  */
  virtual void releaseLock() const throw(LockException) = 0;
  
  /**
    Destroy lock.
  */
  virtual ~Lock();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
