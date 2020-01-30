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

#include <base/concurrency/Semaphore.h>
#include <base/ResourceHandle.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP)
namespace ntapi {
  
  typedef long NTSTATUS;
  typedef long KPRIORITY;

  inline bool succeeded(long status) noexcept
  {
    return status >= 0;
  }

  inline bool failed(long status) noexcept
  {
    return status < 0;
  }
  
  enum {SEMAPHORE_QUERY_INFORMATION_CLASS = 0};

  struct SemaphoreInformation { // semaphore query information
    unsigned int value;
    unsigned int maximum;
  };

  typedef NTSTATUS (__stdcall *PNtQuerySemaphore)(HANDLE, unsigned int /*INFOCLASS*/, SemaphoreInformation*, unsigned long, unsigned long*);

  template<class API>
  inline API getAddress(const char* identifier) noexcept
  {
    if (!identifier) {
      return nullptr;
    }
    HMODULE handle = ::GetModuleHandle(L"ntdll");
    return handle ? (API)::GetProcAddress(handle, identifier) : nullptr;
  }
};
#endif

#else // unix
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <errno.h>
#  if defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
#    include <semaphore.h>
#    include <limits.h>
#  elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) // not desired here
#    define _POSIX_THREADS 1
#endif
#  if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#    include <pthread.h>
#    define _COM_AZURE_DEV__BASE__PTHREAD
#  endif
#  endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class SemaphoreHandle : public ResourceHandle {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
  enum {QUERY_INFORMATION = 0}; // the query type for semaphore
  
  HANDLE semaphore = 0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  
  #if defined(SEM_VALUE_MAX)
    enum {MAXIMUM = SEM_VALUE_MAX};
  #elif defined(_POSIX_SEM_VALUE_MAX)
    enum {MAXIMUM = _POSIX_SEM_VALUE_MAX};
  #else
    enum {MAXIMUM = 32767}; // minimum value specified by POSIX standard
  #endif
  
  sem_t semaphore;
  
  inline SemaphoreHandle() noexcept
  {
    clear(semaphore);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  volatile int value = 0;
  pthread_cond_t condition;
  pthread_mutex_t mutex;

  inline SemaphoreHandle() noexcept
  {
    clear(condition);
    clear(mutex);
  }

  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
#else
  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};

  int count = 0;
#endif

  ~SemaphoreHandle()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (!::CloseHandle(semaphore)) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
    if (sem_destroy(&semaphore) != 0) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
    if (pthread_cond_destroy(&condition)) {
      Runtime::corruption(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION);
    }
    pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#else
    count = 0;
#endif
  }
};

namespace {

  inline SemaphoreHandle* toSemaphoreHandle(const AnyReference& handle)
  {
    if (handle) { // TAG: we do not want a dynamic cast - use static cast
      return handle.cast<SemaphoreHandle>().getValue();
    }
    return nullptr;
  }
}

unsigned int Semaphore::getMaximum() noexcept
{
  return SemaphoreHandle::MAXIMUM;
}

Semaphore::Semaphore(unsigned int value)
{
  Profiler::ResourceCreateTask profile("Semaphore::Semaphore()");

  if (!(value <= SemaphoreHandle::MAXIMUM)) {
    _throw OutOfDomain(this);
  }

  Reference<SemaphoreHandle> _handle = new SemaphoreHandle();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!(_handle->semaphore = ::CreateSemaphore(0, value, SemaphoreHandle::MAXIMUM, 0))) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (sem_init(&_handle->semaphore, 0, value)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  _handle->value = value;
  
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    _throw SemaphoreException(this);
  }
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__ZEPHYR)
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw SemaphoreException(this);
  }
#endif
  if (pthread_mutex_init(&_handle->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    _throw SemaphoreException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&_handle->condition, 0)) {
    pthread_mutex_destroy(&_handle->mutex); // lets just hope that this doesn't fail
    _throw SemaphoreException(this);
  }
