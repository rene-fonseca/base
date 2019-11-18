/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Thread.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/string/String.h>
#include <base/Application.h>
#include <base/Cast.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV)
#  include <base/platforms/compiler/v3mv/exception.h> // includes private features
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
// #define _WIN32_WINNT _WIN32_WINNT_WINXP
#  include <windows.h>
#else // pthread
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <pthread.h>
#  include <signal.h>
#  include <time.h>
#  include <sys/time.h>
#  include <unistd.h>
#  include <errno.h>
#  if defined(_COM_AZURE_DEV__BASE__HAVE_NANOSLEEP)
#    include <time.h> // get nanosleep prototype
#  else // fall back on pselect and finally select
#    include <sys/select.h>
#  endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Thread::ThreadLocal::ThreadLocal(Thread* _thread)
{
  if (!INLINE_ASSERT(_thread)) {
    return;
  }
  auto tlc = new ThreadLocalContext();
  tlc->thread = _thread;
  threadLocalContext.setKey(tlc);
}

Thread::ThreadLocal::~ThreadLocal()
{
  auto tlc = threadLocalContext.getKey();
  if (INLINE_ASSERT(tlc)) {
    threadLocalContext.setKey(nullptr);
    delete tlc; // free thread local storage
  }
}

#if 0
uint8* Thread::ThreadLocal::getStorage(MemorySize size) throw()
{
  auto s = storage.getKey();
  if (!s) {
    s = new Allocator<uint8>(maximum(size, Thread::THREAD_LOCAL_STORAGE));
    storage.setKey(s);
  }
  if (size > s->getSize()) {
    s->setSize(size);
  }
  return s->getElements();
}

void Thread::ThreadLocal::garbageCollect() throw()
{
  auto s = storage.getKey();
  if (s) {
    s->setSize(Thread::THREAD_LOCAL_STORAGE);
  }
}
#endif



void* Thread::entry(Thread* thread) throw()
{
#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV) && !defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV_TRANSPARENT)
  const abi::__cxa_eh_globals* abi::__cxa_get_globals(); // this allows us to use __cxa_get_globals_fast
#endif
  try {
    thread->state = ALIVE;
    ThreadLocal threadLocal(thread);
    try {
      Profiler::WaitTask profile("Thread::entry()"); // TAG: include name of thread
      thread->getRunnable()->run();
      thread->state = TERMINATED;
      Thread* parent = thread->getParent();
      bassert(parent, ThreadException(Type::getType<Thread>()));
      parent->onChildTermination(thread); // signal parent
      // TAG: problem if parent is destroyed before child
    } catch (Exception& e) {
      Application::getApplication()->exceptionHandler(e);
    } catch (...) {
      Application::getApplication()->exceptionHandler();
      thread->state = EXCEPTION; // uncaugth exception
    }
  } catch (...) {
    thread->state = INTERNAL; // hopefully we will never end up here
  }
  // TAG: set kernel and user time
  thread->terminationEvent.signal(); // do not access state here after
  return 0;
}

void Thread::exit() throw()
{
  BASSERT(getThread()->state == ALIVE);
  getThread()->state = EXIT;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::ExitThread(0); // will properly create resource leaks
#else // pthread
  pthread_exit(0); // will properly create resource leaks
#endif // flavor
}

ThreadLocalContext* Thread::getLocalContext() noexcept
{
  auto tlc = threadLocalContext.getKey();
  // BASSERT(tlc); // happens when system starts a new thread - e.g. Ctrl+C on Win32
  return tlc;
}

Thread* Thread::getThread() noexcept
{
  auto tlc = threadLocalContext.getKey();
  return INLINE_ASSERT(tlc) ? tlc->thread : nullptr;
}

