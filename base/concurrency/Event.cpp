/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Event.h"

#ifndef __win32__
  #include <pthread.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <errno.h>
#endif // __win32__

Event::Event() throw(ResourceException) {
#ifdef __win32__
  if ((event = CreateEvent(NULL, true, false, NULL)) == NULL) {
    throw ResourceException("Unable to initialize event");
  }
#else
  signaled = false;

  pthread_mutexattr_t attributes = PTHREAD_MUTEX_ERRORCHECK;
  if (pthread_mutex_init(&mutex, &attributes)) {
    throw ResourceException("Unable to initialize Event");
  }

  if (pthread_cond_init(&condition, NULL)) {
    throw ResourceException("Unable to initialize event");
  }
#endif // __win32__
}

bool Event::isSignaled() const throw(EventException) {
#ifdef __win32__
  return WaitForSingleObject(event, 0) == WAIT_OBJECT_0;
#else
  bool result;
  if (pthread_mutex_lock(&mutex)) {
    throw EventException(__func__);
  }
  result = signaled;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException(__func__);
  }
  return result;
#endif // __win32__
}

void Event::reset() throw(EventException) {
#ifdef __win32__
  if (ResetEvent(event)) {
    throw EventException("Unable to reset event");
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to reset event");
  }
  signaled = false;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to reset event");
  }
#endif // __win32__
}

void Event::signal() throw(EventException) {
#ifdef __win32__
  if (SetEvent(event)) {
    throw EventException("Unable to signal event");
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to signal event");
  }
  signaled = true;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to signal event");
  }
  if (pthread_cond_broadcast(&condition)) { // unblock all blocked threads
    throw EventException("Unable to signal event");
  }
#endif // __win32__
}

void Event::wait() const throw(EventException) {
#ifdef __win32__
  if (WaitForSingleObject(event, INFINITE) != WAIT_OBJECT_0) {
    throw EventException("Unable to wait for event");
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to wait for event");
  }

  while (!signaled) { // wait for signal
    pthread_cond_wait(&condition, &mutex);
  }

  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to wait for event");
  }
#endif // __win32__
}

bool Event::wait(unsigned int microseconds) const throw(OutOfDomain, EventException) {
  if (microseconds >= 1000000) {
    throw OutOfDomain();
  }
#ifdef __win32__
  switch (WaitForSingleObject(event, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    throw EventException("Unable to wait for event");
  }
#else
  int result = true; // no assignment gives warning

  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to wait for event");
  }

  struct timespec absoluteTime;
  struct timeval now; // microsecond resolution
  gettimeofday(&now, NULL);
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
    throw EventException("Unable to wait for event");
  }

  return result;
#endif // __win32__
}

Event::~Event() throw(EventException) {
#ifdef __win32__
  if (!CloseHandle(event)) {
    throw EventException("Unable to destroy event");
  }
#else
  if (pthread_cond_destroy(&condition)) {
    throw EventException("Unable to destroy event");
  }
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif // __win32__
}
