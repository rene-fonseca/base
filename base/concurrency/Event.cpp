/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/concurrency/Event.h>

#if !defined(__win32__)
  #include <pthread.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <errno.h>
#endif

Event::Event() throw(ResourceException) {
#if defined(__win32__)
  if ((event = CreateEvent(NULL, true, false, NULL)) == NULL) {
    throw ResourceException("Unable to initialize event");
  }
#else // pthread
  signaled = false;

  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    throw ResourceException();
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  if (pthread_mutex_init(&mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw ResourceException();
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&condition, NULL)) {
    throw ResourceException();
  }
#endif
}

bool Event::isSignaled() const throw(EventException) {
#if defined(__win32__)
  return WaitForSingleObject(event, 0) == WAIT_OBJECT_0; // should never fail
#else // pthread
  bool result;
  if (pthread_mutex_lock(&mutex)) {
    throw EventException(__func__);
  }
  result = signaled;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException(__func__);
  }
  return result;
#endif
}

void Event::reset() throw(EventException) {
#if defined(__win32__)
  if (!ResetEvent(event)) {
    throw EventException("Unable to reset event");
  }
#else // pthread
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to reset event");
  }
  signaled = false;
  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to reset event");
  }
#endif
}

void Event::signal() throw(EventException) {
#if defined(__win32__)
  if (!SetEvent(event)) {
    throw EventException("Unable to signal event");
  }
#else // pthread
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
#endif
}

void Event::wait() const throw(EventException) {
#if defined(__win32__)
  if (WaitForSingleObject(event, INFINITE) != WAIT_OBJECT_0) {
    throw EventException("Unable to wait for event");
  }
#else // pthread
  if (pthread_mutex_lock(&mutex)) {
    throw EventException("Unable to wait for event");
  }

  while (!signaled) { // wait for signal
    pthread_cond_wait(&condition, &mutex);
  }

  if (pthread_mutex_unlock(&mutex)) {
    throw EventException("Unable to wait for event");
  }
#endif
}

bool Event::wait(unsigned int microseconds) const throw(OutOfDomain, EventException) {
  if (microseconds >= 1000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  switch (WaitForSingleObject(event, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    throw EventException("Unable to wait for event");
  }
#else // pthread
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
#endif
}

Event::~Event() throw(EventException) {
#if defined(__win32__)
  if (!CloseHandle(event)) {
    throw EventException("Unable to destroy event");
  }
#else // pthread
  if (pthread_cond_destroy(&condition)) {
    throw EventException("Unable to destroy event");
  }
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif
}
