/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H
#define _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H

#include "config.h"
#include "MutualExclusion.h"
#include "base/Exception.h"
#include "base/Construct.h"
#include "base/Destruct.h"
#include "base/Overflow.h"
#include <limits.h>

#ifdef HAVE_PTHREAD_SEMAPHORE
#include <semaphore.h>
#endif

/**
  Semaphore. Synchronization object.

  @author René Møller Fonseca
  @version 1.0
*/

class Semaphore : private MutualExclusion {
private:

#ifdef HAVE_PTHREAD_SEMAPHORE
  #define MAXIMUM SEM_VALUE_MAX
#else
  #define MAXIMUM INT_MAX
#endif

#ifdef HAVE_PTHREAD_SEMAPHORE
  mutable sem_t semaphore;
#else
  /** The value of the semaphore. */
  volatile int value;
  /** Condition. */
  pthread_cond_t condition;
#endif
public:

  /** Group of exceptions thrown directly by the Semaphore class. */
  class SemaphoreException : public Exception {
  };

  /**
    Initializes the semaphore object.
  */
  Semaphore(unsigned int value = 0) throw(Construct);

  /**
    Returns the maximum value.
  */
  int getMaximum() const throw();

  /**
    Returns the current value of the semaphore.
  */
  int getValue() const throw();

  /**
    Increments the semaphore and signals any thread waiting for a change. This
    method never blocks.
  */
  void post() throw(Overflow);

  /**
    Decrements the semaphore and blocks if the semaphore is less than zero
    until another thread signals a change.
  */
  void wait() throw();

  /**
    Non-blocking variant of wait.

    @return True if the semaphore was decremented.
  */
  bool tryWait() throw();

  /**
    Destroys the semaphore object.
  */
  ~Semaphore() throw(Destruct);
};

#endif
