/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H
#define _DK_SDU_MIP__BASE_THREAD__SEMAPHORE_H

#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/Type.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  #include <semaphore.h>
  #include <limits.h>
#else
  #include <pthread.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Semaphore. Synchronization object.

  @author René Møller Fonseca
  @version 1.1
*/

class Semaphore : public virtual Object {
public:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  static const unsigned int MAXIMUM = Int::MAXIMUM;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  static const unsigned int MAXIMUM = _POSIX_SEM_VALUE_MAX;
#else
  static const unsigned int MAXIMUM = Int::MAXIMUM;
#endif
private:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE semaphore;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
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

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  /**
    Returns the current value of the semaphore. Warning this is a non-portable
    method.
  */
  unsigned int getValue() const throw(SemaphoreException);
#endif

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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
