/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Thread.h>
#include <base/string/String.h>

// TAG: exception handling - we need something like static void Exception::initialize() to allow for
// TAG: need symbol _DK_SDU_MIP__BASE__EXCEPTION_V3MV_TRANSPARENT
#if (__GNUC__ == 3)
  #define _DK_SDU_MIP__BASE__EXCEPTION_V3MV
#endif

#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV)
  #include <base/platforms/compiler/v3mv/exception.h> // includes private features
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // pthread
  #include <pthread.h>
  #include <signal.h>
  #include <time.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <errno.h>
  #if defined(_DK_SDU_MIP__BASE__HAVE_NANOSLEEP)
    #include <time.h> // get nanosleep prototype
  #else // fall back on pselect and finally select
    #include <sys/select.h>
  #endif
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ThreadKey<Thread> Thread::ThreadLocal::thread; // thread object
ThreadKey<Allocator<char> > Thread::ThreadLocal::storage; // thread local storage

Thread::ThreadLocal::ThreadLocal(Thread* _thread) throw(MemoryException) {
  thread.setKey(_thread);
  storage.setKey(new Allocator<char>(Thread::THREAD_LOCAL_STORAGE));
}

Thread::ThreadLocal::~ThreadLocal() throw() {
  delete getStorage(); // free thread local storage
}



void* Thread::entry(Thread* thread) throw() {
#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV) && !defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV_TRANSPARENT)
  const abi::__cxa_eh_globals* abi::__cxa_get_globals(); // this allows us to use __cxa_get_globals_fast
#endif
  try {
    thread->state = ALIVE;
    ThreadLocal threadLocal(thread);
    try {
      thread->getRunnable()->run();
      thread->state = TERMINATED;
      Thread* parent = thread->getParent();
      assert(parent, ThreadException(Type::getType<Thread>()));
      parent->onChildTermination(thread); // signal parent
      // TAG: problem if parent is destroyed before child
    } catch(...) {
      thread->state = EXCEPTION; // uncaugth exception
    }
  } catch(...) {
    thread->state = INTERNAL; // hopefully we will never end up here
  }
  // TAG: set kernel and user time
  thread->terminationEvent.signal(); // do not access state here after
  return 0;
}

void Thread::exit() throw() {
  ASSERT(getThread()->state == ALIVE);
  getThread()->state = EXIT;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::ExitThread(0); // will properly create resource leaks
#else // pthread
  pthread_exit(0); // will properly create resource leaks
#endif // flavor
}

Thread* Thread::getThread() throw() {
  return ThreadLocal::getThread();
}

Allocator<char>* Thread::getLocalStorage() throw() {
  return ThreadLocal::getStorage();
}

void Thread::nanosleep(unsigned int nanoseconds) throw(OutOfDomain) {
  assert(nanoseconds < 1000000000, OutOfDomain(Type::getType<Thread>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::Sleep((nanoseconds+999999)/1000000); // round up
  // TAG: use select (but test if better)
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = nanoseconds/1000000000;
    interval.tv_nsec = nanoseconds % 1000000000;
    if (::nanosleep(&interval, &interval) == -1) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_nsec;
    }
  #elif defined(_DK_SDU_MIP__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = nanoseconds/1000000000;
    interval.tv_nsec = nanoseconds % 1000000000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = (nanoseconds+999)/1000000000;
    interval.tv_usec = (nanoseconds+999)/1000; // round up
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif // flavor
}

void Thread::microsleep(unsigned int microseconds) throw(OutOfDomain) {
  assert(microseconds < 1000000000, OutOfDomain(Type::getType<Thread>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::Sleep((microseconds+999)/1000); // round up
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_nsec = (microseconds % 1000000) * 1000;
    if (::nanosleep(&interval, &interval) == -1) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_nsec;
    }
  #elif defined(_DK_SDU_MIP__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_nsec = (microseconds % 1000000) * 1000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_usec = microseconds;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif
}

void Thread::millisleep(unsigned int milliseconds) throw(OutOfDomain) {
  assert(milliseconds < 1000000000, OutOfDomain(Type::getType<Thread>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::Sleep(milliseconds);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_nsec = (milliseconds % 1000) * 1000000;
    if (::nanosleep(&interval, &interval) == -1) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_sec * 1000 + interval.tv_nsec/1000000;
    }
  #elif defined(_DK_SDU_MIP__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_nsec = (milliseconds % 1000) * 1000000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_usec = (milliseconds % 1000) * 1000;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif
}

void Thread::sleep(unsigned int seconds) throw(OutOfDomain) {
  assert(seconds < 1000000, OutOfDomain(Type::getType<Thread>()));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::Sleep(seconds * 1000);
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = seconds;
    interval.tv_nsec = 0;
    if (::nanosleep(&interval, &interval) == -1) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_sec;
    }
  #elif defined(_DK_SDU_MIP__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = seconds;
    interval.tv_nsec = 0;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = seconds;
    interval.tv_usec = 0;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      ASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//  return 0;
#endif
}

void Thread::yield() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SwitchToThread(); // no errors
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_YIELD)
  pthread_yield(); // ignore errors
#else // unix
  sched_yield(); // ignore errors
#endif
}



