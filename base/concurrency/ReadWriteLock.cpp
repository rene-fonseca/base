/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ReadWriteLock.h"

#ifndef __win32__
  #include <errno.h>
#endif

ReadWriteLock::ReadWriteLock() throw(ResourceException) {
#ifdef __win32__
  __try {
    InitializeCriticalSection(&lock);
  } __except(STATUS_NO_MEMORY) {
    throw ResourceException();
  }
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  __try {
    EnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw ReadWriteLockException();
  }
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  BOOL result;
  __try {
    result = TryEnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw ReadWriteLockException();
  }
  return result;
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  __try {
    EnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw ReadWriteLockException();
  }
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  BOOL result;
  __try {
    result = TryEnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw ReadWriteLockException();
  }
  return result;
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  LeaveCriticalSection(&lock);
#elif HAVE_PTHREAD_RWLOCK
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
#ifdef __win32__
  DeleteCriticalSection(&lock);
#elif HAVE_PTHREAD_RWLOCK
  if (pthread_rwlock_destroy(&lock)) {
    throw ReadWriteLockException();
  }
#else
  if (pthread_mutex_destroy(&lock)) {
    throw ReadWriteLockException();
  }
#endif
}
