/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/concurrency/ReadWriteLock.h>

#if !defined(__win32__)
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ReadWriteLock::ReadWriteLock() throw(ResourceException) {
#if defined(__win32__)
  //  __try {
    InitializeCriticalSection(&lock);
    //  } __except(STATUS_NO_MEMORY) {
    // throw ResourceException();
    //  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  pthread_rwlockattr_t attributes;
  if (pthread_rwlockattr_init(&attributes) != 0) {
    throw ResourceException();
  }
  if (pthread_rwlockattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE) != 0) {
    // does this also work in a multiprocessor environment (still within the same process)?
    pthread_rwlockattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  if (pthread_rwlock_init(&lock, &attributes) != 0) {
    pthread_rwlockattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  pthread_rwlockattr_destroy(&attributes); // should never fail
#else
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    throw ResourceException();
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  if (pthread_mutex_init(&lock, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#endif
}

void ReadWriteLock::exclusiveLock() const throw(ReadWriteLockException) {
#if defined(__win32__)
  //  __try {
    EnterCriticalSection(&lock);
    //  } __except(STATUS_INVALID_HANDLE) {
    //    throw ReadWriteLockException();
    //  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_wrlock(&lock)) {
    throw ReadWriteLockException();
  }
#else
  int result = pthread_mutex_lock(&lock);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw ReadWriteLockException();
  }
#endif
}

bool ReadWriteLock::tryExclusiveLock() const throw(ReadWriteLockException) {
#if defined(__win32__)
  BOOL result;
  //  __try {
    result = TryEnterCriticalSection(&lock);
    //  } __except(STATUS_INVALID_HANDLE) {
    //    throw ReadWriteLockException();
    //  }
  return result;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_trywrlock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException();
  }
#else
  int result = pthread_mutex_trylock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException();
  }
#endif
}

void ReadWriteLock::sharedLock() const throw(ReadWriteLockException) {
#if defined(__win32__)
  //  __try {
    EnterCriticalSection(&lock);
    //  } __except(STATUS_INVALID_HANDLE) {
    //    throw ReadWriteLockException();
    //  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_rdlock(&lock)) {
    throw ReadWriteLockException();
  }
#else
  int result = pthread_mutex_lock(&lock);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw ReadWriteLockException();
  }
#endif
}

bool ReadWriteLock::trySharedLock() const throw(ReadWriteLockException) {
#if defined(__win32__)
  BOOL result;
  //  __try {
    result = TryEnterCriticalSection(&lock);
    //  } __except(STATUS_INVALID_HANDLE) {
    //    throw ReadWriteLockException();
    //  }
  return result;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  int result = pthread_rwlock_tryrdlock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException();
  }
#else
  int result = pthread_mutex_trylock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ReadWriteLockException();
  }
#endif
}

void ReadWriteLock::releaseLock() const throw(ReadWriteLockException) {
#if defined(__win32__)
  LeaveCriticalSection(&lock);
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_unlock(&lock)) {
    throw ReadWriteLockException();
  }
#else
  if (pthread_mutex_unlock(&lock)) {
    throw ReadWriteLockException();
  }
#endif
}

ReadWriteLock::~ReadWriteLock() throw(ReadWriteLockException) {
#if defined(__win32__)
  DeleteCriticalSection(&lock);
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_RWLOCK)
  if (pthread_rwlock_destroy(&lock)) {
    throw ReadWriteLockException();
  }
#else
  if (pthread_mutex_destroy(&lock)) {
    throw ReadWriteLockException();
  }
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
