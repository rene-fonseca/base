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
#include <base/ResourceHandle.h>
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

class MutexHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  CRITICAL_SECTION handle;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  pthread_mutex_t mutex;

  inline MutexHandle() noexcept
  {
    clear(mutex);
  }
#else
  int count = 0;
#endif

  ~MutexHandle()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    ::DeleteCriticalSection(&handle);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
    if (pthread_mutex_destroy(&mutex) != 0) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#else
    count = 0;
#endif
  }
};

namespace {

  inline MutexHandle* toMutexHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<MutexHandle>().getValue();
    }
    return nullptr;
  }
}

MutualExclusion::MutualExclusion()
{
  Profiler::ResourceCreateTask profile("MutualExclusion::MutualExclusion()");

  Reference<MutexHandle> _handle = new MutexHandle();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::InitializeCriticalSection(&_handle->handle);
  // TAG: could raise STATUS_INVALID_HANDLE
  ::EnterCriticalSection(&_handle->handle); // force allocation of event (non-paged memory)
  ::LeaveCriticalSection(&_handle->handle);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
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
  if (pthread_mutex_init(&_handle->mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#else
  // assume single threaded
#endif
  this->handle = _handle;
  profile.setHandle(*_handle);
}

void MutualExclusion::exclusiveLock() const
{
  MutexHandle* handle = toMutexHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  Profiler::WaitTask profile("MutualExclusion::exclusiveLock()", handle);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::EnterCriticalSection(&handle->handle);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_lock(&handle->mutex);
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
  while (handle->count) {
    Thread::microsleep(1000);
  }
  handle->count = 1;
#endif
}

bool MutualExclusion::tryExclusiveLock() const
{
  MutexHandle* handle = toMutexHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  Profiler::WaitTask profile("MutualExclusion::tryExclusiveLock()", handle);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  BOOL result = TRUE;
  result = ::TryEnterCriticalSection(&handle->handle);
  return result;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_trylock(&handle->mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw MutualExclusionException(this);
  }
#else
  // assume single threaded
  if (handle->count) {
    return false;
  }
  handle->count = 1;
  return true;
#endif
}

void MutualExclusion::releaseLock() const
{
  Profiler::pushSignal("MutualExclusion::releaseLock()");

  MutexHandle* handle = toMutexHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  ::LeaveCriticalSection(&handle->handle);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_unlock(&handle->mutex);
  if (result == 0) {
    return;
  } else if (result == EPERM) {
    _throw MutualExclusionException("Thread does not own mutex.", this);
  } else {
    _throw MutualExclusionException(this);
  }
#else
  if (!handle->count) {
    _throw MutualExclusionException(this);
  }
  handle->count = 0;
#endif
}

MutualExclusion::~MutualExclusion()
{
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
