/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H

#include "config.h"
#include "base/Object.h"
#include "base/Exception.h"
#include "Runnable.h"
#include "MutualExclusion.h"
#include "ThreadKey.h"
#include <pthread.h>

class Runnable;

/** Scheduling policy type. */
typedef enum {
  INHERITED,
  FIFO,
  REALTIME,
  OTHER
} SchedulingPolicy;

/** Thread termination state type. */
typedef enum {
  ALIVE, // thread is still running
  NORMAL, // thread has exited normally
  EXIT, // thread was exited using Thread::exit()
  EXCEPTION, // thread has exited due to uncaught exception
  CANCEL, // thread was cancelled
  INTERNAL // thread was exited due to internal exception - please forgive me
} ThreadTermination;

  /**
    Thread (a single flow of control) implementation using Posix Threads. Do NOT call any pthread functions directly.

    @author René Møller Fonseca
    @version 1.0
  */

  class Thread : public Object {
  public:

    /** Group of exceptions thrown directly by the Thread class. */
    class ThreadException : public Exception {};
    /** Thrown if thread was expected to be alive. */
    class NotAlive : public ThreadException {};
    /** Thrown if thread tries to manage itself when disallowed. */
    class Self : public ThreadException {};
    /** Thrown if child thread tries to manage parent thread when disallowed. */
    class Child : public ThreadException {};
    /** Allows objects that have been constructed on the stack to be destructed. */
    class Exit : public ThreadException {};
  private:

    /** The main thread. */
    static Thread* mainThread;
    /** The executing thread. */
    static ThreadKey<Thread> executingThread;
    /** The default stack size. */
    const static size_t DEFAULT_STACK_SIZE = 4096; // may be too small
    /** Redirects a thread to a specified runnable object. */
    static void* execute(Thread* thread) throw();
    /** Called when a thread is exited. */
    static void exited(Thread* thread) throw();
  private:

    /** The parent thread of the thread. */
    Thread* parent;
    /** The runnable object. */
    Runnable* runnable;
    /** The minimum stack size. */
    unsigned int stackSize;
    /** Posix thread identifier. */
    pthread_t threadID;
    /** Specifies that the thread should be terminated. */
    volatile bool terminated;
    /** Mutual exclusion. */
    MutualExclusion* mutex;
    /** Termination status. */
    ThreadTermination termination;
  protected:

    /** Type of signal handlers. */
    typedef void (*SignalHandler)(int);

    /**
      Install a signal handler.

      @param signal The signal to be handled.
      @param handler The signal handler.
      @param flags Flags determining the behavior.
    */
    void installSignalHandler(int signal, SignalHandler handler, int flags) throw(Construct);

//    static void suspendHandler(int signal);
//    static void signalHandler(int signal);
//    static void cancelHandler(int signal);

    /**
      Pass event to runnable object.
    */
//    void onChild(Thread* thread);
  public:

    /**
      Inserts a cancellation point that allows the thread to be cancelled.
    */
    inline void allowCancellation() throw() {pthread_testcancel();};

    /**
      Exits the executing thread. May result in resource leaks if objects have been created on the heap. It's safe the create the objects on the stack.
    */
    static void exit() throw(Thread::Exit);

    /**
      Returns the thread object of the executing thread.
    */
    inline static Thread* getThread() throw() {return (Thread*)executingThread.getKey();};

    /**
      Makes the executing thread sleep for at least the specified time.

      @param nanoseconds The desired time in nanoseconds to make the thread sleep.
    */
    static void nanosleep(unsigned int nanoseconds) throw();

    /**
      Makes the executing thread sleep for at least the specified time.

      @param microseconds The desired time in microseconds to make the thread sleep.
    */
    static void microsleep(unsigned int microseconds) throw();

    /**
      Makes the executing thread sleep for at least the specified time.

      @param milliseconds The desired time in milliseconds to make the thread sleep.
    */
    static void millisleep(unsigned int milliseconds) throw();

    /**
      Makes the executing thread sleep for at least the specified time.

      @param seconds The desired time in seconds to make the thread sleep.
    */
    static void sleep(unsigned int seconds) throw();

    /**
      Relinquishes the currently executing thread voluntarily without blocking.
      Notifies the scheduler that the current thread is willing to release its processor to other threads of the same or higher priority.
    */
    static void yield() throw();
  protected:

    /**
      Called when child thread exists.
    */
    void onChildExit(Thread* thread);
  private:

    /**
      Constructor used to create a thread object for the current execution context. Only used to create the object for the main thread.
    */
    Thread();
  public:

    /**
      Initializes the thread object. Initially the thread is suspended.

      @param runnable The desired object to be run.
      @param schedulingPolicy The desired scheduling policy. The thread will inherit the policy from the parent thread as default.
      @param stackSize The minimum stack size.
    */
    Thread(Runnable& runnable, SchedulingPolicy schedulingPolicy = INHERITED, size_t stackSize = DEFAULT_STACK_SIZE) throw(Construct, ResourceException, MutualExclusion::MutualExclusionException);

    /**
      Returns the thread that created the thread object. Returns NULL for the main thread.
    */
    inline Thread* getParent() const throw() {return parent;};

    /**
      Returns the minimum stack size.
    */
    unsigned int getMinimumStackSize() const throw() {return stackSize;};

    /**
      Returns the termination state.
    */
    ThreadTermination getTerminationState() const throw() {return termination;};

    /**
      Returns true if the thread is alive.
    */
    inline bool isAlive() const throw() {return threadID != 0;};

    /**
      Returns true if the executing thread is an ancestor of the thread.
    */
    bool isAncestor() const throw();

    /**
      Returns true if the executing thread is a child of the thread..
    */
    bool isChild() const throw();

    /**
      Returns true if the executing thread is the parent of the thread.
    */
    inline bool isParent() const throw() {return getThread() == getParent();};

    /**
      Returns true if the thread is the executing thread.
    */
    inline bool isSelf() const throw() {return pthread_self() == threadID;};

    /**
      Returns true if the thread has been asked to terminate.
    */
    inline bool isTerminated() const throw() {return terminated;};

    /**
      Cancels the thread at a cancellation point. Objects will not have their destructors called. This is a problem if the objects have allocated memory on the heap. The executing thread is suspended until the thread has been canceled. Throws a 'ThreadException' if the thread is not alive. Throws a 'ThreadException' if the thread tries to cancel itself. If you want the executing thread to terminate use 'Thread::exit()' at any point. Throws a 'ThreadException' if a child tries to cancel an ancestor.
    */
    void cancel() throw(Self, Child, Thread::ThreadException);

    /**
      Waits for the thread to terminate. Throws a 'ThreadException' if a thread tries to wait for itself. Throws a 'ThreadException' if thread is ancestor of the executing thread.
      CONST
    */
    void join() throw(Thread::Self, Thread::Child, Thread::ThreadException);

    /**
      Throws an exception if called by........... Calling this routine multiple times has no effect.
      THINK THINK THINK... Second time call -> Exception
    */
    void start() throw(Thread::Self, Thread::Child, MutualExclusion::Unlock);

    /**
      Asks the thread to terminate as soon as possible. This does not suspend the executing thread. Use cancel() if termination should be enforced.
    */
    void terminate();

    /**
      Writes the object to standard out. This method is intended to be used for debugging purposes.
    */
    void debug() const;

    /**
      Destroys the thread object. The thread is canceled if it is still alive.
    */
    ~Thread() throw();
  };

#endif
