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
#include <base/concurrency/MutualExclusion.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
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
  mutex = new CRITICAL_SECTION[1];
  ::InitializeCriticalSection((CRITICAL_SECTION*)mutex);
  // TAG: could raise STATUS_INVALID_HANDLE
  ::EnterCriticalSection((CRITICAL_SECTION*)mutex); // force allocation of event (non-paged memory)
  ::LeaveCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  mutex = new pthread_mutex_t[1];
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    _throw ResourceException(this);
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
   #warning disabled selection of mutex type due to CYGWIN bug
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
#endif // cygwin temporary bug fix
  if (pthread_mutex_init((pthread_mutex_t*)mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#else
  // DO NOT ASSERT SINCE MUTEX IS USED BASSERT(!"Not supported.");
#endif
}

void MutualExclusion::exclusiveLock() const
{
  Profiler::WaitTask profile("MutualExclusion::exclusiveLock()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::EnterCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_lock((pthread_mutex_t*)mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    _throw MutualExclusionException(this);
  }
#else
  BASSERT(!"Not supported.");
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
  int result = pthread_mutex_trylock((pthread_mutex_t*)mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw MutualExclusionException(this);
  }
#else
  BASSERT(!"Not supported.");
  return false;
#endif
}

void MutualExclusion::releaseLock() const
{
  Profiler::pushSignal("MutualExclusion::releaseLock()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::LeaveCriticalSection((CRITICAL_SECTION*)mutex);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_unlock((pthread_mutex_t*)mutex)) {
    _throw MutualExclusionException(this);
  }
#else
  BASSERT(!"Not supported.");
#endif
}

MutualExclusion::~MutualExclusion()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::DeleteCriticalSection((CRITICAL_SECTION*)mutex);
  delete[] (CRITICAL_SECTION*)mutex;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_destroy((pthread_mutex_t*)mutex)) {
    _throw MutualExclusionException(this);
  }
  delete[] (pthread_mutex_t*)mutex;
#else
  BASSERT(!"Not supported.");
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
