/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SEMAPHORE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SEMAPHORE_H

#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>
#include <base/OutOfDomain.h>
#include <base/Overflow.h>
#include <base/Primitives.h>
#include <base/OperatingSystem.h>

// TAG: fix alien header
#if defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  #include <semaphore.h>
  #include <limits.h>
#elif (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <pthread.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Semaphore synchronization object. Semaphores are useful when resources need to
  be synchronized between multiple threads.
  
  @short Semaphore synchronization object
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class Semaphore : public virtual Object {
public:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  static const unsigned int MAXIMUM = PrimitiveTraits<int>::MAXIMUM;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  static const unsigned int MAXIMUM = _POSIX_SEM_VALUE_MAX;
#else
  static const unsigned int MAXIMUM = PrimitiveTraits<int>::MAXIMUM;
#endif // flavor
private:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  OperatingSystem::Handle semaphore;
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

  /**
    Exception thrown directly by the Semaphore class on improper use.
    
    @short Semaphore exception
    @ingroup concurrency exceptions
    @see Semaphore
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class SemaphoreException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    SemaphoreException() throw() {}

    /**
      Initializes the exception object.
      
      @param message The message.
    */
    SemaphoreException(const char* message) throw() {}

    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    SemaphoreException(Type type) throw() : Exception(type) {}
  
    /**
      Initializes the exception object.
    
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    SemaphoreException(const char* message, Type type) throw() : Exception(message, type) {}
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
  void wait() const throw(SemaphoreException);

  /**
    Non-blocking variant of wait.

    @return True if the semaphore was decremented.
  */
  bool tryWait() const throw(SemaphoreException);

  /**
    Destroys the semaphore object.
  */
  ~Semaphore() throw(SemaphoreException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