Thread::UseThreadLocalBuffer::UseThreadLocalBuffer(MemorySize size)
{
  // BASSERT(size > 0);
  if (size == 0) {
    size = THREAD_LOCAL_STORAGE;
  }
  if (auto tlc = threadLocalContext.getKey()) {
    id = tlc->storageManager.acquire();
    if (id >= 0) {
      auto& buffer = tlc->storage[id];
      if (buffer.getCapacity() < Thread::THREAD_LOCAL_STORAGE) {
        buffer.ensureCapacity(Thread::THREAD_LOCAL_STORAGE);
      }
      buffer.setSize(size);
      reusedStorage = &buffer;
    }
  }
}

Thread::UseThreadLocalBuffer::~UseThreadLocalBuffer()
{
  if (reusedStorage) {
    if (auto tlc = threadLocalContext.getKey()) {
      if (reusedStorage->getSize() > (Thread::THREAD_LOCAL_STORAGE * 16)) {
        reusedStorage->ensureCapacity(Thread::THREAD_LOCAL_STORAGE);
        reusedStorage->setSize(0);
      }
      tlc->storageManager.release(id);
    }
  }
}

#if 0
// TAG: add helper function for ensuring thread local buffer of the right size and automatic clear big buffers?
static uint8* getStorage(MemorySize size) throw();
static void garbageCollect() throw();
#endif

void Thread::nanosleep(unsigned int nanoseconds) throw(OutOfDomain)
{
  Profiler::WaitTask profile("sleep");
  
  if (nanoseconds >= 1000000000) {
    throw OutOfDomain(Type::getType<Thread>());
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: use select (but test if better)
  HANDLE timer = ::CreateWaitableTimer(0, TRUE, 0);
  if (timer) {
    long long timeout = -(static_cast<long long>(nanoseconds) + 99)/100; // "-" selects relative mode
    ::SetWaitableTimer(timer, (const LARGE_INTEGER*)&timeout, 0, 0, 0, FALSE);
    ::WaitForSingleObjectEx(timer, INFINITE, TRUE);
    // either WAIT_OBJECT_0 or WAIT_IO_COMPLETION
    ::CloseHandle(timer);
  } else {
    ::Sleep((nanoseconds+999999)/1000000); // round up
  }
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = nanoseconds/1000000000;
    interval.tv_nsec = nanoseconds % 1000000000;
    if (::nanosleep(&interval, &interval) == -1) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_nsec;
    }
  #elif defined(_COM_AZURE_DEV__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = nanoseconds/1000000000;
    interval.tv_nsec = nanoseconds % 1000000000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = (nanoseconds+999)/1000000000;
    interval.tv_usec = (nanoseconds+999)/1000; // round up
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif // flavor
}

void Thread::microsleep(unsigned int microseconds) throw(OutOfDomain)
{
  Profiler::WaitTask profile("sleep");

  if (microseconds >= 1000000000) {
    throw OutOfDomain(Type::getType<Thread>());
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::Sleep((microseconds+999)/1000); // round up
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_nsec = (microseconds % 1000000) * 1000;
    if (::nanosleep(&interval, &interval) == -1) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_nsec;
    }
  #elif defined(_COM_AZURE_DEV__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_nsec = (microseconds % 1000000) * 1000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = microseconds/1000000;
    interval.tv_usec = microseconds;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif
}

void Thread::millisleep(unsigned int milliseconds) throw(OutOfDomain)
{
  Profiler::WaitTask profile("sleep");

  if (milliseconds >= 1000000000) {
    throw OutOfDomain(Type::getType<Thread>());
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::Sleep(milliseconds);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_nsec = (milliseconds % 1000) * 1000000;
    if (::nanosleep(&interval, &interval) == -1) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_sec * 1000 + interval.tv_nsec/1000000;
    }
  #elif defined(_COM_AZURE_DEV__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_nsec = (milliseconds % 1000) * 1000000;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = milliseconds/1000;
    interval.tv_usec = (milliseconds % 1000) * 1000;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//    return 0;
#endif
}

