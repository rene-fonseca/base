/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "MutualExclusion.h"
#include <errno.h>

MutualExclusion::MutualExclusion() throw(Construct) {
  pthread_mutexattr_t attributes;
  if (pthread_mutexattr_init(&attributes) != 0) {
    throw Construct();
  }
  if (pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  if (pthread_mutex_init(&mutex, &attributes) != 0) {
    pthread_mutexattr_destroy(&attributes); // should never fail
    throw Construct();
  }
  pthread_mutexattr_destroy(&attributes); // should never fail
}

void MutualExclusion::lock() const throw(MutualExclusion::Lock) {
  if (pthread_mutex_lock(&mutex) != 0) {
    throw MutualExclusion::Lock();
  }
}

bool MutualExclusion::tryLock() const throw(MutualExclusion::Lock) {
  int result = pthread_mutex_trylock(&mutex);
  if (result == 0) {
    return true;
  } else if (result == EBUSY) {
    return false;
  } else {
    throw MutualExclusion::Lock();
  }
}

void MutualExclusion::unlock() const throw(MutualExclusion::Unlock) {
  if (pthread_mutex_unlock(&mutex) != 0) {
    throw MutualExclusion::Unlock();
  }
}

MutualExclusion::~MutualExclusion() throw() {
  pthread_mutex_destroy(&mutex); // lets just hope that this doesn't fail
}
