/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/Event.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // pthread
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <pthread.h>
#  define _COM_AZURE_DEV__BASE__PTHREAD
#endif
#  include <sys/time.h>
#  include <unistd.h>
#  include <errno.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
template<class TYPE>
class HandlePointer {
private:

  TYPE* p = nullptr;

  HandlePointer(const HandlePointer&);
  HandlePointer(HandlePointer&&);
public:
  
  inline HandlePointer()
    : p(new TYPE[1])
  {
  }

  inline TYPE& operator*() noexcept
  {
    BASSERT(p);
    return *p;
  }

  inline const TYPE& operator*() const noexcept
  {
    BASSERT(p);
    return *p;
  }

  inline TYPE* operator->() noexcept
  {
    return p;
  }

  inline const TYPE* operator->() const noexcept
  {
    return p;
  }

  template<class POLY>
  inline const POLY* cast() const noexcept
  {
    return reinterpret_cast<POLY>(p);
  }

  inline TYPE* detach() noexcept
  {
    BASSERT(p);
    TYPE* result = p;
    p = nullptr;
    return result;
  }

  inline ~HandlePointer()
  {
    if (p) {
      delete[] p;
    }
  }
};

template<>
class HandlePointer<void> {
private:

  void* p = nullptr;
public:
  
  template<typename POLY>
  inline POLY* cast() noexcept
  {
    return p;
  }

  template<typename POLY>
  inline const POLY* cast() const noexcept
  {
    return p;
  }
  
  inline ~HandlePointer()
  {
    BASSERT(!p); // prevent destruction
  }
};

// need to convert between void* and internal pointer
// void* cannot be destructed
#endif

class EventImpl {
public:
  
#if defined(_COM_AZURE_DEV__BASE__PTHREAD)
  struct Context {
    bool signaled;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
  };
#endif
};

Event::Event()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if ((context = Cast::pointer<void*>(::CreateEvent(0, TRUE, FALSE, 0))) == 0) {
    _throw ResourceException("Unable to initialize event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  EventImpl::Context* context = new EventImpl::Context[1];
  context->signaled = false;

  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    delete[] context;
    _throw ResourceException(this);
  }
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#  warning disabled selection of mutex type due to CYGWIN bug
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] context;
    _throw ResourceException(this);
  }
#endif // TAG: cygwin temporary bug fix
  if (pthread_mutex_init(&Cast::pointer<EventImpl::Context*>(context)->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] context;
    _throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&Cast::pointer<EventImpl::Context*>(context)->condition, 0)) {
    pthread_mutex_destroy(&Cast::pointer<EventImpl::Context*>(context)->mutex);
    delete[] context;
    _throw ResourceException(this);
  }
  this->context = context;
#else
  BASSERT(!"Not supported.");
#endif
}

bool Event::isSignaled() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::WaitForSingleObject(context, 0) == WAIT_OBJECT_0; // should never fail
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  bool result = false;
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException(this);
  }
  result = Cast::pointer<EventImpl::Context*>(context)->signaled;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException(this);
  }
  return result;
#else
  return false;
#endif
}

void Event::reset()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::ResetEvent(context)) {
    _throw EventException("Unable to reset event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to reset event.", this);
  }
  Cast::pointer<EventImpl::Context*>(context)->signaled = false;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to reset event.", this);
  }
#else
  BASSERT(!"Not supported.");
#endif
}

void Event::signal()
{
  Profiler::pushSignal("Event::wait()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::SetEvent(context)) {
    _throw EventException("Unable to signal event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to signal event.", this);
  }
  Cast::pointer<EventImpl::Context*>(context)->signaled = true;
  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to signal event.", this);
  }
  if (pthread_cond_broadcast(&Cast::pointer<EventImpl::Context*>(context)->condition)) { // unblock all blocked threads
    _throw EventException("Unable to signal event.", this);
  }
#else
  BASSERT(!"Not supported.");
#endif
}

void Event::wait() const
{
  Profiler::WaitTask profile("Event::wait()");
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::WaitForSingleObject(context, INFINITE) != WAIT_OBJECT_0) {
    _throw EventException("Unable to wait for event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }

  while (!Cast::pointer<EventImpl::Context*>(context)->signaled) { // wait for signal
    pthread_cond_wait(
      &Cast::pointer<EventImpl::Context*>(context)->condition,
      &Cast::pointer<EventImpl::Context*>(context)->mutex
    );
  }

  if (pthread_mutex_unlock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }
#else
  BASSERT(!"Not supported.");
#endif
}

bool Event::wait(unsigned int microseconds) const
{
  Profiler::WaitTask profile("Event::wait()");
  
  if (microseconds > 1000000000) {
    _throw OutOfDomain(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  switch (::WaitForSingleObject(context, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    _throw EventException("Unable to wait for event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = true; // no assignment gives warning

  if (pthread_mutex_lock(&Cast::pointer<EventImpl::Context*>(context)->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }

  struct timespec absoluteTime;
  struct timeval now; // microsecond resolution
  gettimeofday(&now, 0);
  long long nanoseconds = (now.tv_usec + microseconds) * 1000ULL;
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
    _throw EventException("Unable to wait for event.", this);
  }

  return result;
#else
  return false;
#endif
}

Event::~Event()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::CloseHandle(context) == 0) {
    _throw EventException("Unable to destroy event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  EventImpl::Context* p = Cast::pointer<EventImpl::Context*>(context);
  if (pthread_cond_destroy(&p->condition)) {
    pthread_mutex_destroy(&p->mutex); // lets just hope that this doesn't fail
    delete[] p;
    _throw EventException("Unable to destroy event.", this);
  }
  pthread_mutex_destroy(&p->mutex); // lets just hope that this doesn't fail
  delete[] p;
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Event) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/concurrency");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Event e;
    TEST_ASSERT(!e.isSignaled());
    e.signal();
    e.wait();
    TEST_ASSERT(e.wait(1000000));
    e.reset();
    TEST_ASSERT(!e.isSignaled());
    TEST_ASSERT(!e.wait(1000));
  }
};

TEST_REGISTER(Event);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
