/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Semaphore.h"
#include <errno.h>

Semaphore::Semaphore(unsigned int value = 0) throw(Construct) {
#ifdef HAVE_PTHREAD_SEMAPHORE
  if (sem_init(&semaphore, 0, value) != 0) {
    throw Construct();
  }
#else
  this->value = value;
  if (value > (unsigned int)getMaximum()) {
    throw Construct();
  }
  pthread_condattr_t attributes;
  if (pthread_condattr_init(&attributes) != 0) {
    throw Construct();
  }
  if (pthread_cond_init(&condition, &attributes) != 0) {
    pthread_condattr_destroy(&attributes); // this should never fail
    throw Construct();
  }
  pthread_condattr_destroy(&attributes); // this should never fail
#endif
}

int Semaphore::getMaximum() const throw() {
  return MAXIMUM;
}

int Semaphore::getValue() const throw() {
#ifdef HAVE_PTHREAD_SEMAPHORE
  int value;
  sem_getvalue(&semaphore, &value); // should never fail
  return value;
#else
  int result;
  MutualExclusion::lock(); // does not throw exception when used correctly
  result = value;
  MutualExclusion::unlock(); // does not throw exception when used correctly
  return result;
#endif
}

void Semaphore::post() throw(Overflow) {
#ifdef HAVE_PTHREAD_SEMAPHORE
  if (sem_post(&semaphore) == ERANGE) { // otherwise sem_post returns successfully
    throw Overflow();
  }
#else
  MutualExclusion::lock(); // does not throw exception when used correctly
  if (value == MAXIMUM) {
    MutualExclusion::unlock(); // does not throw exception when used correctly
    throw Overflow();
  }
  value++;
  MutualExclusion::unlock(); // does not throw exception when used correctly
  pthread_cond_signal(&condition); // we only need to signal one thread
#endif
}

void Semaphore::wait() throw() {
#ifdef HAVE_PTHREAD_SEMAPHORE
  sem_wait(&semaphore);
#else
  MutualExclusion::lock(); // does not throw exception when used correctly
  while (value == 0) { // wait for resource
    pthread_cond_wait(&condition, &mutex);
  }
  value--;
  MutualExclusion::unlock(); // does not throw exception when used correctly
#endif
}

bool Semaphore::tryWait() throw() {
#ifdef HAVE_PTHREAD_SEMAPHORE
  return sem_trywait(&semaphore) == 0; // did we decrement?
#else
  bool result;
  MutualExclusion::lock(); // does not throw exception when used correctly
  result = value > 0;
  if (result) {
    value--;
  }
  MutualExclusion::unlock();  // does not throw exception when used correctly
  return result;
#endif
}

Semaphore::~Semaphore() throw(Destruct) {
#ifdef HAVE_PTHREAD_SEMAPHORE
  if (sem_destroy(&semaphore) != 0) { // possible resource leak - semaphore could be busy
    throw Destruct();
  }
#else
  if (pthread_cond_destroy(&condition)) { // possible resource leak - condition could be busy
    throw Destruct();
  }
#endif
}
