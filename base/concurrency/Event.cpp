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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) // not desired here
#  undef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
#endif

#include <base/concurrency/Event.h>
#include <base/ResourceHandle.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // pthread
#undef _POSIX_THREADS
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) // not desired here
#  define _POSIX_THREADS 1
#endif
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

const String& Resource::getDescription() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->description;
}

void Resource::setDescription(const String& description)
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  handle->description = description;
}

unsigned int Resource::getResourceId() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->resourceId;
}

unsigned int Resource::getCreatedById() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->createdById;
}

class EventHandle : public ResourceHandle {
public:

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE handle = 0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  pthread_cond_t condition;
  pthread_mutex_t mutex;
  bool signaled = false;

  inline EventHandle() noexcept
  {
    clear(condition);
    clear(mutex);
  }
#else
  bool signaled = false;
#endif

  ~EventHandle()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (::CloseHandle(handle) == 0) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
    handle = 0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
    if (pthread_cond_destroy(&condition)) {
      pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
    pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#else
    signaled = false;
#endif
  }
};

namespace {

  inline EventHandle* toEventHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<EventHandle>().getValue();
    }
    return nullptr;
  }
}

Event::Event()
{
  Profiler::ResourceCreateTask profile("Event::Event()");

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE context = ::CreateEvent(0, TRUE, FALSE, 0);
  if (!context) {
    _throw ResourceException("Unable to initialize event.", this);
  }
  Reference<EventHandle> _handle = new EventHandle();
  _handle->handle = context;
  this->handle = _handle;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    _throw ResourceException(this);
  }

  Reference<EventHandle> _handle = new EventHandle();

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
#  warning disabled selection of mutex type due to CYGWIN bug
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
#else
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
#endif // TAG: cygwin temporary bug fix

  if (pthread_mutex_init(&_handle->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw ResourceException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&_handle->condition, 0)) {
    pthread_mutex_destroy(&_handle->mutex);
    _throw ResourceException(this);
  }
  this->handle = _handle;
#else
  Reference<EventHandle> _handle = new EventHandle();
  this->handle = _handle;
#endif
  profile.setHandle(*_handle);
}

OperatingSystem::Handle Event::getHandle() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (EventHandle* handle = toEventHandle(this->handle)) {
    return handle->handle;
  }
  return nullptr;
#else
  return 0;
#endif
}

bool Event::isSignaled() const
{
  EventHandle* handle = toEventHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::WaitForSingleObject(handle->handle, 0) == WAIT_OBJECT_0; // should never fail
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  bool result = false;
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw EventException(this);
  }
  result = handle->signaled;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw EventException(this);
  }
  return result;
#else
  return handle->signaled;
#endif
}

void Event::reset()
{
  EventHandle* handle = toEventHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::ResetEvent(handle->handle)) {
    _throw EventException("Unable to reset event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw EventException("Unable to reset event.", this);
  }
  handle->signaled = false;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw EventException("Unable to reset event.", this);
  }
#else
  handle->signaled = false;
#endif
}

void Event::signal()
{
  Profiler::pushSignal("Event::signal()");

  EventHandle* handle = toEventHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::SetEvent(handle->handle)) {
    _throw EventException("Unable to signal event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw EventException("Unable to signal event.", this);
  }
  handle->signaled = true;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw EventException("Unable to signal event.", this);
  }
  if (pthread_cond_broadcast(&handle->condition)) { // unblock all blocked threads
    _throw EventException("Unable to signal event.", this);
  }
#else
  handle->signaled = true;
#endif
}

void Event::wait() const
{
  EventHandle* handle = toEventHandle(this->handle);
  Profiler::WaitTask profile("Event::wait()", handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::WaitForSingleObject(handle->handle, INFINITE) != WAIT_OBJECT_0) {
    _throw EventException("Unable to wait for event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }

  while (!handle->signaled) { // wait for signal
    int status = pthread_cond_wait(&handle->condition, &handle->mutex);
    // EINTR not expected
    if (status != 0) {
      pthread_mutex_unlock(&handle->mutex);
      _throw EventException("Unable to wait for event.", this);
    }
  }

  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }
#else
  while (!handle->signaled) {
    Thread::microsleep(1000);
  }
#endif
}

bool Event::wait(unsigned int microseconds) const
{
  EventHandle* handle = toEventHandle(this->handle);
  Profiler::WaitTask profile("Event::wait()", handle);
  if (!handle) {
    _throw NullPointer(this);
  }

  if (microseconds > 1000000000) {
    _throw OutOfDomain(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  switch (::WaitForSingleObject(handle->handle, microseconds/1000)) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  default:
    _throw EventException("Unable to wait for event.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  int result = true; // no assignment gives warning

  if (pthread_mutex_lock(&handle->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }

  struct timespec absoluteTime;
  struct timeval now; // microsecond resolution
  gettimeofday(&now, 0);
  long long nanoseconds = (now.tv_usec + microseconds) * 1000ULL;
  absoluteTime.tv_sec = now.tv_sec + nanoseconds/1000000000;
  absoluteTime.tv_nsec = nanoseconds % 1000000000;

  while (!handle->signaled) { // wait for signal
    // concurrent envocations of signal() does not matter
    // TAG: fails for sparc64 - setup_rt_frame: not implemented
    if (pthread_cond_timedwait(
          &handle->condition,
          &handle->mutex, &absoluteTime
        ) == ETIMEDOUT) {
      result = false;
      break; // timed out
    }
  }

  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw EventException("Unable to wait for event.", this);
  }

  return result;
#else
  if (!handle->signaled) {
    Thread::microsleep(microseconds);
  }
  return handle->signaled;
#endif
}

Event::~Event()
{
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
#if !defined(__clang_analyzer__)
    Event e;
    TEST_ASSERT(!e.isSignaled());
    e.signal();
    e.wait();
    TEST_ASSERT(e.wait(1000000));
    e.reset();
    TEST_ASSERT(!e.isSignaled());
#if (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC64)
    TEST_ASSERT(!e.wait(1000));
#endif
#endif
  }
};

TEST_REGISTER(Event);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
