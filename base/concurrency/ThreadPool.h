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

#include <base/Object.h>
#include <base/collection/Set.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/Semaphore.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API JobProvider {
public:

  virtual bool isEmpty() const = 0;

  virtual Runnable* pop() = 0;
};



/**
  Pool of threads used to run 'm' jobs using 'n' threads. The implementation is
  MT-safe.

  @short Thread pool maintainer
  @ingroup concurrency
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ThreadPool {
private:
  
  /** The type of the guard. */
  typedef MutualExclusion Guard;
  
  Guard guard;
public:

  /**
    Exception raised directly by the ThreadPool class.

    @short Thread pool exception.
    @ingroup concurrency exceptions
  */
  class _COM_AZURE_DEV__BASE__API ThreadPoolException : public Exception {
  public:
    
    inline ThreadPoolException() throw() {
    }
    
    inline ThreadPoolException(const char* message) throw()
      : Exception(message) {
    }
    
    inline ThreadPoolException(Type type) throw() : Exception(type) {
    }
    
    inline ThreadPoolException(const char* message, Type type) throw()
      : Exception(message, type) {
    }
  };
private:

  /*
    Wrapper runnable used by ThreadPool to run the jobs. All the threads of the
    thread pool share one runnable object.
    
    @version 1.0
  */

  class Wrapper : public Runnable {
  private:

    /** The thread pool. */
    ThreadPool* pool = nullptr;
  public:

    /**
      Initialize the runnable with the specified thread pool.

      @param pool The thread pool.
    */
    inline Wrapper(ThreadPool* _pool) throw() : pool(_pool) {
    }

    /**
      Run the object.
    */
    void run() throw();
  };

  friend class Wrapper;

  /** Runnable. */
  Wrapper runnable;
  /** Job provider. */
  JobProvider* provider = nullptr;
  /** The number of desired threads. */
  unsigned int desiredThreads = 0;
  /** Specifies that the thread pool has been terminated. */
  bool terminated = false;
  /** Specifies that a thread should be terminated. */
  bool terminateOne = false;
  /** The last terminated thread. */
  Thread* terminatedThread = nullptr;
  /** Collection holding the thread objects. */
  Set<Thread*> pool;
  /** Synchronization object. */
  MutualExclusion threadLock;
  /** Synchronization object used to wake threads in the pool. */
  Semaphore semaphore;
  /** Synchronization object used to signal thread termination. */
  Semaphore termination;
private:

  void run() throw();
public:

  /**
    Initializes thread pool with no threads.

    @param provider The job provider.
  */
  ThreadPool(JobProvider* provider) throw();

  /**
    Initializes thread pool with specified number of threads.

    @param provider The job provider.
    @param threads The initial number of threads in the pool.
  */
  ThreadPool(JobProvider* provider, unsigned int threads) throw();

  /**
    Returns the desired number of threads of the pool.
  */
  unsigned int getThreads() const throw();

  /**
    Sets the desired number of threads of the pool. Blocks until accomplished.
    Raises ThreadPoolException is the pool has been terminated.
  */
  void setThreads(unsigned int value) throw(ThreadPoolException);

  /**
    Asks all the threads to terminate. Jobs that have not been started are not
    executed.
  */
  void terminate() throw();

  /**
    Waits for all the threads to terminate. All waiting jobs of the pool are
    executed first unless the pool has been explicitly terminated.
  */
  void join() throw();

  /**
    The job provider is required to invoke this function when a job becomes
    available.
  */
  void post() throw();

  /**
    Destroys the thread pool.
  */
  ~ThreadPool() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
