/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__LOCK_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__LOCK_H

#include <base/features.h>

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
public:

  /**
    Acquires an exclusive lock.
  */
  virtual void exclusiveLock() const = 0;

  /**
    Tries to acquire an exclusive lock.

    @return True on success.
  */
  virtual bool tryExclusiveLock() const = 0;

  /**
    Acquires a shared lock. For some lock implementations this will acquire an
    exclusive lock.
  */
  virtual void sharedLock() const = 0;

  /**
    Tries to acquire a shared lock. For some lock implementations this will
    acquire an exclusive lock.

    @return True on success.
  */
  virtual bool trySharedLock() const = 0;

  /**
    Releases the lock.
  */
  virtual void releaseLock() const = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
