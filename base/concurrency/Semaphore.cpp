/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Semaphore.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Semaphore::Semaphore(unsigned int value = 0) throw(OutOfDomain, ResourceException) {
  if (value > MAXIMUM) {
    OutOfDomain();
  }
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!(semaphore = CreateSemaphore(0, value, MAXIMUM, 0))) {
    throw ResourceException();
  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  if (sem_init(&semaphore, 0, value)) {
    throw ResourceException();
  }
#else
  this->value = value;

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

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
unsigned int Semaphore::getValue() const throw(SemaphoreException) {
#if defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  int value;
  if (sem_getvalue(&semaphore, &value)) { // value is not negative
    throw SemaphoreException();
  }
  return value;
#else // mutual exclusion
  unsigned int result;
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException();
  }
  result = value;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException();
  }
  return result;
#endif
}
#endif

void Semaphore::post() throw(Overflow, SemaphoreException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!ReleaseSemaphore(semaphore, 1, NULL)) {
    throw SemaphoreException();
  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  if (sem_post(&semaphore) == ERANGE) { // otherwise sem_post returns successfully
    throw Overflow();
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException();
  }
  if ((unsigned int)value == MAXIMUM) {
    if (pthread_mutex_unlock(&mutex)) {
      throw SemaphoreException();
    }
    throw Overflow();
  }
  value++;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException();
  }
  pthread_cond_signal(&condition); // we only need to signal one thread
#endif
}

void Semaphore::wait() throw(SemaphoreException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (WaitForSingleObject(semaphore, INFINITE) != WAIT_OBJECT_0) {
    throw SemaphoreException();
  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  if (sem_wait(&semaphore)) {
    throw SemaphoreException();
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException();
  }
  while (value == 0) { // wait for resource to become available
    pthread_cond_wait(&condition, &mutex);
  }
  value--;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException();
  }
#endif
}

bool Semaphore::tryWait() throw(SemaphoreException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return WaitForSingleObject(semaphore, 0) == WAIT_OBJECT_0;
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  return sem_trywait(&semaphore) == 0; // did we decrement?
#else
  bool result;
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException();
  }
  if (result = value > 0) {
    value--;
  }
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException();
  }
  return result;
#endif
}

Semaphore::~Semaphore() throw(SemaphoreException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!CloseHandle(semaphore)) {
    throw SemaphoreException();
  }
#elif defined(_DK_SDU_MIP__BASE__PTHREAD_SEMAPHORE)
  if (sem_destroy(&semaphore) != 0) {
    throw SemaphoreException();
  }
#else
  if (pthread_cond_destroy(&condition)) {
    throw SemaphoreException();
  }
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
