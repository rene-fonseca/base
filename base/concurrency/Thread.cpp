/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Thread.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/Trace.h>
#include <stdio.h>

#if defined(__win32__)
  #include <windows.h>
#else // pthread
  #include <pthread.h>
  #include <signal.h>
  #include <time.h>
  #include <sys/time.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(__win32__)
// The original unhandled exception filter
static LPTOP_LEVEL_EXCEPTION_FILTER originalExceptionFilter = NULL;

// win32 - structured exception handler
LONG __stdcall exceptionFilter(EXCEPTION_POINTERS* exception) {
  //  LONG result = EXCEPTION_CONTINUE_SEARCH;
  printf("System exception: 0x%8x at %p\n", exception->ExceptionRecord->ExceptionCode, exception->ExceptionRecord->ExceptionAddress);
  return originalExceptionFilter(exception);
  //  return result;
}
#endif // __win32__

/**
  The class is used to make a Thread object for the current context.
*/
class MainThread {
private:

  Thread thread;
public:

  /**
    Initialize thread object for the current context.
  */
  MainThread() throw() : thread() {
    TRACE_MEMBER();
#if defined(__win32__)
    if (originalExceptionFilter == NULL) {
      originalExceptionFilter = SetUnhandledExceptionFilter(exceptionFilter);
    }
#endif // __win32__
  }

  inline Thread* getThread() throw() {
    return &thread;
  }

  ~MainThread() {
#if defined(__win32__)
    // Restore the original unhandled exception filter
    if (originalExceptionFilter != NULL) {
      SetUnhandledExceptionFilter(originalExceptionFilter);
    }
#endif // __win32__
    TRACE_MEMBER();
  }
};

// Setup main thread object
MainThread mainThread; // use this variable through 'threadLocal'

/**
  This class is used to initialize and destroy the thread local resources.
*/
class ThreadLocal {
private:

  /** Thread object */
  static ThreadKey<Thread> thread;
  /** Thread local storage */
  static ThreadKey<Allocator<char> > storage;
public:

  ThreadLocal(Thread* thread) {
    TRACE_MEMBER();
    this->thread.setKey(thread);
    storage.setKey(new Allocator<char>(THREAD_LOCAL_STORAGE));
  }

  static inline Thread* getThread() throw() {
    return thread.getKey();
  }

  static inline Allocator<char>* getStorage() throw() {
    return storage.getKey();
  }

  ~ThreadLocal() {
    TRACE_MEMBER();
    delete getStorage(); // free thread local storage
  }
};

/** Thread object */
ThreadKey<Thread> ThreadLocal::thread;
/** Thread local storage */
ThreadKey<Allocator<char> > ThreadLocal::storage;

// Setup thread local variables for the main thread
ThreadLocal threadLocal(mainThread.getThread());



void* Thread::entry(Thread* thread) throw() {
  //  TRACE("TRACE %p >> %s\n", thread, __PRETTY_FUNCTION__);
  try {
    ThreadLocal local(thread);
#if !defined(__win32__)
    thread->event.wait(); // wait until signaled - win32 uses suspend/resume
#endif
    try {
      thread->getRunnable()->run();
      thread->termination = NORMAL;
      Thread* parent = thread->getParent();
      if (parent) {
        parent->onChildTermination(thread); // signal parent
      }
    } catch(...) {
      thread->termination = EXCEPTION; // uncaugth exception
    }
  } catch(...) {
    thread->termination = INTERNAL; // hopefully we will never end up here
  }
  //  TRACE("TRACE %p << %s\n", thread, __PRETTY_FUNCTION__);
  return 0;
}

void Thread::exit() throw() {
#if defined(__win32__)
  ExitThread(0); // will properly create resource leaks
#else // pthread
  pthread_exit(0); // will properly create resource leaks
#endif
}

Thread* Thread::getThread() throw() {
  return ThreadLocal::getThread();
}

Allocator<char>* Thread::getLocalStorage() throw() {
  return ThreadLocal::getStorage();
}

