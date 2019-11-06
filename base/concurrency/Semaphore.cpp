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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP)
namespace ntapi {
  
  typedef long NTSTATUS;
  typedef long KPRIORITY;

  inline bool succeeded(long status) throw() {
    return status >= 0;
  }

  inline bool failed(long status) throw() {
    return status < 0;
  }
  
  enum {SEMAPHORE_QUERY_INFORMATION_CLASS = 0};

  struct SemaphoreInformation { // semaphore query information
    unsigned int value;
    unsigned int maximum;
  };

  typedef NTSTATUS (__stdcall *PNtQuerySemaphore)(HANDLE, unsigned int /*INFOCLASS*/, SemaphoreInformation*, unsigned long, unsigned long*);

  template<class API>
  inline API getAddress(const char* identifier) throw() {
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
#    include <pthread.h>
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
#else
  struct Semaphore {
    volatile int value;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
  };

  enum {MAXIMUM = PrimitiveTraits<int>::MAXIMUM};
#endif
};

unsigned int Semaphore::getMaximum() throw() {
  return SemaphoreImpl::MAXIMUM;
}

Semaphore::Semaphore(unsigned int value) throw(OutOfDomain, SemaphoreException)
{
  if (!(value <= SemaphoreImpl::MAXIMUM)) {
    throw OutOfDomain(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!(semaphore = (SemaphoreImpl::Semaphore)::CreateSemaphore(0, value, SemaphoreImpl::MAXIMUM, 0))) {
    throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (sizeof(sem_t) <= sizeof(void*)) {
    if (sem_init((sem_t*)&semaphore, 0, value)) {
      throw SemaphoreException(this);
    }
  } else {
    semaphore = new sem_t[1];
    if (sem_init((sem_t*)semaphore, 0, value)) {
      delete[] (sem_t*)semaphore;
      throw SemaphoreException(this);
    }
  }
#else
  SemaphoreImpl::Semaphore* semaphore = new SemaphoreImpl::Semaphore[1];
  semaphore->value = value;
  
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes)) {
    delete[] semaphore;
    throw SemaphoreException(this);
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] semaphore;
    throw SemaphoreException(this);
  }
  if (pthread_mutex_init(&semaphore->mutex, &attributes)) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    delete[] semaphore;
    throw SemaphoreException(this);
  }
  pthread_mutexattr_destroy(&attributes); // should never fail

  if (pthread_cond_init(&(semaphore->condition), 0)) {
    pthread_mutex_destroy(&(semaphore->mutex)); // lets just hope that this doesn't fail
    delete[] semaphore;
    throw SemaphoreException(this);
  }
  this->semaphore = semaphore;
#endif
}

int Semaphore::getValue() const throw(SemaphoreException) {
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
    throw SemaphoreException(this);
  }
  return value;
#else // mutual exclusion
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  unsigned int result = 0;
  if (pthread_mutex_lock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  result = sem->value;
  if (pthread_mutex_unlock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  return result;
#endif
}

void Semaphore::post() throw(Overflow, SemaphoreException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::ReleaseSemaphore((HANDLE)semaphore, 1, 0)) {
    throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  if (::sem_post(sem) == ERANGE) { // otherwise sem_post returns successfully
    throw Overflow(this);
  }
#else
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  if ((unsigned int)sem->value == SemaphoreImpl::MAXIMUM) {
    if (pthread_mutex_unlock(&(sem->mutex))) {
      throw SemaphoreException(this);
    }
    throw Overflow(this);
  }
  sem->value++;
  if (pthread_mutex_unlock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  pthread_cond_signal(&(sem->condition)); // we only need to signal one thread
#endif
}

void Semaphore::wait() const throw(SemaphoreException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (::WaitForSingleObject((HANDLE)semaphore, INFINITE) != WAIT_OBJECT_0) {
    throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  if (::sem_wait(sem)) {
    throw SemaphoreException(this);
  }
#else
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  while (sem->value == 0) { // wait for resource to become available
    pthread_cond_wait(&(sem->condition), &(sem->mutex));
  }
  sem->value--;
  if (pthread_mutex_unlock(&sem->mutex)) {
    throw SemaphoreException(this);
  }
#endif
}

bool Semaphore::tryWait() const throw(SemaphoreException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::WaitForSingleObject((HANDLE)semaphore, 0) == WAIT_OBJECT_0;
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  sem_t* sem = (sizeof(sem_t) <= sizeof(void*)) ? (sem_t*)&semaphore : (sem_t*)semaphore;
  return ::sem_trywait(sem) == 0; // did we decrement?
#else
  SemaphoreImpl::Semaphore* sem = (SemaphoreImpl::Semaphore*)semaphore;
  if (pthread_mutex_lock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  bool result = sem->value > 0;
  if (result) {
    sem->value--;
  }
  if (pthread_mutex_unlock(&(sem->mutex))) {
    throw SemaphoreException(this);
  }
  return result;
#endif
}

Semaphore::~Semaphore() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::CloseHandle((HANDLE)semaphore)) {
    throw SemaphoreException(this);
  }
#elif defined(_COM_AZURE_DEV__BASE__PTHREAD_SEMAPHORE)
  if (sizeof(sem_t) <= sizeof(semaphore)) {
    if (sem_destroy((sem_t*)&semaphore) != 0) {
      throw SemaphoreException(this);
    }
  } else {
    if (sem_destroy((sem_t*)semaphore) != 0) {
      throw SemaphoreException(this);
    }
    delete[] (SemaphoreImpl::Semaphore*)semaphore;
  }
#else
  if (pthread_cond_destroy(&((SemaphoreImpl::Semaphore*)semaphore)->condition)) {
    throw SemaphoreException(this);
  }
  pthread_mutex_destroy(&((SemaphoreImpl::Semaphore*)semaphore)->mutex); // lets just hope that this doesn't fail
  delete[] (SemaphoreImpl::Semaphore*)semaphore;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
