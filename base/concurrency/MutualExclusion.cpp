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
    InitializeCriticalSection(&criticalSection);
  } __except(STATUS_NO_MEMORY) {
    throw ResourceException("Unable to initialize MutualExclusion");
  }
#else // __win32__
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    throw ResourceException("Unable to initialize MutualExclusion");
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException("Unable to initialize MutualExclusion");
  }
  if (pthread_mutex_init(&mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException("Unable to initialize MutualExclusion");
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#endif // __win32__
}

void MutualExclusion::lock() const throw(Exception) {
#ifdef __win32__
  __try {
    EnterCriticalSection(&criticalSection);
  } __except(STATUS_INVALID_HANDLE) {
    throw Exception("Unable to acquire lock on MutualExclusion");
  }
#else
  int result = pthread_mutex_lock(&mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw Exception("Unable to acquire lock on MutualExclusion");
  }
#endif // __win32__
}

bool MutualExclusion::tryLock() const throw(Exception) {
#ifdef __win32__
  BOOL result;
  __try {
    result = TryEnterCriticalSection(&criticalSection);
  } __except(STATUS_INVALID_HANDLE) {
    throw Exception("Unable to attempt to acquire lock on MutualExclusion");
  }
  return result;
#else
  int result = pthread_mutex_trylock(&mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw Exception("Unable to attempt to acquire lock on MutualExclusion");
  }
#endif // __win32__
}

void MutualExclusion::unlock() const throw(Exception) {
#ifdef __win32__
  LeaveCriticalSection(&criticalSection);
#else
  int result = pthread_mutex_unlock(&mutex);
  if (result == 0) {
    return;
  } else if (result == EPERM) {
    throw Exception("Unable to unlock MutualExclusion");
  } else {
    throw Exception("Unable to unlock MutualExclusion");
  }
#endif // __win32__
}

MutualExclusion::~MutualExclusion() throw() {
#ifdef __win32__
  DeleteCriticalSection(&criticalSection);
#else
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif // __win32__
}
