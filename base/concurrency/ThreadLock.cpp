/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ThreadLock.h"
#include <errno.h>

ThreadLock::ThreadLock() throw(Construct) {
#ifdef HAVE_PTHREAD_RWLOCK
  pthread_rwlockattr_t attributes;
  if (pthread_rwlockattr_init(&attributes) != 0) {
    throw Construct();
  }
  if (pthread_rwlockattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE) != 0) {
    // does this also work in a multiprocessor environment (still within the same process)?
    pthread_rwlockattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  if (pthread_rwlock_init(&lock, &attributes) != 0) {
    pthread_rwlockattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  pthread_rwlockattr_destroy(&attributes); // should never fail
#else
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    throw Construct();
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  if (pthread_mutex_init(&lock, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#endif
}

void ThreadLock::readLock() const throw(ThreadLock::Lock) {
#ifdef HAVE_PTHREAD_RWLOCK
  if (pthread_rwlock_rdlock(&lock) != 0) {
    throw ThreadLock::Lock();
  }
#else
  if (pthread_mutex_lock(&lock) != 0) {
    throw ThreadLock::Lock();
  }
#endif
}

bool ThreadLock::tryReadLock() const throw(ThreadLock::Lock) {
#ifdef HAVE_PTHREAD_RWLOCK
  int result = pthread_rwlock_tryrdlock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ThreadLock::Lock();
  }
#else
  int result = pthread_mutex_trylock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ThreadLock::Lock();
  }
#endif
}

void ThreadLock::writeLock() const throw(ThreadLock::Lock) {
#ifdef HAVE_PTHREAD_RWLOCK
  if (pthread_rwlock_wrlock(&lock) != 0) {
    throw ThreadLock::Lock();
  }
#else
  if (pthread_mutex_lock(&lock) != 0) {
    throw ThreadLock::Lock();
  }
#endif
}

bool ThreadLock::tryWriteLock() const throw(ThreadLock::Lock) {
#ifdef HAVE_PTHREAD_RWLOCK
  int result = pthread_rwlock_trywrlock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ThreadLock::Lock();
  }
#else
  int result = pthread_mutex_trylock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw ThreadLock::Lock();
  }
#endif
}

void ThreadLock::unlock() const throw(ThreadLock::Unlock) {
#ifdef HAVE_PTHREAD_RWLOCK
  if (pthread_rwlock_unlock(&lock) != 0) {
    throw ThreadLock::Unlock();
  }
  // Results are undefined if lock is not held by the executing thread. How do we fix this?
#else
  if (pthread_mutex_unlock(&lock) != 0) {
    throw ThreadLock::Unlock();
  }
#endif
}

ThreadLock::~ThreadLock() throw(Destruct) {
#ifdef HAVE_PTHREAD_RWLOCK
  if (pthread_rwlock_destroy(&lock) != 0) { // lets just hope that this doesn't fail
    throw Destruct();
  }
#else
  if (pthread_mutex_destroy(&lock) != 0) { // lets just hope that this doesn't fail
    throw Destruct();
  }
#endif
}
