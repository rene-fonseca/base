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
#include <base/concurrency/ReadWriteLock.h>
#include <base/concurrency/SpinLock.h>
#include <base/Base.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <pthread.h>
#  include <errno.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

class ReadWriteLockImpl {
private:

  unsigned int readers = 0;
  unsigned int writers = 0;
  SpinLock spinLock;
  CRITICAL_SECTION common;
  HANDLE blockReaders;
public:
  
  inline ReadWriteLockImpl() throw(ResourceException) {
    blockReaders = ::CreateEvent(0, TRUE, TRUE, 0); // allow shared locks initially
    bassert(blockReaders != 0, ResourceException(this));
    ::InitializeCriticalSection(&common);
  }

  inline void exclusiveLock() throw() {
    spinLock.exclusiveLock();
    if (writers++ == 0) {
      ::ResetEvent(blockReaders); // prevent shared locks
    }
    spinLock.releaseLock();
    ::EnterCriticalSection(&common); // wait for exclusive lock
  }

  inline bool tryExclusiveLock() throw() {
    // we do not want to relinquish execution context if we not required to do so
    spinLock.exclusiveLock();
    bool result = (::TryEnterCriticalSection(&common) != 0); // must not block
    if (result) {
      if (writers++ == 0) {
        ::ResetEvent(blockReaders); // prevent shared locks
      }
    }
    spinLock.releaseLock();
    return result;
  }
  
  inline void sharedLock() throw() {
    // we do not want to relinquish execution context if we not required to do so
    spinLock.exclusiveLock();
    while (true) {
      if (writers == 0) {
        // no writers are waiting (or have the lock)
        if (readers++ == 0) {
          // first reader must get the exclusive lock
          ::EnterCriticalSection(&common); // must not block
        }
        break;
      }
      spinLock.releaseLock();
      ::WaitForSingleObject(blockReaders, INFINITE); // block until all writers have completed
      spinLock.exclusiveLock();
      // writers could have reacquired the lock here
    }
    spinLock.releaseLock();
  }

  inline bool trySharedLock() throw() {
    // we do not want to relinquish execution context if we not required to do so
    bool result = false;
    spinLock.exclusiveLock();
    // writers have higher priority than readers and readers may already have the lock
    if (writers == 0) {
      result = true;
      if (readers++ == 0) {
        // first reader must get the exclusive lock
        ::EnterCriticalSection(&common); // must not block
      }
    }    
    spinLock.releaseLock();
    return result;
  }

  inline void releaseLock() throw() {
    spinLock.exclusiveLock();
    if (readers > 0) {
      // release shared lock
      if (--readers == 0) {
        ::LeaveCriticalSection(&common);
      }
    } else {
      // release exclusive lock
      ::LeaveCriticalSection(&common);
      BASSERT(writers > 0);
      if (--writers == 0) {
        ::SetEvent(blockReaders);
      }
    }
    BASSERT(::WaitForSingleObject(blockReaders, 0) == WAIT_OBJECT_0);
    spinLock.releaseLock();
  }
  
  inline ~ReadWriteLockImpl() throw() {
    BASSERT(::TryEnterCriticalSection(&common) != 0);
    ::DeleteCriticalSection(&common);
    ::CloseHandle(blockReaders);
    BASSERT((spinLock.tryExclusiveLock()) && (readers == 0) && (writers == 0));
  }
};

#endif // flavor

ReadWriteLock::ReadWriteLock() throw(ResourceException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  representation = new ReadWriteLockImpl();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  representation = new pthread_rwlock_t[1]; // TAG: needs automatic deletion on exception
  pthread_rwlockattr_t attributes;
  if (pthread_rwlockattr_init(&attributes) != 0) {
    delete[] static_cast<pthread_rwlock_t*>(representation);
    throw ResourceException(this);
  }
  if (pthread_rwlockattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE) != 0) {
    // TAG: does this also work in a multiprocessor environment (still within the same process)?
    pthread_rwlockattr_destroy(&attributes); // should never fail
    delete[] static_cast<pthread_rwlock_t*>(representation);
    throw ResourceException(this);
  }
  if (pthread_rwlock_init(static_cast<pthread_rwlock_t*>(representation), &attributes) != 0) {
    pthread_rwlockattr_destroy(&attributes); // should never fail
    delete[] static_cast<pthread_rwlock_t*>(representation);
    throw ResourceException(this);
  }
  pthread_rwlockattr_destroy(&attributes); // should never fail
#else
  representation = new pthread_mutex_t[1]; // TAG: needs automatic deletion on exception
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    delete[] static_cast<pthread_mutex_t*>(representation);
    throw ResourceException(this);
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
   #warning disabled selection of mutex type due to CYGWIN bug
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] static_cast<pthread_mutex_t*>(representation);
    throw ResourceException(this);
  }
#endif // cygwin temporary bug fix
  if (pthread_mutex_init(static_cast<pthread_mutex_t*>(representation), &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] static_cast<pthread_mutex_t*>(representation);
    throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#endif
}

void ReadWriteLock::exclusiveLock() const throw(ReadWriteLockException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static_cast<ReadWriteLockImpl*>(representation)->exclusiveLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_wrlock(static_cast<pthread_rwlock_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
#else
  int result = pthread_mutex_lock(static_cast<pthread_mutex_t*>(representation));
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw ReadWriteLockException(this);
  }
#endif
}

bool ReadWriteLock::tryExclusiveLock() const throw(ReadWriteLockException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return static_cast<ReadWriteLockImpl*>(representation)->tryExclusiveLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_trywrlock(static_cast<pthread_rwlock_t*>(representation));
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException(this);
  }
#else
  int result = pthread_mutex_trylock(static_cast<pthread_mutex_t*>(representation));
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException(this);
  }
#endif
}

void ReadWriteLock::sharedLock() const throw(ReadWriteLockException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static_cast<ReadWriteLockImpl*>(representation)->sharedLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_rdlock(static_cast<pthread_rwlock_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
#else
  int result = pthread_mutex_lock(static_cast<pthread_mutex_t*>(representation));
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw ReadWriteLockException(this);
  }
#endif
}

bool ReadWriteLock::trySharedLock() const throw(ReadWriteLockException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return static_cast<ReadWriteLockImpl*>(representation)->trySharedLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_tryrdlock(static_cast<pthread_rwlock_t*>(representation));
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException(this);
  }
#else
  int result = pthread_mutex_trylock(static_cast<pthread_mutex_t*>(representation));
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException(this);
  }
#endif
}

void ReadWriteLock::releaseLock() const throw(ReadWriteLockException) {
  // must be invoked by a thread which has already has a acquired an exclusive or shared lock!
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static_cast<ReadWriteLockImpl*>(representation)->releaseLock();
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_unlock(static_cast<pthread_rwlock_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
#else
  if (pthread_mutex_unlock(static_cast<pthread_mutex_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
#endif
}

ReadWriteLock::~ReadWriteLock() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  delete static_cast<ReadWriteLockImpl*>(representation);
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_destroy(static_cast<pthread_rwlock_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
  delete[] static_cast<pthread_rwlock_t*>(representation);
#else
  if (pthread_mutex_destroy(static_cast<pthread_mutex_t*>(representation))) {
    throw ReadWriteLockException(this);
  }
  delete[] static_cast<pthread_mutex_t*>(representation);
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
