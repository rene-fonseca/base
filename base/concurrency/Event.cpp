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
#include <base/concurrency/Event.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // pthread
  #include <pthread.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Event::Event() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if ((handle = ::CreateEvent(0, TRUE, FALSE, 0)) == 0) {
    throw ResourceException("Unable to initialize event", this);
  }
#else // pthread
  signaled = false;

  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    throw ResourceException(this);
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException(this);
  }
  if (pthread_mutex_init(&mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&condition, 0)) {
    throw ResourceException(this);
  }
#endif
}

bool Event::isSignaled() const throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return ::WaitForSingleObject(handle, 0) == WAIT_OBJECT_0; // should never fail
#else // pthread
  bool result;
  if (pthread_mutex_lock(&mutex)) {
    throw EventException(this);
  }
  result = signaled;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException(this);
  }
  return result;
#endif
}

void Event::reset() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::ResetEvent(handle)) {
    throw EventException("Unable to reset event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to reset event", this);
  }
  signaled = false;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to reset event", this);
  }
#endif
}

void Event::signal() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetEvent(handle)) {
    throw EventException("Unable to signal event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to signal event", this);
  }
  signaled = true;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to signal event", this);
  }
  if (pthread_cond_broadcast(&condition)) { // unblock all blocked threads
    throw EventException("Unable to signal event", this);
  }
#endif
}

void Event::wait() const throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0) {
    throw EventException("Unable to wait for event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  while (!signaled) { // wait for signal
    pthread_cond_wait(&condition, &mutex);
  }

  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to wait for event", this);
  }
#endif
}

bool Event::wait(unsigned int microseconds) const throw(OutOfDomain, EventException) {
  if (microseconds >= 1000000) {
    throw OutOfDomain(this);
  }
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  switch (::WaitForSingleObject(handle, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    throw EventException("Unable to wait for event", this);
  }
#else // pthread
  int result = true; // no assignment gives warning

  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  struct timespec absoluteTime;
  struct timeval now; // microsecond resolution
  gettimeofday(&now, 0);
  long long nanoseconds = (now.tv_usec + microseconds) * 1000;
  absoluteTime.tv_sec = now.tv_sec + nanoseconds/1000000000;
  absoluteTime.tv_nsec = nanoseconds % 1000000000;

  while (!signaled) { // wait for signal
    // concurrent envocations of signal() does not matter
    if (pthread_cond_timedwait(&condition, &mutex, &absoluteTime) == ETIMEDOUT) {
      result = false;
      break; // timed out
    }
  }

  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  return result;
#endif
}

Event::~Event() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (::CloseHandle(handle) == 0) {
    throw EventException("Unable to destroy event", this);
  }
#else // pthread
  if (pthread_cond_destroy(&condition)) {
    throw EventException("Unable to destroy event", this);
  }
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
