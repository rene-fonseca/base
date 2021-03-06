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

#include <base/Exception.h>
#include <base/Object.h>
#include <base/OutOfDomain.h>
#include <base/concurrency/Event.h>
#include <base/concurrency/Runnable.h>
#include <base/concurrency/ThreadKey.h>
#include <base/mem/Allocator.h>
#include <base/mem/NullPointer.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/RandomInputStream.h>
#include <base/StackFrame.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup concurrency Concurrency
*/

class Runnable;

class ThreadLocalContext;

/**
  Thread (a single flow of control).

  Example:
  @code
  class MyActiveObject : public Runnable {
  protected:

    unsigned int count = 0;
  public:

    MyThread(unsigned int _count) : count(_count) {
    }
    
    void run() {
      setThreadName("MyThread");
      while (count--) {
      }
    }
  };

  class MyApplication : public Application {
  public:
  
    void main() {
      MyActiveObject myActiveObject(100);
      Thread myThread(myActiveObject);
      myThread.start(); // start thread
      myThread.join(); // wait for thread to complete
    }
  };
  @endcode

  @short Thread
  @ingroup concurrency
  @see Runnable
  @version 1.3
*/

class _COM_AZURE_DEV__BASE__API Thread : public Runnable {
  friend class ThreadImpl;
  friend class DaemonImpl;
public:

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  static constexpr bool SUPPORTS_THREADING = false;
#else
  static constexpr bool SUPPORTS_THREADING = true;
#endif

  /** Thread resource identifier type. */
  typedef void* Identifier;

  /* Structure holding the user and system times. */
  class _COM_AZURE_DEV__BASE__API Times {
  public:
    
    /** The user mode time in nanoseconds. */
    uint64 user = 0;
    /** The system mode time in nanoseconds. */
    uint64 system = 0;

    inline Times() noexcept
    {
    }

    inline Times(uint64 _user, uint64 _system) noexcept
      : user(_user), system(_system)
    {
    }

    /** Returns the total processing time in nanoseconds. */
    inline uint64 getTotal() const noexcept
    {
      return user + system;
    }

    /** Returns the total processing time in microseconds. */
    inline uint64 getTotal_US() const noexcept
    {
      return (user + system + 500)/1000;
    }
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
  static constexpr MemorySize THREAD_LOCAL_STORAGE = 4096;
  
  /**
    Group of exceptions raised directly by the Thread class.
    
    @short Thread exception
    @ingroup concurrency exceptions
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API ThreadException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline ThreadException() noexcept
    {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline ThreadException(const char* message) noexcept
      : Exception(message)
    {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline ThreadException(const Type& type) noexcept
      : Exception(type)
    {
    }
    
    /**
      Initializes the exception object.
    
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline ThreadException(const char* message, const Type& type) noexcept
      : Exception(message, type) {
    }
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };

  /**
    Raised if thread tries to manage itself when disallowed.

    @short Exception raised by Thread.
    @ingroup concurrency exceptions
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API Self : public ThreadException {
  public:

    inline Self() noexcept
    {
    }
    
    inline Self(const char* message) noexcept
      : ThreadException(message)
    {
    }
    
    inline Self(const Type& type) noexcept
      : ThreadException(type)
    {
    }
    
    inline Self(const char* message, const Type& type) noexcept
      : ThreadException(message, type)
    {
    }
  };

  /**
    Returns the priority level corresponding to the named priority.

    @param priority The named priority.
  */
  static int getNamedPriority(Priority priority) noexcept;

  /**
    Returns the identifier of the executing thread.
  */
  static Identifier getIdentifier() noexcept;
private:

  // TAG: refactor to use AnyReference instead
  /** The parent thread of the thread. */
  Thread* parent = nullptr;
  /** The runnable object. */
  Runnable* runnable = nullptr;
  /** Specifies that the thread should be terminated. */
  volatile bool terminated = false;
  /** State. */
  volatile State state = NOTSTARTED;
  /** Identifier for the thread. */
  Identifier identifier = nullptr;
  /** Termination synchronization object. */
  Event terminationEvent;
  /** Simple ID assigned  to thread. */
  unsigned int simpleId = 0;

  /** The thread object associated with context. */
  static ThreadKey<ThreadLocalContext> threadLocalContext;

  /*
    This class is used to initialize and destroy the thread local resources.
  */
  class _COM_AZURE_DEV__BASE__API ThreadLocal {
  public:

    /**
      Initializes thread local context.
    */
    ThreadLocal(Thread* thread);

    /**
      Release thread local context.
    */
    ~ThreadLocal();
  };

  /**
    Redirects a thread to a specified runnable object.
  */
  static void* entry(Thread* thread) noexcept;
protected:

  /**
    Returns the active object.
  */
  inline Runnable* getRunnable() noexcept
  {
    return runnable;
  }
public:

  /**
    Exits the executing thread. May result in resource leaks if objects
    have been created on the heap. It is not recommended that you use this
    method.
  */
  static void exit() noexcept;

  /**
    Returns the thread object associated with the executing thread.
  */
  static ThreadLocalContext* getLocalContext() noexcept;

  /** Performance counters. */
  class PerformanceCounters {
  public:

    /** Microseconds spend waiting (blocking). */
    uint64 waiting = 0;
    /** Bytes read. */
    uint64 bytesRead = 0;
    /** Bytes written. */
    uint64 bytesWritten = 0;
  };

  /**
    Returns the thread performance counters. Requires Profiler to be enabled.
  */
  static PerformanceCounters getPerformanceCounters() noexcept;

  /**
    Returns the thread object associated with the executing thread.
  */
  static Thread* getThread() noexcept;

  /** Reuses thread local buffer is available. Avoids heap allocations. */
  class UseThreadLocalBuffer {
  private:

