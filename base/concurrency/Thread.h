/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__THREAD_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__THREAD_H

#include <base/Exception.h>
#include <base/Object.h>
#include <base/OutOfDomain.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/Runnable.h>
#include <base/concurrency/ThreadKey.h>
#include <base/mem/Allocator.h>
#include <base/mem/NullPointer.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup concurrency Concurrency
*/

class Runnable;

/**
  Thread (a single flow of control).

  Example:
  <pre>
  class MyActiveObject : public Runnable {
  protected:

    unsigned int count;
  public:

    MyThread(unsigned int _count) throw() : count(_count) {}
    
    void run() throw() {
      while (count--) {
      }
    }
  };

  int main() {
    MyActiveObject myActiveObject(100);
    Thread myThread(myActiveObject);
    myThread.start(); // start thread
    myThread.join(); // wait for thread to complete
    return 0;
  }
  </pre>

  @short Thread
  @ingroup concurrency
  @see Runnable
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Thread : public Object {
  friend class ThreadImpl;
  friend class DaemonImpl;
public:

  /** Thread resource identifier type. */
  typedef void* Identifier;

  /** Structure holding the user and system times. */
  struct Times {
    /** The user mode time in nanoseconds. */
    unsigned long long user;
    /** The system mode time in nanoseconds. */
    unsigned long long system;
  };
  
  /** Scheduling policy type. */
  enum SchedulingPolicy {
    INHERITED, /**< Scheduling policy is inherited from parent to child thread. */
    FIFO, /**< First-in-first-out policy. */
    REALTIME, /**< Real time scheduling policy. */
    OTHER /**< Unspecified policy. */
  };

  /** Thread running state. */
  enum State {
    NOTSTARTED, /**< Thrad has been created but not started. */
    STARTING, /**< Thread has been started but may not have begun execution. */
    ALIVE, /**< Thread is running. */
    TERMINATED, /**< Thread has exited normally. */
    EXIT, /**< Thread was exited using Thread::exit(). */
    EXCEPTION, /**< Thread has exited due to uncaught exception. */
    CANCEL, /**< Thread was cancelled. */
    INTERNAL /**< Thread was exited due to internal exception - please forgive me. */
  };
  
  enum Priority {
    LOWEST_PRIORITY, /**< Asks for the lowest priority level. */
    NORMAL_PRIORITY, /**< Asks for the normal priority level (which is 0). */
    HIGHEST_PRIORITY, /**< Asks for the highest recommended priority level. */
    HIGHEST_REALTIME_PRIORITY /**< Asks for the highest supported priority level. */
  };
  
  /** Specifies the size of the thread local storage. */
  static const unsigned int THREAD_LOCAL_STORAGE = 4096;
  
  /**
    Group of exceptions raised directly by the Thread class.
    
    @short Thread exception
    @ingroup concurrency exceptions
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class ThreadException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    ThreadException() throw() {}
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    ThreadException(const char* message) throw() : Exception(message) {}
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    ThreadException(Type type) throw() : Exception(type) {}
    
    /**
      Initializes the exception object.
    
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    ThreadException(const char* message, Type type) throw() : Exception(message, type) {}
  };

  /**
    Raised if thread tries to manage itself when disallowed.

    @short Exception raised by Thread.
    @ingroup concurrency exceptions
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class Self : public ThreadException {
  public:

    Self() throw() {}
    Self(const char* message) throw() : ThreadException(message) {}
    Self(Type type) throw() : ThreadException(type) {}
    Self(const char* message, Type type) throw() : ThreadException(message, type) {}
  };

  /**
    Returns the priority level corresponding to the named priority.

    @param priority The named priority.
  */
  static int getNamedPriority(Priority priority) throw();
private:

  /** The parent thread of the thread. */
  Thread* parent;
  /** The runnable object. */
  Runnable* runnable;
  /** Specifies that the thread should be terminated. */
  volatile bool terminated;
  /** State. */
  volatile State state;
  /** Identifier for the thread. */
  Identifier identifier;
  /** Termination synchronization object. */
  Event terminationEvent;
  
  /**
    This class is used to initialize and destroy the thread local resources.
  */
  class ThreadLocal {
  private:

    /** The thread object associated with context. */
    static ThreadKey<Thread> thread;
    /** The thread local storage. */
    static ThreadKey<Allocator<char> > storage;
  public:

    /**
      Initializes thread local storage.
    */
    ThreadLocal(Thread* thread) throw(MemoryException);
    
    /**
      Returns the thread object.
    */
    static inline Thread* getThread() throw() {
      return thread.getKey();
    }

    /**
      Returns the thread local storage.
    */
    static inline Allocator<char>* getStorage() throw() {
      return storage.getKey();
    }

    /**
      Release thread local storage.
    */
    ~ThreadLocal() throw();
  };

  /**
    Redirects a thread to a specified runnable object.
  */
  static void* entry(Thread* thread) throw();
