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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // pthread
#  include <pthread.h>
#  include <sys/time.h>
#  include <unistd.h>
#  include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class EventImpl {
public:
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
  struct Context {
    bool signaled;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
  };
#endif
};

// TAG: why "throw(Event::EventException)" and not just "throw(EventException)"
Event::Event() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if ((context = Cast::pointer<void*>(::CreateEvent(0, TRUE, FALSE, 0))) == 0) {
    throw ResourceException("Unable to initialize event", this);
  }
#else // pthread
  EventImpl::Context* context = new EventImpl::Context[1];
  context->signaled = false;

  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    delete[] context;
    throw ResourceException(this);
  }
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
#  warning disabled selection of mutex type due to CYGWIN bug
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] context;
    throw ResourceException(this);
  }
#endif // TAG: cygwin temporary bug fix
  if (pthread_mutex_init(&Cast::pointer<EventImpl::Context*>(context)->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] context;
    throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&Cast::pointer<EventImpl::Context*>(context)->condition, 0)) {
    pthread_mutex_destroy(&Cast::pointer<EventImpl::Context*>(context)->mutex);
    delete[] context;
    throw ResourceException(this);
  }
  this->context = context;
#endif
}

bool Event::isSignaled() const throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return ::WaitForSingleObject(context, 0) == WAIT_OBJECT_0; // should never fail
#else // pthread
  bool result;
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException(this);
  }
  result = Cast::pointer<EventImpl::Context*>(context)->signaled;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException(this);
  }
  return result;
#endif
}

void Event::reset() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::ResetEvent(context)) {
    throw EventException("Unable to reset event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to reset event", this);
  }
  Cast::pointer<EventImpl::Context*>(context)->signaled = false;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to reset event", this);
  }
#endif
}

void Event::signal() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetEvent(context)) {
    throw EventException("Unable to signal event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to signal event", this);
  }
  Cast::pointer<EventImpl::Context*>(context)->signaled = true;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to signal event", this);
  }
  if (pthread_cond_broadcast(&Cast::pointer<EventImpl::Context*>(context)->condition)) { // unblock all blocked threads
    throw EventException("Unable to signal event", this);
  }
#endif
}

void Event::wait() const throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::WaitForSingleObject(context, INFINITE) != WAIT_OBJECT_0) {
    throw EventException("Unable to wait for event", this);
  }
#else // pthread
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  while (!Cast::pointer<EventImpl::Context*>(context)->signaled) { // wait for signal
    pthread_cond_wait(
      &Cast::pointer<EventImpl::Context*>(context)->condition,
      &Cast::pointer<EventImpl::Context*>(context)->mutex
    );
  }

  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to wait for event", this);
  }
#endif
}

bool Event::wait(unsigned int microseconds) const throw(OutOfDomain, EventException) {
  if (microseconds >= 1000000) {
    throw OutOfDomain(this);
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  switch (::WaitForSingleObject(context, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    throw EventException("Unable to wait for event", this);
  }
#else // pthread
  int result = true; // no assignment gives warning

  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  struct timespec absoluteTime;
  struct timeval now; // microsecond resolution
  gettimeofday(&now, 0);
  long long nanoseconds = (now.tv_usec + microseconds) * 1000;
  absoluteTime.tv_sec = now.tv_sec + nanoseconds/1000000000;
  absoluteTime.tv_nsec = nanoseconds % 1000000000;

  while (!Cast::pointer<EventImpl::Context*>(context)->signaled) { // wait for signal
    // concurrent envocations of signal() does not matter
    if (pthread_cond_timedwait(
          &Cast::pointer<EventImpl::Context*>(context)->condition,
          &Cast::pointer<EventImpl::Context*>(context)->mutex, &absoluteTime
        ) == ETIMEDOUT) {
      result = false;
      break; // timed out
    }
  }

  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    throw EventException("Unable to wait for event", this);
  }

  return result;
#endif
}

Event::~Event() throw(EventException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (::CloseHandle(context) == 0) {
    throw EventException("Unable to destroy event", this);
  }
#else // pthread
  if (pthread_cond_destroy(&Cast::pointer<EventImpl::Context*>(context)->condition)) {
    throw EventException("Unable to destroy event", this);
  }
  pthread_mutex_destroy(&Cast::pointer<EventImpl::Context*>(context)->mutex); // lets just hope that this doesn't fail
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