    Allocator<uint8> newStorage;
    Allocator<uint8>* reusedStorage  = nullptr;
    int id = -1;
  public:

    UseThreadLocalBuffer(MemorySize size = 0);

    UseThreadLocalBuffer(const UseThreadLocalBuffer&) = delete;
    UseThreadLocalBuffer& operator=(const UseThreadLocalBuffer&) = delete;

    inline operator Allocator<uint8>&() noexcept
    {
      return reusedStorage ? *reusedStorage : newStorage;
    }

    ~UseThreadLocalBuffer();
  };

  /**
    Makes the executing thread sleep for at least the specified time.

    @param nanoseconds The desired time in nanoseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void nanosleep(unsigned int nanoseconds);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param microseconds The desired time in microseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void microsleep(unsigned int microseconds);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param milliseconds The desired time in milliseconds to make the thread
    sleep. The value must be within the domain from 0 to 999999999.
  */
  static void millisleep(unsigned int milliseconds);

  /**
    Makes the executing thread sleep for at least the specified time.

    @param seconds The desired time in seconds to make the thread sleep.
    The value must be within the domain from 0 to 999999.
  */
  static void sleep(unsigned int seconds);

  /**
    Relinquishes the time slot of currently executing thread voluntarily
    without blocking. Notifies the scheduler that the current thread is willing
    to release its time slice to other threads of the same or higher priority.
  */
  static void yield() noexcept;
private:
  
  /**
    Initializes thread object for the current execution context with the
    specified parent.
  */
  Thread(Thread* parent) noexcept;
  
  /**
    Sets the termination state of the thread just before the thread is exiting.
  */
  void setTerminationState(State state) noexcept;
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
  */
  Thread();
  
  /**
    Initializes thread object. The thread is suspended until it is
    explicitly started.

    @param runnable The desired object to be run when the thread is started.
  */
  Thread(Runnable* runnable);
  
  /**
    Returns the thread that created this thread. Returns 0 for the main thread.
  */
  inline Thread* getParent() const noexcept
  {
    return parent;
  }

  /**
    Returns the execution state of the thread.
  */
  inline State getState() const noexcept
  {
    return state;
  }
  
  /**
    Returns true if the thread is alive and kicking.
  */
  bool isAlive() const noexcept;

  /**
    Returns true if the executing thread is an ancestor of this thread.
  */
  bool isAncestor() const noexcept;

  /**
    Returns true if the executing thread is a child of this thread.
  */
  bool isChild() const noexcept;

  /**
    Returns true if the thread is the main thread.
  */
  inline bool isMainThread() const noexcept
  {
    return parent == nullptr;
  }

  /**
    Returns the simple ID assigned to the thread.
  */
  inline unsigned int getThreadId() const noexcept
  {
    return simpleId;
  }

  /**
    Returns true if the executing thread is the only thread within the process.
    This method is experimental!
  */
  static bool isStandalone() noexcept;
  
  /**
    Returns the priority level of the executing thread.
  */
  static int getPriority();

  /**
    Returns true if the executing thread is the parent of this thread.
  */
  bool isParent() const noexcept;

  /**
    Returns true if the thread is the executing thread.
  */
  bool isSelf() const noexcept;

  /**
    Returns true if the thread has been asked to terminate.
  */
  inline bool isTerminated() const noexcept
  {
    return terminated;
  }

  /**
    Returns the current processing times (both user and system times).
  */
  static Times getTimes() noexcept;
  
  /**
    Returns the simple ID assigned to the thread.
  */
  static unsigned int getThreadSimpleId() noexcept;

  /** Returns the thread name is available. */
  static String getThreadName() noexcept;

  /** Sets thread name. */
  static void setThreadName(const char* name);

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
  bool join() const;
  
  /**
    Entry point for the thread.
  */
  void run();
  
  /**
    Starts the thread. The underlying context of execution is allocated here
    and not in the thread constructor.
  */
  void start();

  /**
    Asks the thread to terminate as soon as possible. This does not block
    the executing thread.
  */
  void terminate() noexcept;

  /**
    Destroys the thread object. The calling thread blocks until the thread has
    completed. It is also valid to destroy a thread which has never been
    started. The thread will dead-lock if it tries to destroy itself. The
    resources associated with the thread context may linger for some time until
    the operating system desides its time to release them.
  */
  ~Thread();
};

/** Add resource times. */
inline Thread::Times operator+(const Thread::Times& a, const Thread::Times& b) noexcept
{
  return Thread::Times{a.user + b.user, a.system + b.system};
}

/** Subtract resource times. */
inline Thread::Times operator-(const Thread::Times& a, const Thread::Times& b) noexcept
{
  return Thread::Times{a.user - b.user, a.system - b.system};
}

/** Manages fixed size resource. Not MT-safe. */
template<unsigned int SIZE>
class FixedResourceManager {
public:

  bool slots[SIZE]; // debug only
  unsigned int availableSlots[SIZE] = {0}; // available slots
  unsigned int count = SIZE; // number of available slots

  /** Initializes manager. */
  FixedResourceManager() noexcept
  {
    for (unsigned int i = 0; i < SIZE; ++i) {
      slots[i] = true;
      availableSlots[i] = i;
    }
  }

  /** Returns the next available slot. Returns -1 if none available. */
  int acquire() noexcept
  {
    if (count > 0) {
      --count;
      auto id = availableSlots[count];
      BASSERT(slots[id]);
      slots[id] = false; // mark in use
      return id;
    }
    return -1;
  }

  /** Releases the given slot. */
  inline void release(unsigned int id) noexcept
  {
    BASSERT(id < SIZE);
    BASSERT(!slots[id]); // must be allocated
    slots[id] = true; // mark available
    availableSlots[count++] = id; // return to available
  }
};

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Thread)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
