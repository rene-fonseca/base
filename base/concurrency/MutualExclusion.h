/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H
#define _DK_SDU_MIP__BASE_THREAD__MUTUAL_EXCLUSION_H

#include "config.h"
#include "base/Object.h"
#include "base/Exception.h"
#include "base/Construct.h"
#include <pthread.h>

/**
  This class implements a mutual exclusion semaphore used to protect shared
  data structures from concurrent modifications, implementing critical sections
  and monitors. Please note that the lock/unlock mechanism is considered const.

  @author René Møller Fonseca
  @version 1.0
*/

class MutualExclusion : public Object {
protected:

 /** Internal mutex representation. */
 mutable pthread_mutex_t mutex;
public:

  /** Group of exceptions thrown directly by the MutualExclusion class. */
  class MutualExclusionException : public Exception {
  };
  /** Thrown on failure to lock the MutualExclusion object. */
  class Lock : public MutualExclusionException {
  };
  /** Thrown on failure to unlock the MutualExclusion object. */
  class Unlock : public MutualExclusionException {
  };

  /**
    Initializes the mutex object in unlocked state.
  */
  MutualExclusion() throw(Construct);

  /**
    Requests the mutex. The calling thread is suspended if the mutex has been
    locked by another thread. Throws the exception
    'MutualExclusionException:Lock' on failure.
  */
  void lock() const throw(Lock);

  /**
    Tries to lock the mutex. Throws the exception
    'MutualExclusionException:Lock' on failure.

    @return True if the mutex was locked successfully.
  */
  bool tryLock() const throw(Lock);

  /**
    This method unlocks the mutex. Throws the exception
    'MutualExclusionException:Unlock' on failure.
  */
  void unlock() const throw(Unlock);

  /**
    Destroys the mutex object.
  */
  ~MutualExclusion() throw();
};

#endif