#else
  // assume single threaded
#endif
  this->handle = _handle;
  profile.setHandle(*_handle);
}

int Semaphore::getValue() const
{
  // TAG: Profiler - add get value info
  
  SemaphoreHandle* handle = toSemaphoreHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP)
    static ntapi::PNtQuerySemaphore NtQuerySemaphore = nullptr; // 0~unsupported if resolved
    static bool isResolved = false;
    if (!isResolved) {
      NtQuerySemaphore = ntapi::getAddress<ntapi::PNtQuerySemaphore>("NtQuerySemaphore");
      isResolved = true;
    }
    if (NtQuerySemaphore) {
      ntapi::SemaphoreInformation information;
      if (ntapi::succeeded(NtQuerySemaphore(handle->semaphore,
                                            ntapi::SEMAPHORE_QUERY_INFORMATION_CLASS,
                                            &information,
                                            sizeof(information),
                                            0))) {
        return information.value;
      }
    }
    return -1; // not supported
  #else // win32 does not support this functionality
    #warning Semaphore::getValue() is not supported
    return -1; // not supported
  #endif
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  int value = 0;
  if (::sem_getvalue(handle->semaphore, &value)) { // value is not negative
    _throw SemaphoreException(this);
  }
  return value;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  unsigned int result = 0;
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  result = handle->value;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  return result;
#else
  return handle->count;
#endif
}

void Semaphore::post()
{
  Profiler::WaitTask profile("Semaphore::post()");

  SemaphoreHandle* handle = toSemaphoreHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::ReleaseSemaphore(handle->semaphore, 1, 0)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (::sem_post(handle->Semaphore) == ERANGE) { // otherwise sem_post returns successfully
    _throw Overflow(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  if ((unsigned int)handle->value == SemaphoreHandle::MAXIMUM) {
    if (pthread_mutex_unlock(&handle->mutex)) {
      _throw SemaphoreException(this);
    }
    _throw Overflow(this);
  }
  handle->value++;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  pthread_cond_signal(&handle->condition); // we only need to signal one thread
#else
  ++handle->count;
#endif
}

void Semaphore::wait() const
{
  Profiler::WaitTask profile("Semaphore::wait()");
  
  SemaphoreHandle* handle = toSemaphoreHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::WaitForSingleObject(handle->semaphore, INFINITE) != WAIT_OBJECT_0) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (::sem_wait(&handle->semaphore)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  while (handle->value == 0) { // wait for resource to become available
    pthread_cond_wait(&handle->condition, &handle->mutex);
  }
  handle->value--;
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
#else
  while (!handle->count) {
  }
  --handle->count;
#endif
}

bool Semaphore::tryWait() const
{
  Profiler::WaitTask profile("Semaphore::tryWait()");

  SemaphoreHandle* handle = toSemaphoreHandle(this->handle);
  if (!handle) {
    _throw NullPointer(this);
  }
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::WaitForSingleObject(handle->semaphore, 0) == WAIT_OBJECT_0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  return ::sem_trywait(handle->semaphore) == 0; // did we decrement?
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_mutex_lock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  bool result = handle->value > 0;
  if (result) {
    handle->value--;
  }
  if (pthread_mutex_unlock(&handle->mutex)) {
    _throw SemaphoreException(this);
  }
  return result;
#else
  if (handle->count) {
    --handle->count;
    return true;
  }
  return false;
#endif
}

Semaphore::~Semaphore()
{
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Semaphore) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/concurrency");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Semaphore semaphore;
    TEST_ASSERT(semaphore.getValue() == 0);
    semaphore.post();
    semaphore.post();
    TEST_ASSERT(semaphore.getValue() == 2);
    semaphore.wait();
    TEST_ASSERT(semaphore.getValue() == 1);
    TEST_ASSERT(semaphore.tryWait());
    TEST_ASSERT(semaphore.getValue() == 0);
  }
};

TEST_REGISTER(Semaphore);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
