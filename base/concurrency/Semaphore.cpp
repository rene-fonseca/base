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
#include <base/concurrency/Semaphore.h>
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

  inline bool succeeded(long status) noexcept {
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
  inline API getAddress(const char* identifier) noexcept {
    return (API)::GetProcAddress(::GetModuleHandle(L"ntdll"), identifier);
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
#  if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#    include <pthread.h>
#    define _COM_AZURE_DEV__BASE__PTHREAD
#  endif
#  endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class SemaphoreImpl {
public:
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  typedef OperatingSystem::Handle Semaphore;

  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
  enum {QUERY_INFORMATION = 0}; // the query type for semaphore
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  typedef sem_t Semaphore;
  
  #if defined(SEM_VALUE_MAX)
    enum {MAXIMUM = SEM_VALUE_MAX};
  #elif defined(_POSIX_SEM_VALUE_MAX)
    enum {MAXIMUM = _POSIX_SEM_VALUE_MAX};
  #else
    enum {MAXIMUM = 32767}; // minimum value specified by POSIX standard
  #endif
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  struct Semaphore {
    volatile int value;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
  };

  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
#else
  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
#endif
};

unsigned int Semaphore::getMaximum() noexcept
{
  return SemaphoreImpl::MAXIMUM;
}

Semaphore::Semaphore(unsigned int value)
{
  if (!(value <= SemaphoreImpl::MAXIMUM)) {
    _throw OutOfDomain(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!(semaphore = (SemaphoreImpl::Semaphore)::CreateSemaphore(0, value, SemaphoreImpl::MAXIMUM, 0))) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (sizeof(sem_t) <= sizeof(void*)) {
    if (sem_init((sem_t*)&semaphore, 0, value)) {
      _throw SemaphoreException(this);
    }
  } else {
    semaphore = new sem_t[1];
    if (sem_init((sem_t*)semaphore, 0, value)) {
      delete[] (sem_t*)semaphore;
      _throw SemaphoreException(this);
    }
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  SemaphoreImpl::Semaphore* semaphore = new SemaphoreImpl::Semaphore[1];
  semaphore->value = value;
  
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    delete[] semaphore;
    _throw SemaphoreException(this);
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] semaphore;
    _throw SemaphoreException(this);
  }
  if (pthread_mutex_init(&semaphore->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] semaphore;
    _throw SemaphoreException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&(semaphore->condition), 0)) {
    pthread_mutex_destroy(&(semaphore->mutex)); // lets just hope that this doesn't fail
    delete[] semaphore;
    _throw SemaphoreException(this);
  }
  this->semaphore = semaphore;
#else
  BASSERT(!"Not supported.");
#endif
}

int Semaphore::getValue() const
{
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
      if (ntapi::succeeded(NtQuerySemaphore((HANDLE)semaphore,
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
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  if (::sem_getvalue(sem, &value)) { // value is not negative
    _throw SemaphoreException(this);
  }
  return value;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  unsigned int result = 0;
  if (pthread_mutex_lock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  result = sem->value;
  if (pthread_mutex_unlock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  return result;
#else
  BASSERT(!"Not supported.");
  return false;
#endif
}

void Semaphore::post()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::ReleaseSemaphore((HANDLE)semaphore, 1, 0)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  if (::sem_post(sem) == ERANGE) { // otherwise sem_post returns successfully
    _throw Overflow(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  if ((unsigned int)sem->value == SemaphoreImpl::MAXIMUM) {
    if (pthread_mutex_unlock(&(sem->mutex))) {
      _throw SemaphoreException(this);
    }
    _throw Overflow(this);
  }
  sem->value++;
  if (pthread_mutex_unlock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  pthread_cond_signal(&(sem->condition)); // we only need to signal one thread
#else
  BASSERT(!"Not supported.");
#endif
}

void Semaphore::wait() const
{
  Profiler::WaitTask profile("Semaphore::wait()");
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::WaitForSingleObject((HANDLE)semaphore, INFINITE) != WAIT_OBJECT_0) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  if (::sem_wait(sem)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  while (sem->value == 0) { // wait for resource to become available
    pthread_cond_wait(&(sem->condition), &(sem->mutex));
  }
  sem->value--;
  if (pthread_mutex_unlock(&sem->mutex)) {
    _throw SemaphoreException(this);
  }
#else
  BASSERT(!"Not supported.");
#endif
}

bool Semaphore::tryWait() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::WaitForSingleObject((HANDLE)semaphore, 0) == WAIT_OBJECT_0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  return ::sem_trywait(sem) == 0; // did we decrement?
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  bool result = sem->value > 0;
  if (result) {
    sem->value--;
  }
  if (pthread_mutex_unlock(&(sem->mutex))) {
    _throw SemaphoreException(this);
  }
  return result;
#else
  BASSERT(!"Not supported.");
  return false;
#endif
}

Semaphore::~Semaphore()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::CloseHandle((HANDLE)semaphore)) {
    _throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (sizeof(sem_t) <= sizeof(semaphore)) {
    if (sem_destroy((sem_t*)&semaphore) != 0) {
      _throw SemaphoreException(this);
    }
  } else {
    if (sem_destroy((sem_t*)semaphore) != 0) {
      _throw SemaphoreException(this);
    }
    delete[] (SemaphoreImpl::Semaphore*)semaphore;
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD)
  if (pthread_cond_destroy(&((SemaphoreImpl::Semaphore*)semaphore)->condition)) {
    _throw SemaphoreException(this);
  }
  pthread_mutex_destroy(&((SemaphoreImpl::Semaphore*)semaphore)->mutex); // lets just hope that this doesn't fail
  delete[] (SemaphoreImpl::Semaphore*)semaphore;
#else
  BASSERT(!"Not supported.");
#endif
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
