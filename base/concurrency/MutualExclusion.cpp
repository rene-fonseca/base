/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "MutualExclusion.h"

#ifndef __win32__
  #include <errno.h>
#endif // __win32__

MutualExclusion::MutualExclusion() throw(ResourceException) {
#ifdef __win32__
  __try {
    InitializeCriticalSection(&lock);
  } __except(STATUS_NO_MEMORY) {
    throw ResourceException();
  }
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

void MutualExclusion::exclusiveLock() const throw(MutualExclusionException) {
#ifdef __win32__
  __try {
    EnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw MutualExclusionException();
  }
#else
  int result = pthread_mutex_lock(&lock);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw MutualExclusionException();
  }
#endif
}

bool MutualExclusion::tryExclusiveLock() const throw(MutualExclusionException) {
#ifdef __win32__
  BOOL result;
  __try {
    result = TryEnterCriticalSection(&lock);
  } __except(STATUS_INVALID_HANDLE) {
    throw MutualExclusionException();
  }
  return result;
#else
  int result = pthread_mutex_trylock(&lock);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw MutualExclusionException();
  }
#endif
}

void MutualExclusion::releaseLock() const throw(MutualExclusionException) {
#ifdef __win32__
  LeaveCriticalSection(&lock);
#else
  if (pthread_mutex_unlock(&lock)) {
    throw MutualExclusionException();
  }
#endif
}

MutualExclusion::~MutualExclusion() throw(MutualExclusionException) {
#ifdef __win32__
  DeleteCriticalSection(&lock);
#else
  if (pthread_mutex_destroy(&lock)) {
    throw MutualExclusionException();
  }
#endif
}