protected:

  /**
    Returns the active object.
  */
  inline Runnable* getRunnable() throw() {return runnable;}
public:

  /**
    Exits the executing thread. May result in resource leaks if objects
    have been created on the heap. It is not recommended that you use this
    method.
  */
  static void exit() throw();

  /**
    Returns the thread object associated with the executing thread.
  */
  static Thread* getThread() throw();

  /**
    Returns the thread object associated with the executing thread.
  */
  static Allocator<char>* getLocalStorage() throw();

  /**
    Makes the executing thread sleep for at least the specified time.

    @param nanoseconds The desired time in nanoseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void nanosleep(unsigned int nanoseconds) throw(OutOfDomain);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param microseconds The desired time in microseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void microsleep(unsigned int microseconds) throw(OutOfDomain);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param milliseconds The desired time in milliseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void millisleep(unsigned int milliseconds) throw(OutOfDomain);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param seconds The desired time in seconds to make the thread sleep.
    The value must be within the domain from 0 to 999999.
  */
  static void sleep(unsigned int seconds) throw(OutOfDomain);

  /**
    Relinquishes the time slot of currently executing thread voluntarily
    without blocking. Notifies the scheduler that the current thread is willing
    to release its time slice to other threads of the same or higher priority.
  */
  static void yield() throw();
private:
  
  /**
    Initializes thread object for the current execution context with the
    specified parent.
  */
  Thread(Thread* parent) throw();
  
  /**
    Sets the termination state of the thread just before the thread is exiting.
  */
  void setTerminationState(State state) throw();
protected:

  /**
    Invoked by a child thread of this thread upon termination. Only threads that
    have been started result in a notification on termination.

    @param child The child thread.
  */
  void onChildTermination(Thread* child);
public:

  /**
    Initializes thread object. The thread is suspended until it is
    explicitly started.

    @param runnable The desired object to be run when the thread is started.
  */
  Thread(Runnable* runnable) throw(NullPointer, ResourceException);

  /**
    Returns the thread that created this thread. Returns 0 for the main thread.
  */
  inline Thread* getParent() const throw() {return parent;}

  /**
    Returns the execution state of the thread.
  */
  inline State getState() const throw() {return state;}
  
  /**
    Returns true if the thread is alive and kicking.
  */
  bool isAlive() const throw();

  /**
    Returns true if the executing thread is an ancestor of this thread.
  */
  bool isAncestor() const throw();

  /**
    Returns true if the executing thread is a child of this thread.
  */
  bool isChild() const throw();

  /**
    Returns true if the thread is the main thread.
  */
  inline bool isMainThread() const throw() {return parent == 0;}

  /**
    Returns true if the executing thread is the only thread within the process.
    This method is experimental!
  */
  static bool isStandalone() throw();
  
  /**
    Returns the priority level of the executing thread.
  */
  static int getPriority() throw(ThreadException);

  /**
    Returns true if the executing thread is the parent of this thread.
  */
  bool isParent() const throw();

  /**
    Returns true if the thread is the executing thread.
  */
  bool isSelf() const throw();

  /**
    Returns true if the thread has been asked to terminate.
  */
  inline bool isTerminated() const throw() {return terminated;}

  /**
    Returns the current processing times (both user and system times).
  */
  static Times getTimes() throw();
  
  /**
    The calling thread waits for the thread complete. Several threads are
    allowed to be waiting for the same thread to complete. A thread will block
    indefinitely if it tries to join itself. The resources associated with the
    thread context may linger for some time until the operating system desides
    its time to release them. It is, however, legal to destroy the thread object
    after join() has returned.

    @return The method returns false if the thread has not been started and
    otherwise true.
  */
  bool join() const throw(ThreadException);

  /**
    Starts the thread. The underlying context of execution is allocated here
    and not in the thread constructor.
  */
  void start() throw(ThreadException);

  /**
    Asks the thread to terminate as soon as possible. This does not block
    the executing thread.
  */
  void terminate() throw();

  /**
    Destroys the thread object. The calling thread blocks until the thread has
    completed. It is also valid to destroy a thread which has never been
    started. The thread will dead-lock if it tries to destroy itself. The
    resources associated with the thread context may linger for some time until
    the operating system desides its time to release them.
  */
  ~Thread() throw(ThreadException);
};

/**
  Writes a string representation of a thread object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