void Thread::sleep(unsigned int seconds) throw(OutOfDomain)
{
  Profiler::WaitTask profile("sleep");

  if (seconds >= 1000000) {
    throw OutOfDomain(Type::getType<Thread>());
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::Sleep(seconds * 1000);
#else // unix
  #if defined(_COM_AZURE_DEV__BASE__HAVE_NANOSLEEP)
    struct timespec interval;
    interval.tv_sec = seconds;
    interval.tv_nsec = 0;
    if (::nanosleep(&interval, &interval) == -1) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return interval.tv_sec;
    }
  #elif defined(_COM_AZURE_DEV__BASE__HAVE_PSELECT)
    struct timespec interval;
    interval.tv_sec = seconds;
    interval.tv_nsec = 0;
    int result = ::pselect(0, 0, 0, 0, &interval, 0);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since pselect must support min. 31 days
  #else
    struct timeval interval;
    interval.tv_sec = seconds;
    interval.tv_usec = 0;
    int result = ::select(0, 0, 0, 0, &interval);
    if (result != 0) {
      BASSERT(errno == EINTR); // interrupted by signal
//      return 0; // TAG: calculate remaining time
    }
    // [EINVAL] is not possible since select must support min. 31 days
  #endif
//  return 0;
#endif
}

void Thread::yield() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::SwitchToThread(); // no errors
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_YIELD)
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

inline const void* getAsPointer(const void* value) noexcept
{
  return value;
}

inline void* getAsPointer(void* value) noexcept
{
  return value;
}

inline void* getAsPointer(const int8 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemoryDiff>(value));
}

inline void* getAsPointer(const int16 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemoryDiff>(value));
}

inline void* getAsPointer(const int32 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemoryDiff>(value));
}

inline void* getAsPointer(const long value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemoryDiff>(value));
}

#if (_COM_AZURE_DEV__BASE__COMPILER != _COM_AZURE_DEV__BASE__COMPILER_GCC)
inline void* getAsPointer(const int64 value) noexcept
{
  BASSERT(sizeof(void*) >= sizeof(value));
  return reinterpret_cast<void*>(static_cast<MemoryDiff>(value));
}
#endif

inline void* getAsPointer(const uint8 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemorySize>(value));
}

inline void* getAsPointer(const uint16 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemorySize>(value));
}

inline void* getAsPointer(const uint32 value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemorySize>(value));
}

inline void* getAsPointer(const unsigned long value) noexcept
{
  return reinterpret_cast<void*>(static_cast<MemorySize>(value));
}

#if (_COM_AZURE_DEV__BASE__COMPILER != _COM_AZURE_DEV__BASE__COMPILER_GCC)
inline void* getAsPointer(const uint64 value) noexcept
{
  BASSERT(sizeof(void*) >= sizeof(value));
  return reinterpret_cast<void*>(static_cast<MemorySize>(value));
}
#endif

Thread::Thread(Thread* _parent) throw()
  : parent(_parent),
    runnable(nullptr),
    terminated(false),
    state(ALIVE)
{
#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV) && !defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV_TRANSPARENT)
  const abi::__cxa_eh_globals* abi::__cxa_get_globals(); // this allows us to use __cxa_get_globals_fast
#endif
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  identifier = getAsPointer(::GetCurrentThreadId());
#else // pthread
  identifier = getAsPointer(::pthread_self());
#endif
}

Thread::Thread() throw(ResourceException)
  : runnable(this),
    terminated(false),
    state(NOTSTARTED),
    identifier(nullptr)
{
  parent = Thread::getThread();
  BASSERT(parent); // a parent must always exist
}

Thread::Thread(Runnable* _runnable) throw(NullPointer, ResourceException)
  : runnable(_runnable),
    terminated(false),
    state(NOTSTARTED),
    identifier(nullptr)
{
  if (!runnable) {
    throw NullPointer(this);
  }
  parent = Thread::getThread();
  BASSERT(parent); // a parent must always exist
}

void Thread::setTerminationState(State state) throw() {
  BASSERT(isSelf() && (state > ALIVE));
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::GetCurrentThreadId() == getAddressOf(identifier);
#else // pthread
  return ::pthread_self() == Cast::extract<pthread_t>(identifier);
#endif
}