void Thread::nanosleep(unsigned int nanoseconds) throw(OutOfDomain) {
  if (nanoseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(nanoseconds/1000);
#else // __unix__
  struct timespec interval;
  interval.tv_sec = nanoseconds / 1000000000;
  interval.tv_nsec = nanoseconds % 1000000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::microsleep(unsigned int microseconds) throw(OutOfDomain) {
  if (microseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(microseconds/1000);
#else // __unix__
  struct timespec interval;
  interval.tv_sec = microseconds / 1000000;
  interval.tv_nsec = (microseconds % 1000000) * 1000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::millisleep(unsigned int milliseconds) throw(OutOfDomain) {
  if (milliseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(milliseconds);
#else // __unix__
  struct timespec interval;
  interval.tv_sec = milliseconds / 1000;
  interval.tv_nsec = (milliseconds % 1000) * 1000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::sleep(unsigned int seconds) throw(OutOfDomain) {
  if (seconds >= 1000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(seconds * 1000);
#else // __unix__
  struct timespec interval;
  interval.tv_sec = seconds;
  interval.tv_nsec = 0;
  do {
    ::nanosleep(&interval, &interval);
  } while (interval.tv_sec != 0);
#endif
}

void Thread::yield() throw() {
#if defined(__win32__)
  SwitchToThread(); // no errors
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_YIELD)
  pthread_yield(); // ignore errors
#else // __unix__
  sched_yield(); // ignore errors
#endif
}



void Thread::onChildTermination(Thread* thread) {
  if (runnable) {
    runnable->onChild(thread);
  }
}



#if defined(__win32__)
Thread::Thread() throw() :
  parent(0), runnable(0), terminated(false), termination(ALIVE), threadHandle(0) {
  threadID = GetCurrentThreadId();
}
#else // pthread
Thread::Thread() throw() :
  parent(0), runnable(0), terminated(false), termination(ALIVE) {
  threadID = pthread_self();
}
#endif

Thread::Thread(Runnable* runnable) throw(ResourceException) :
  runnable(runnable), terminated(false), termination(ALIVE), threadID(0) {
  parent = getThread(); // must never be NULL
#if defined(__win32__)
  // we need to grant SYNCHRONIZE access so we can join the thread object
/*
  EXPLICIT_ACCESS ea;

  BuildExplicitAccessWithName(&ea, "CURRENT_USER", SYNCHRONIZE, GRANT_ACCESS, NO_INHERITANCE);

  PACL newACL;
  if (SetEntriesInAcl(1, &ea, NULL, &newACL) != ERROR_SUCCESS) {
    throw ResourceException("Unable to create thread");
  }

  PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
  if (!sd) {
    LocalFree(newACL);
    throw ResourceException("Unable to create thread");
  }
  if (!InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
    LocalFree(sd);
    LocalFree(newACL);
    throw ResourceException("Unable to create thread");
  }

  if (!SetSecurityDescriptorDacl(sd, true, newACL, false)) {
    LocalFree(sd);
    LocalFree(newACL);
    throw ResourceException("Unable to create thread");
  }

  SECURITY_ATTRIBUTES sa;
  fill<char>((char*)&sa, sizeof(SECURITY_ATTRIBUTES), 0);
  sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
  sa.lpSecurityDescriptor = sd;
  sa.bInheritHandle = false;
*/
  if ((threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entry, this, CREATE_SUSPENDED, &threadID)) == NULL) {
    //    LocalFree(sd);
    //    LocalFree(newACL);
    throw ResourceException("Unable to create thread");
  }

  /*  if (!CloseHandle(threadHandle)) { // ignore error???
    ThreadException("Unable to release thread");
    }*/
  
  //  LocalFree(sd);
  //  LocalFree(newACL);
#else // pthread
  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED);
  if (pthread_create(&threadID, &attributes, (void*(*)(void*))&entry, (void*) this)) {
    pthread_attr_destroy(&attributes);
    throw ResourceException("Unable to create thread");
  }
  pthread_attr_destroy(&attributes);
#endif
}

bool Thread::isAlive() const throw() {
  return termination == ALIVE;
}

bool Thread::isAncestor() const throw() {
  const Thread* executing = getThread();
  const Thread* current = this;
  if (current == executing) {
    return false;
  }
  while (!current) { // stop if main thread
    if (current == executing) { // could have checked thread IDs
      return true;
    }
    current = current->getParent();
  }
  return false;
}

bool Thread::isChild() const throw() {
  const Thread* current = getThread();
  if (current == this) {
    return false;
  }
  while (!current) { // stop if main thread
    if (current == this) { // could have checked thread IDs
      return true;
    }
    current = current->getParent();
  }
  return false;
}

bool Thread::isParent() const throw() {
  return getThread() == getParent();
}

bool Thread::isSelf() const throw() {
#if defined(__win32__)
  return GetCurrentThreadId() == threadID;
#else // pthread
  return pthread_self() == threadID;
#endif
}

void Thread::join() const throw(ThreadException) {
  TRACE_MEMBER();
//    if (isSelf()) { // is thread trying to wait for itself to exit
//      throw Self();
//    }
#if defined(__win32__)
  WaitForSingleObject(threadHandle, INFINITE);
#else // pthread
  if (pthread_join(threadID, NULL)) {
    throw ThreadException("Unable to join thread");
  }
#endif
}

void Thread::start() throw() {
#if defined(__win32__)
  //  event.signal();
  ResumeThread((HANDLE)threadHandle);
#else // pthread
  event.signal();
#endif
}

void Thread::terminate() throw() {
  if (!terminated) {
    terminated = true;
    if (runnable) {
      runnable->onTermination();
    }
  }
}

Thread::~Thread() throw(ThreadException) {
  // do not close handle for main thread
  TRACE_MEMBER();
  if (getParent() != 0) {
    if (isAlive()) {
      throw ThreadException();
    }
#if defined(__win32__)
    if (!CloseHandle(threadHandle)) {
      ThreadException("Unable to release thread");
    }
#endif
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value) {
  stream << "class/Thread{"
         << "alive=" << value.isAlive()
         << "; terminated=" << value.isTerminated()
         << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
