/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H
#define _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H

#include <config.h>
#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <limits.h>

#if defined(__win32__)
  #include <windows.h>
#elif HAVE_PTHREAD_SEMAPHORE
  #include <semaphore.h>
#else
  #include <pthread.h>
#endif

/**
  Semaphore. Synchronization object.

  @author René Møller Fonseca
  @version 1.1
*/

class Semaphore : public virtual Object {
public:

#if defined(__win32__)
  static const unsigned int MAXIMUM = INT_MAX;
#elif HAVE_PTHREAD_SEMAPHORE
  static const unsigned int MAXIMUM = _POSIX_SEM_VALUE_MAX;
#else
  static const unsigned int MAXIMUM = INT_MAX;
#endif
private:

#if defined(__win32__)
  HANDLE semaphore;
#elif HAVE_PTHREAD_SEMAPHORE
  mutable sem_t semaphore;
#else
  /** The value of the semaphore. */
  volatile int value;
  /** Condition. */
  pthread_cond_t condition;
  /** Mutex. */
  mutable pthread_mutex_t mutex;
#endif
public:

  /** Exception thrown directly by the Semaphore class. */
  class SemaphoreException : public Exception {
  };

  /**
    Initializes the semaphore object.
  */
  Semaphore(unsigned int value = 0) throw(OutOfDomain, ResourceException);

#if !defined(__win32__)
  /**
    Returns the current value of the semaphore. Warning this is a non-portable
    method.
  */
  unsigned int getValue() const throw(SemaphoreException);
#endif // __win32__

  /**
    Increments the semaphore and signals any thread waiting for a change. This
    method never blocks.
  */
  void post() throw(Overflow, SemaphoreException);

  /**
    Decrements the semaphore and blocks if the semaphore is less than zero
    until another thread signals a change.
  */
  void wait() throw(SemaphoreException);

  /**
    Non-blocking variant of wait.

    @return True if the semaphore was decremented.
  */
  bool tryWait() throw(SemaphoreException);

  /**
    Destroys the semaphore object.
  */
  ~Semaphore() throw(SemaphoreException);
};

#endif
