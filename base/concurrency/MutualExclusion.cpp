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

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MutualExclusion::MutualExclusion() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  InitializeCriticalSection(&lock);
#else // pthread
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  LeaveCriticalSection(&lock);
#else // pthread
  if (pthread_mutex_unlock(&lock)) {
    throw MutualExclusionException();
  }
#endif
}

MutualExclusion::~MutualExclusion() throw(MutualExclusionException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  DeleteCriticalSection(&lock);
#else // pthread
  if (pthread_mutex_destroy(&lock)) {
    throw MutualExclusionException();
  }
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
