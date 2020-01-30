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

#include <base/concurrency/ReadWriteLock.h>
#include <base/concurrency/SpinLock.h>
#include <base/Base.h>
#include <base/ResourceHandle.h>
#include <base/Profiler.h>

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

class ReadWriteHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  unsigned int readers = 0;
  unsigned int writers = 0;
  SpinLock spinLock;
  CRITICAL_SECTION common;
  HANDLE blockReaders = 0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  pthread_rwlock_t lock;
  
  inline ReadWriteHandle() noexcept
  {
    clear(lock);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  pthread_mutex_t mutex;

  inline ReadWriteHandle() noexcept
  {
    clear(mutex);
  }
#else
  int count = 0;
#endif

  ~ReadWriteHandle()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    BASSERT(::TryEnterCriticalSection(&common) != 0);
    ::DeleteCriticalSection(&common);
    ::CloseHandle(blockReaders);
    BASSERT((spinLock.tryExclusiveLock()) && (readers == 0) && (writers == 0));
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
    if (pthread_rwlock_destroy(&lock)) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
    if (pthread_mutex_destroy(&mutex)) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#else
    count = 0;
#endif
  }
};

namespace {

  inline ReadWriteHandle* toReadWriteHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<ReadWriteHandle>().getValue();
    }
    return nullptr;
  }
}

ReadWriteLock::ReadWriteLock()
{
  Profiler::ResourceCreateTask profile("ReadWriteLock::ReadWriteLock()");

  Reference<ReadWriteHandle> _handle = new ReadWriteHandle();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  _handle->blockReaders = ::CreateEvent(0, TRUE, TRUE, 0); // allow shared locks initially
  if (_handle->blockReaders == 0) {
    _throw ResourceException(this);
  }
  ::InitializeCriticalSection(&_handle->common);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  pthread_rwlockattr_t attributes;
  if (pthread_rwlockattr_init(&attributes) != 0) {
    _throw ResourceException(this);
  }
  if (pthread_rwlockattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE) != 0) {
    // TAG: does this also work in a multiprocessor environment (still within the same process)?
    pthread_rwlockattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  if (pthread_rwlock_init(&_handle->lock, &attributes) != 0) {
    pthread_rwlockattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  pthread_rwlockattr_destroy(&attributes); // should never fail
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
}

void ReadWriteLock::exclusiveLock() const
{
  Profiler::WaitTask profile("ReadWriteLock::exclusiveLock()");
  
  ReadWriteHandle* handle = toReadWriteHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  handle->spinLock.exclusiveLock();
  if (handle->writers++ == 0) {
    ::ResetEvent(handle->blockReaders); // prevent shared locks
  }
  handle->spinLock.releaseLock();
  ::EnterCriticalSection(&handle->common); // wait for exclusive lock
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_wrlock(&handle->lock)) {
    _throw ReadWriteLockException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_lock(&handle->mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    _throw ReadWriteLockException(this);
  }
#else
  // assume single threaded
  while (handle->count) {
    Thread::microsleep(1000);
  }
  handle->count = 1;
#endif
}

bool ReadWriteLock::tryExclusiveLock() const
{
  Profiler::WaitTask profile("ReadWriteLock::tryExclusiveLock()");

  ReadWriteHandle* handle = toReadWriteHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // we do not want to relinquish execution context if we not required to do so
  handle->spinLock.exclusiveLock();
  bool result = (::TryEnterCriticalSection(&handle->common) != 0); // must not block
  if (result) {
    if (handle->writers++ == 0) {
      ::ResetEvent(handle->blockReaders); // prevent shared locks
    }
  }
  handle->spinLock.releaseLock();
  return result;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_trywrlock(&handle->mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw ReadWriteLockException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_trylock(&handle->mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw ReadWriteLockException(this);
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

void ReadWriteLock::sharedLock() const
{
  Profiler::WaitTask profile("ReadWriteLock::sharedLock()");
  
  ReadWriteHandle* handle = toReadWriteHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // we do not want to relinquish execution context if we not required to do so
  handle->spinLock.exclusiveLock();
  while (true) {
    if (handle->writers == 0) {
      // no writers are waiting (or have the lock)
      if (handle->readers++ == 0) {
        // first reader must get the exclusive lock
        ::EnterCriticalSection(&handle->common); // must not block
      }
      break;
    }
    handle->spinLock.releaseLock();
    ::WaitForSingleObject(handle->blockReaders, INFINITE); // block until all writers have completed
    handle->spinLock.exclusiveLock();
    // writers could have reacquired the lock here
  }
  handle->spinLock.releaseLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_rdlock(&handle->lock)) {
    _throw ReadWriteLockException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_lock(&handle->mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    _throw ReadWriteLockException(this);
  }
#else
  exclusiveLock();
#endif
}

bool ReadWriteLock::trySharedLock() const
{
  Profiler::WaitTask profile("ReadWriteLock::trySharedLock()");
  
  ReadWriteHandle* handle = toReadWriteHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // we do not want to relinquish execution context if we not required to do so
  bool result = false;
  handle->spinLock.exclusiveLock();
  // writers have higher priority than readers and readers may already have the lock
  if (handle->writers == 0) {
    result = true;
    if (handle->readers++ == 0) {
      // first reader must get the exclusive lock
      ::EnterCriticalSection(&handle->common); // must not block
    }
  }
  handle->spinLock.releaseLock();
  return result;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_tryrdlock(&handle->lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw ReadWriteLockException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = pthread_mutex_trylock(&handle->mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    _throw ReadWriteLockException(this);
  }
#else
  return tryExclusiveLock();
#endif
}

void ReadWriteLock::releaseLock() const
{
  Profiler::pushSignal("ReadWriteLock::releaseLock()");
  
  ReadWriteHandle* handle = toReadWriteHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  // must be invoked by a thread which has already has a acquired an exclusive or shared lock!
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  handle->spinLock.exclusiveLock();
  if (handle->readers > 0) {
    // release shared lock
    if (--handle->readers == 0) {
      ::LeaveCriticalSection(&handle->common);
    }
  } else {
    // release exclusive lock
    ::LeaveCriticalSection(&handle->common);
    BASSERT(handle->writers > 0);
    if (--handle->writers == 0) {
      ::SetEvent(handle->blockReaders);
    }
  }
  BASSERT(::WaitForSingleObject(handle->blockReaders, 0) == WAIT_OBJECT_0);
  handle->spinLock.releaseLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_unlock(&handle->lock)) {
    _throw ReadWriteLockException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw ReadWriteLockException(this);
  }
#else
  if (!handle->count) {
    _throw ReadWriteLockException(this);
  }
  handle->count = 0;
#endif
}

ReadWriteLock::~ReadWriteLock()
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
