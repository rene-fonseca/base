/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) // not desired here
#  undef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
#endif

#include <base/concurrency/MutualExclusion.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) // not desired here
#  define _POSIX_THREADS 1
#endif
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <pthread.h>
#  define _COM_AZURE_DEV__BASE__PTHREAD
#endif
#  include <errno.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MutualExclusion::MutualExclusion()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  mutex = new CRITICAL_SECTION;
  ::InitializeCriticalSection((CRITICAL_SECTION*)mutex);
  // TAG: could raise STATUS_INVALID_HANDLE
  ::EnterCriticalSection((CRITICAL_SECTION*)mutex); // force allocation of event (non-paged memory)
  ::LeaveCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  mutex = new pthread_mutex_t;
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    _throw ResourceException(this);
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
   #warning disabled selection of mutex type due to CYGWIN bug
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
#endif // cygwin temporary bug fix
  auto _mutex = reinterpret_cast<pthread_mutex_t*>(mutex);
  if (pthread_mutex_init(_mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#else
  // assume single threaded
  mutex = new int(0);
#endif
}

void MutualExclusion::exclusiveLock() const
{
  Profiler::WaitTask profile("MutualExclusion::exclusiveLock()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::EnterCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  auto _mutex = reinterpret_cast<pthread_mutex_t*>(mutex);
  int result = pthread_mutex_lock(_mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    _throw MutualExclusionException("Dead lock detected.", this); // TAG: add deadlock exception?
    return;
  } else {
    _throw MutualExclusionException(this);
  }
#else
  // assume single threaded
  int* handle = reinterpret_cast<int*>(mutex);
  while (*handle) {
    Thread::microsleep(1000);
  }
  *handle = 1;
#endif
}

bool MutualExclusion::tryExclusiveLock() const
{
  Profiler::WaitTask profile("MutualExclusion::tryExclusiveLock()");
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  BOOL result = TRUE;
  result = ::TryEnterCriticalSection((CRITICAL_SECTION*)mutex);
  return result;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  auto _mutex = reinterpret_cast<pthread_mutex_t*>(mutex);
  int result = pthread_mutex_trylock(_mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw MutualExclusionException(this);
  }
#else
  // assume single threaded
  int* handle = reinterpret_cast<int*>(mutex);
  if (*handle) {
    return false;
  }
  *handle = 1;
  return true;
#endif
}

void MutualExclusion::releaseLock() const
{
  Profiler::pushSignal("MutualExclusion::releaseLock()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::LeaveCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  auto _mutex = reinterpret_cast<pthread_mutex_t*>(mutex);
  int result = pthread_mutex_unlock(_mutex);
  if (result == 0) {
    return;
  } else if (result == EPERM) {
    _throw MutualExclusionException("Thread does not own mutex.", this);
  } else {
    _throw MutualExclusionException(this);
  }
#else
  int* handle = reinterpret_cast<int*>(mutex);
  if (!*handle) {
    _throw MutualExclusionException(this);
  }
  *handle = 0;
#endif
}

MutualExclusion::~MutualExclusion()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::DeleteCriticalSection((CRITICAL_SECTION*)mutex);
  delete (CRITICAL_SECTION*)mutex;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  auto _mutex = reinterpret_cast<pthread_mutex_t*>(mutex);
  if (pthread_mutex_destroy(_mutex)) {
    Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
  }
  delete _mutex;
#else
  int* handle = reinterpret_cast<int*>(mutex);
  delete handle;
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(MutualExclusion) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/concurrency");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    // TAG: add Thread
    
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_HERE(B);

    MutualExclusion lock;
    if (lock.tryExclusiveLock()) {
      TEST_HERE(A);
      lock.releaseLock();
    }
    if (lock.tryExclusiveLock()) {
      TEST_HERE(B);
      lock.releaseLock();
    }
    lock.exclusiveLock();
    lock.releaseLock();
  }
};

TEST_REGISTER(MutualExclusion);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