bool Thread::isStandalone() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // THREAD_AM_I_LAST_THREAD info;
//  status = ntapi::NtQueryInformationThread(::GetCurrentThread(), ntapi::ThreadAmILastThread, &info, sizeof(info), 0);
  return false;
#else
  return false;
#endif
}


// TAG: put in Base.h
template<class TYPE>
inline TYPE clamp(TYPE minimum, TYPE value, TYPE maximum) throw() {
  if (value < minimum) {
    return minimum;
  } else if (value > maximum) {
    return maximum;
  } else {
    return value;
  }
}

// TAG: put in Base.h
template<class TYPE>
inline bool isWithin(TYPE minimum, TYPE value, TYPE maximum) throw() {
  if (value < minimum) {
    return false;
  } else if (value > maximum) {
    return false;
  } else {
    return true;
  }
}

int Thread::getNamedPriority(Priority priority) throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static const int PRIORITY[] = {7 - 31, 7 - 15, 7 - 7, 7 - 1};
#else // unix
  static const int PRIORITY[] = {-20, -20, 0, 19};
#endif // flavor
  BASSERT(isWithin<int>(0, priority, 3));
  return PRIORITY[priority];
}

Thread::Identifier Thread::getIdentifier() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return getAsPointer(::GetCurrentThreadId());

  // add optimized method for windows nt and intel platform
  // structured exception handling: fs:[0x00]
  // stack user top: fs:[0x04]
  // stack user base: fs:[0x08]
  // arbitrary: fs:[0x14]
  // pointer to thread local storage: fs:[0x2c]
  // process id: fs:[0x20]
  // thread id: fs:[0x24]
#else // pthread
  return getAsPointer(::pthread_self());
#endif
}

#if !defined(BELOW_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
#  define BELOW_NORMAL_PRIORITY_CLASS ((DWORD)0x00004000)
#endif

#if !defined(ABOVE_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
#  define ABOVE_NORMAL_PRIORITY_CLASS ((DWORD)0x00008000)
#endif

int Thread::getPriority() throw(ThreadException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // see http://msdn.microsoft.com/library/en-us/dllproc/prothred_75ir.asp
  DWORD priorityClass = ::GetPriorityClass(::GetCurrentProcess());
  int priority = ::GetThreadPriority(::GetCurrentThread());
  if ((priorityClass == 0) || (priority == THREAD_PRIORITY_ERROR_RETURN)) {
    throw ThreadException("Unable to get priority of thread", Type::getType<Thread>());
  }
  
  // named thread priorities
  //   THREAD_PRIORITY_IDLE
  //   THREAD_PRIORITY_TIME_CRITICAL
  //   THREAD_PRIORITY_LOWEST
  //   THREAD_PRIORITY_BELOW_NORMAL
  //   THREAD_PRIORITY_NORMAL
  //   THREAD_PRIORITY_ABOVE_NORMAL
  //   THREAD_PRIORITY_HIGHEST

  int basePriority = 0;
  switch (priorityClass) {
  case IDLE_PRIORITY_CLASS:
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 1;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 15;
      break;
    default:
      basePriority = 4 + priority;
    }
    break;
  case BELOW_NORMAL_PRIORITY_CLASS: // w2k or later
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 1;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 15;
      break;
    default:
      basePriority = 6 + priority;
    }
    break;
  case ABOVE_NORMAL_PRIORITY_CLASS: // w2k or later
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 1;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 15;
      break;
    default:
      basePriority = 10 + priority;
    }
    break;
  case HIGH_PRIORITY_CLASS:
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 1;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 15;
      break;
    default:
      basePriority = 13 + priority;
    }
    break;
  case REALTIME_PRIORITY_CLASS:
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 16;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 31;
      break;
    default:
      basePriority = 24 + priority; // [17; 31]
    }
    break;
  case NORMAL_PRIORITY_CLASS:
  default:
    switch (priority) {
    case THREAD_PRIORITY_IDLE:
      basePriority = 1;
      break;
    case THREAD_PRIORITY_TIME_CRITICAL:
      basePriority = 15;
      break;
    default:
      basePriority = 7 + priority;
    }
  }
  
  if (priorityClass == REALTIME_PRIORITY_CLASS) {
    basePriority = clamp(16, basePriority, 31);
  } else {
    basePriority = clamp(1, basePriority, 15);
  }
  return 7 - basePriority;