void Thread::onChildTermination(Thread* thread) {
  if (runnable) {
    runnable->onChild(thread);
  }
}



Thread::Thread(Thread* _parent) throw()
  : parent(_parent),
    runnable(0),
    terminated(false),
    state(ALIVE) {
#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV) && !defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV_TRANSPARENT)
  const abi::__cxa_eh_globals* abi::__cxa_get_globals(); // this allows us to use __cxa_get_globals_fast
#endif
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ASSERT(sizeof(DWORD) <= sizeof(Identifier));
  identifier = (Identifier)::GetCurrentThreadId();
#else // pthread
  ASSERT(sizeof(pthread_t) <= sizeof(Identifier));
  identifier = (Identifier)::pthread_self();
#endif
}

Thread::Thread(Runnable* _runnable) throw(NullPointer, ResourceException)
  : runnable(_runnable),
    terminated(false),
    state(NOTSTARTED),
    identifier(0) {  
  assert(runnable, NullPointer(this));
  parent = Thread::getThread();
  ASSERT(parent); // a parent must always exist
}

void Thread::setTerminationState(State state) throw() {
  ASSERT(isSelf() && (state > ALIVE));
  this->state = state;
  terminationEvent.signal(); // do not access state here after
}

bool Thread::isAlive() const throw() {
  return state == ALIVE;
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
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ASSERT(sizeof(DWORD) <= sizeof(Identifier));
  return ::GetCurrentThreadId() == (DWORD)identifier;
#else // pthread
  ASSERT(sizeof(pthread_t) <= sizeof(Identifier));
  return ::pthread_self() == (pthread_t)identifier;
#endif
}

bool Thread::join() const throw(ThreadException) {
  if (state == NOTSTARTED) {
    return false;
  }
  terminationEvent.wait(); // allows multiple contexts to wait for thread to terminate
  ASSERT(state > ALIVE); // thread must be terminated here
  return true;
}

void Thread::start() throw(ThreadException) {
  // TAG: don't forget the thread priority
  assert(state == NOTSTARTED, ThreadException(this));
  state = STARTING;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ASSERT(sizeof(DWORD) <= sizeof(Identifier));
  HANDLE handle;
  DWORD id;
  if ((handle = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)entry, this, 0, &id)) == 0) {
    throw ResourceException("Unable to create thread", this);
  }
  identifier = (Identifier)id;
  ::CloseHandle(handle); // detach
  // TAG: does this always work or must this be postponed until entry function
#else // pthread
  ASSERT(sizeof(pthread_t) <= sizeof(Identifier));
  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
  pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED);
  pthread_t id;
  if (pthread_create(&id, &attributes, (void*(*)(void*))&entry, (void*)this)) {
    pthread_attr_destroy(&attributes);
    throw ResourceException("Unable to create thread", this);
  }
  identifier = (Identifier)id;
  pthread_attr_destroy(&attributes);
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
  TRACE_MEMBER();
  if (getParent() != 0) {
    if (state != NOTSTARTED) {
      terminationEvent.wait(); // allows multiple contexts to wait for thread to terminate
      ASSERT(state > ALIVE); // thread must be terminated here
    }
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value) {
  stream << '{'
         << MESSAGE("alive=") << value.isAlive()
         << MESSAGE("; terminated=") << value.isTerminated()
         << MESSAGE("; state=") << value.getState()
         << '}';
  return stream;
}

/*
#include <sys/times.h>
#include <limits.h>

clock_t times(struct tms *buffer);
*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
