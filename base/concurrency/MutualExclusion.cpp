/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/MutualExclusion.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <pthread.h>
  #include <errno.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MutualExclusion::MutualExclusion() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  mutex = new CRITICAL_SECTION[1];
  ::InitializeCriticalSection((CRITICAL_SECTION*)mutex);
#else // pthread
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    throw ResourceException(this);
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException(this);
  }
  if (pthread_mutex_init((pthread_mutex_t*)mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
#endif
}

void MutualExclusion::exclusiveLock() const throw(MutualExclusionException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::EnterCriticalSection((CRITICAL_SECTION*)mutex);
#else // pthread
  int result = pthread_mutex_lock((pthread_mutex_t*)mutex);
  if (result == 0) {
    return;
  } else if (result == EDEADLK) {
    return;
  } else {
    throw MutualExclusionException(this);
  }
#endif
}

bool MutualExclusion::tryExclusiveLock() const throw(MutualExclusionException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  BOOL result;
  result = ::TryEnterCriticalSection((CRITICAL_SECTION*)mutex);
  return result;
#else // pthread
  int result = pthread_mutex_trylock((pthread_mutex_t*)mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw MutualExclusionException(this);
  }
#endif
}

void MutualExclusion::releaseLock() const throw(MutualExclusionException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::LeaveCriticalSection((CRITICAL_SECTION*)mutex);
#else // pthread
  if (pthread_mutex_unlock((pthread_mutex_t*)mutex)) {
    throw MutualExclusionException(this);
  }
#endif
}

MutualExclusion::~MutualExclusion() throw(MutualExclusionException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ::DeleteCriticalSection((CRITICAL_SECTION*)mutex);
  delete[] (CRITICAL_SECTION*)mutex;
#else // pthread
  if (pthread_mutex_destroy((pthread_mutex_t*)mutex)) {
    throw MutualExclusionException(this);
  }
  delete[] (pthread_mutex_t*)mutex;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
