/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__THREAD_H
#define _DK_SDU_MIP__BASE_THREAD__THREAD_H

#include <config.h>
#include <base/Object.h>
#include <base/Exception.h>
#include <base/OutOfDomain.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/Runnable.h>
#include <base/concurrency/ThreadKey.h>
#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Synchronize.h>

#if defined(__win32__)
  #include <windows.h>
#else
  #include <pthread.h>
#endif

/** Specifies the size of the thread local buffer. */
#define THREAD_LOCAL_STORAGE 4096

class Runnable;

/** Scheduling policy type. */
typedef enum {INHERITED, FIFO, REALTIME, OTHER} SchedulingPolicy;

/** Thread termination state type. */
typedef enum {ALIVE, NORMAL, EXIT, EXCEPTION, CANCEL, INTERNAL} ThreadTermination;

/*
typedef enum {
  ALIVE, // thread is still running
  NORMAL, // thread has exited normally
  EXIT, // thread was exited using Thread::exit()
  EXCEPTION, // thread has exited due to uncaught exception
  CANCEL, // thread was cancelled
  INTERNAL // thread was exited due to internal exception - please forgive me
} ThreadTermination;
*/

  /**
    Thread (a single flow of control).

    Example:
    <pre>
    class MyActiveObject : public Runnable {
    protected:

      unsigned int count;
    public:

      MyThread(unsigned int c) throw() : count(c) {}

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

    @see Runnable
    @author René Møller Fonseca
    @version 1.1
  */

  class Thread : public Object {
  public:

    /** Group of exceptions thrown directly by the Thread class. */
    class ThreadException : public Exception {
    public:
      ThreadException() throw() : Exception() {}
      ThreadException(const char* message) throw() : Exception(message) {}
    };

    /** Thrown if thread tries to manage itself when disallowed. */
    class Self : public ThreadException {
    };
  private:

#if defined(__win32__)
    /** Redirects a thread to a specified runnable object. */
    static DWORD WINAPI execute(Thread* thread) throw();
#else
    /** Redirects a thread to a specified runnable object. */
    static void* execute(Thread* thread) throw();
#endif
  private:

    /** The parent thread of the thread. */
    Thread* parent;
    /** The runnable object. */
    Runnable* runnable;
    /** Specifies that the thread should be terminated. */
    volatile bool terminated;
    /** Termination status. */
    ThreadTermination termination;
    /** Event used to start thread. */
    Event event;
#if defined(__win32__)
    /** Handle to the thread. */
    HANDLE threadHandle;
    /** Identifier for the thread. */
    DWORD threadID;
#else
    /** Identifier for the thread. */
    pthread_t threadID;
#endif
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
    static char* getLocalStorage() throw();

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
      Relinquishes the currently executing thread voluntarily without blocking.
      Notifies the scheduler that the current thread is willing to release its
      time slice to other threads of the same or higher priority.
    */
    static void yield() throw();
  private:

    /**
      Initializes thread object for the current execution context. Only to be
      used for the main thread.
    */
    Thread() throw();
  protected:

    /**
      Invocated by a child thread of this thread upon termination.

      @param child The child thread.
    */
    void onChildTermination(Thread* child);
  public:

    /**
      Initializes thread object. The thread is suspended until it is
      explicitly started.

      @param runnable The desired object to be run when the thread is started.
    */
    Thread(Runnable& runnable) throw(ResourceException);

    /**
      Returns the thread that created this thread. Returns NULL for the main
      thread.
    */
    inline Thread* getParent() const throw() {return parent;};

    /**
      Returns the termination state.
    */
    ThreadTermination getTerminationState() const throw() {return termination;};

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
      Waits for this thread to terminate. Throws a 'ThreadException' if a
      thread tries to wait for itself to exit. Several threads are not allowed
      to be waiting for the same thread to complete.
    */
    void join() const throw(ThreadException);

    /**
      Starts the thread. Has no effect the second time.
    */
    void start() throw();

    /**
      Asks the thread to terminate as soon as possible. This does not block
      the executing thread.
    */
    void terminate() throw();

    /**
      Destroys the thread object. The thread must be completed prior to
      destruction.
    */
    ~Thread() throw(ThreadException);

    /**
      Writes a string representation of a thread object to a format stream.
    */
    friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value);
  };

/**
  Writes a string representation of a thread object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value);

#endif
