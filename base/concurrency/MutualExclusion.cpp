/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/MutualExclusion.h>

#if !defined(__win32__)
  #include <errno.h>
#endif // __win32__

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MutualExclusion::MutualExclusion() throw(ResourceException) {
#if defined(__win32__)
  InitializeCriticalSection(&lock);
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
#if defined(__win32__)
  EnterCriticalSection(&lock);
#else // pthread
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
#if defined(__win32__)
  BOOL result;
  result = TryEnterCriticalSection(&lock);
  return result;
#else // pthread
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
#if defined(__win32__)
  LeaveCriticalSection(&lock);
#else // pthread
  if (pthread_mutex_unlock(&lock)) {
    throw MutualExclusionException();
  }
#endif
}

MutualExclusion::~MutualExclusion() throw(MutualExclusionException) {
#if defined(__win32__)
  DeleteCriticalSection(&lock);
#else // pthread
  if (pthread_mutex_destroy(&lock)) {
    throw MutualExclusionException();
  }
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