#else // unix
  return 0; // TAG: use process priority?
#endif
}

Thread::Times Thread::getTimes() throw()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Thread::Times result;
  FILETIME creationTime;
  FILETIME exitTime;
  FILETIME system;
  FILETIME user;
  BOOL status = ::GetThreadTimes(::GetCurrentThread(), &creationTime, &exitTime, &system, &user);
  BASSERT(status);
  result.user = Cast::impersonate<unsigned long long>(user) * 100ULL;
  result.system = Cast::impersonate<unsigned long long>(system) * 100ULL;
  return result;
#else // unix
  #if defined(_POSIX_THREAD_CPUTIME)
    Thread::Times result;
    struct timespec ts;
    ::clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    result.user = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    result.system = 0;
    return result;
  #else
    Thread::Times result; // TAG: use current time - time at thread start
    result.user = 0;
    result.system = 0;
    return result;
  #endif
#endif // flavor
}

bool Thread::join() const throw(ThreadException)
{
  Profiler::WaitTask profile("Thread::join()");

  if (state == NOTSTARTED) {
    return false;
  }
  terminationEvent.wait(); // allows multiple contexts to wait for thread to terminate
  BASSERT(state > ALIVE); // thread must be terminated here
  return true;
}

void Thread::run() {
}

void Thread::start() throw(ThreadException) {
  // TAG: don't forget the thread priority
  bassert(state == NOTSTARTED, ThreadException(this));
  state = STARTING;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD id = 0;
  HANDLE handle = ::CreateThread(
    0,
    0,
    (LPTHREAD_START_ROUTINE)entry,
    this,
    0,
    &id
  );
  bassert(handle, ResourceException("Unable to create thread", this));
  identifier = getAsPointer(id);
  ::CloseHandle(handle); // detach
  // TAG: does this always work or must this be postponed until entry function
#else // pthread
  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
  pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED);
  pthread_t id;
  if (pthread_create(&id, &attributes, (void*(*)(void*))&entry, (void*)this)) {
    pthread_attr_destroy(&attributes);
    throw ResourceException("Unable to create thread", this);
  }
  identifier = getAsPointer(id);
  pthread_attr_destroy(&attributes);
#endif
}

void Thread::terminate() throw()
{
  if (!terminated) {
    terminated = true;
    if (runnable) {
      runnable->onTermination();
    }
  }
}

Thread::~Thread()
{
  if (getParent() != 0) {
    if (state != Thread::NOTSTARTED) {
      terminationEvent.wait(); // allows multiple contexts to wait for thread to terminate
      BASSERT(state > Thread::ALIVE); // thread must be terminated here
    }
  }
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Thread);

class MyThread : public Thread {
private:
  
  TEST_CLASS(Thread)* parent = nullptr;
public:
  
  MyThread(TEST_CLASS(Thread)* _parent) : parent(_parent)
  {
  }
  
  void run() override;
};

class TEST_CLASS(Thread) : public UnitTest {
private:

  MutualExclusion lock;
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/concurrency");
  TEST_DEPENDENCY("base/*/AtomicCounter");
  TEST_DEPENDENCY("base/*/MutualExclusion");
  TEST_TIMEOUT_MS(30 * 1000);

  void runFromThread()
  {
    TEST_ASSERT(!Thread::getThread()->isMainThread());
    lock.exclusiveLock();
    Thread::nanosleep(1000 * 1000);
    lock.releaseLock();
  }
  
  void run() override
  {
    lock.exclusiveLock();

    MyThread thread1(this);
    thread1.start();
    Thread::nanosleep(1000 * 1000);
    lock.releaseLock();
    thread1.join();
  }
};

void MyThread::run()
{
  parent->runFromThread();
}

TEST_REGISTER(Thread);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
