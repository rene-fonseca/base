/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SYNCHRONIZE_H
#define _DK_SDU_MIP__BASE_THREAD__SYNCHRONIZE_H

#include "ThreadLock.h"

/** Exclusively synchronization of this scope of this synchronizable class (the variable _sync is reserved for this purpose). */
#define SynchronizeExclusively() Synchronize _sync(this, true)
/** Shared synchronization of this scope of this synchronizeable class (the variable _sync is reserved for this purpose). */
#define SynchronizeShared() Synchronize _sync(this, false)

/** Exclusively synchronization of this scope using the specified lock (the variable _sync is reserved for this purpose). */
#define SynchronizeExclusivelyWith(lock) Synchronize _sync(lock, true)
/** Shared synchronization of this scope using the specified lock (the variable _sync is reserved for this purpose). */
#define SynchronizeSharedWith(lock) Synchronize _sync(lock, false)

/** Release synchronization object. */
#define SynchronizeRelease() (_sync.release())

/**
  This is a stack based synchronization wrapper of the ThreadLock class. A Synchronize object will acquire and release an exclusive (or a shared lock) on the provided thread lock object when it is, respectively, created and destroyed. If required, the lock can be explicitly released by calling the method release(). This is required before throwing an exception.

  @author René Møller Fonseca
  @version 1.0
*/

class Synchronize : public Object {
private:

  /** The lock object to synchronize with. */
  const ThreadLock* l;

  Synchronize(const Synchronize& copy); // disable copy constructor
  Synchronize& operator=(const Synchronize& eq); // disable assignment
public:

  /**
    Initializes the synchronization object.

    @param lock The lock to synchronize with.
    @param exclusive Specifies if the lock should be exclusive (write-lock) or shared (read-lock). Default is exclusive.
  */
  inline explicit Synchronize(const ThreadLock* lock, bool exclusive = true) throw() : l(lock) {
    if (exclusive) {
      l->writeLock();
    } else {
      l->readLock();
    }
  }

  /**
    Forces the lock to be released.
  */
  inline void release() throw() {
    l->unlock();
  }

  /**
    Releases the lock if not already released and destroys the synchronization object.
  */
  inline ~Synchronize() throw() {
    release();
  }
};

#endif
