/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Semaphore.h"

#ifndef __win32__
  #include <errno.h>
#endif // __win32__

Semaphore::Semaphore(unsigned int value = 0) throw(OutOfDomain, ResourceException) {
  if (value > MAXIMUM) {
    OutOfDomain();
  }
#ifdef __win32__
  if (!(semaphore = CreateSemaphore(NULL, value, MAXIMUM, NULL))) {
    throw ResourceException(__func__);
  }
#elif HAVE_PTHREAD_SEMAPHORE
  if (sem_init(&semaphore, 0, value) != 0) {
    throw ResourceException(__func__);
  }
#else
  this->value = value;

  pthread_mutexattr_t attributes = PTHREAD_MUTEX_ERRORCHECK;
  if (pthread_mutex_init(&mutex, &attributes)) {
    throw ResourceException(__func__);
  }

  if (pthread_cond_init(&condition, NULL)) {
    throw ResourceException(__func__);
  }
#endif
}

#ifndef __win32__
unsigned int Semaphore::getValue() const throw(SemaphoreException) {
#ifdef HAVE_PTHREAD_SEMAPHORE
  unsigned int value;
  if (sem_getvalue(&semaphore, &value)) { // value is not negative
    throw SemaphoreException(__func__);
  }
  return value;
#else
  unsigned int result;
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  result = value;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  return result;
#endif
}
#endif // __win32__

void Semaphore::post() throw(Overflow, SemaphoreException) {
#ifdef __win32__
  if (!ReleaseSemaphore(semaphore, 1, NULL)) {
    throw SemaphoreException(__func__);
  }
#elif HAVE_PTHREAD_SEMAPHORE
  if (sem_post(&semaphore) == ERANGE) { // otherwise sem_post returns successfully
    throw Overflow();
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  if (value == MAXIMUM) {
    if (pthread_mutex_unlock(&mutex)) {
      throw SemaphoreException(__func__);
    }
    throw Overflow();
  }
  value++;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  pthread_cond_signal(&condition); // we only need to signal one thread
#endif
}

void Semaphore::wait() throw(SemaphoreException) {
#ifdef __win32__
  if (WaitForSingleObject(semaphore, INFINITE) != WAIT_OBJECT_0) {
    throw SemaphoreException(__func__);
  }
#elif HAVE_PTHREAD_SEMAPHORE
  if (sem_wait(&semaphore)) {
    throw SemaphoreException(__func__);
  }
#else
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  while (value == 0) { // wait for resource to become available
    pthread_cond_wait(&condition, &mutex);
  }
  value--;
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException(__func__);
  }
#endif
}

bool Semaphore::tryWait() throw(SemaphoreException) {
#ifdef __win32__
  return WaitForSingleObject(semaphore, 0) == WAIT_OBJECT_0;
#elif HAVE_PTHREAD_SEMAPHORE
  return sem_trywait(&semaphore) == 0; // did we decrement?
#else
  bool result;
  if (pthread_mutex_lock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  if (result = value > 0) {
    value--;
  }
  if (pthread_mutex_unlock(&mutex)) {
    throw SemaphoreException(__func__);
  }
  return result;
#endif
}

Semaphore::~Semaphore() throw(SemaphoreException) {
#ifdef __win32__
  if (!CloseHandle(semaphore)) {
    throw SemaphoreException(__func__);
  }
#elif HAVE_PTHREAD_SEMAPHORE
  if (sem_destroy(&semaphore) != 0) {
    throw SemaphoreException(__func__);
  }
#else
  if (pthread_cond_destroy(&condition)) {
    throw SemaphoreException(__func__);
  }
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
#endif
}
