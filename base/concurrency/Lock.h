/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__LOCK_H
#define _DK_SDU_MIP__BASE_THREAD__LOCK_H

/**
  The locking interface.

  @author René Møller Fonseca
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
    Acquires a shared lock. For some lock implementations this will acquire an exclusive lock.
  */
  virtual void sharedLock() const = 0;

  /**
    Tries to acquire a shared lock. For some lock implementations this will acquire an exclusive lock.

    @return True on success.
  */
  virtual bool trySharedLock() const = 0;

  /**
    Releases the lock.
  */
  virtual void releaseLock() const = 0;
};

#endif
