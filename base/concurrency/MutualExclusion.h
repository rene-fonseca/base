/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H
#define _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H

#include <config.h>
#include "base/Object.h"
#include "base/Exception.h"
#include "base/ResourceException.h"

#ifdef __win32__
  #include <Windows.h>
#else
  #include <pthread.h>
#endif // __win32__

/**
  This class implements a mutual exclusion object used to protect shared
  data structures from concurrent modifications, implementing critical sections
  and monitors. Please note that the lock/unlock mechanism is considered a
  non-modifying property of a class.

  @author René Møller Fonseca
  @version 1.1
*/

class MutualExclusion : public Object {
protected:

#ifdef __win32__
  /** Internal mutex representation. */
  mutable CRITICAL_SECTION criticalSection;
#else
  /** Internal mutex representation. */
  mutable pthread_mutex_t mutex;
#endif // __win32__
public:

  /**
    Initializes a mutual exclusion object in the unlocked state. Throws
    'ResourceException' if unable to initialize the object.
  */
  MutualExclusion() throw(ResourceException);

  /**
    Requests a lock on this mutual exclusion object. The calling thread is
    suspended if the mutex has been locked by another thread. Does nothing if
    the calling thread already has a lock on the object. Throws 'Exception'
    on undefined failure.
  */
  void lock() const throw(Exception);

  /**
    Tries to lock this mutual exclusion object. Throws 'Exception' on
    undefined failure.

    @return True if the mutual exclusion was locked successfully or already was locked by the calling thread.
  */
  bool tryLock() const throw(Exception);

  /**
    This method unlocks this mutual exclusion object. Throws 'Exception' on
    undefined failure. The calling thread must have a lock on this mutual
    exlusion object prior to invocation otherwise the behaviour is undefined.
  */
  void unlock() const throw(Exception);

  /**
    Destroys the mutual exclusion object. The mutual exclusion must be
    in the unlocked state prior to destruction.
  */
  ~MutualExclusion() throw();
};

#endif
