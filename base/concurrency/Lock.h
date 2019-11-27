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

#include <base/concurrency/LockException.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This interface is implemented by classes used to synchronize contexts of
  execution by locking/unlocking.
  
  @short Lock interface
  @ingroup concurrency
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Lock {
private:

  /* Disable the default copy constructor. */
  Lock(const Lock& copy) throw();
  /* Disable the default assignment operator. */
  Lock& operator=(const Lock& assign) throw();
public:

  typedef ExclusiveSynchronize<Lock> Sync;

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
