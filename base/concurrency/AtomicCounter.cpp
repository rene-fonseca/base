/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "AtomicCounter.h"

template<class TYPE>
AtomicCounter<TYPE>::AtomicCounter(TYPE value) throw() {
  this->value = value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator()() const throw() {
  TYPE result;
  ThreadLock::readLock();
    result = value;
  ThreadLock::unlock();
  return result;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator++() throw() {
  TYPE result;
  ThreadLock::writeLock();
    ++value;
    result = value;
  ThreadLock::unlock();
  return result;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator--() throw() {
  TYPE result;
  ThreadLock::writeLock();
    --value;
    result = value;
  ThreadLock::unlock();
  return result;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator+=(TYPE value) throw() {
  TYPE result;
  ThreadLock::writeLock();
    this->value += value;
    result = value;
  ThreadLock::unlock();
  return result;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator-=(TYPE value) throw() {
  TYPE result;
  ThreadLock::writeLock();
    this->value -= value;
    result = value;
  ThreadLock::unlock();
  return result;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator=(TYPE value) throw() {
  ThreadLock::writeLock();
    this->value = value;
  ThreadLock::unlock();
  return value;
}

template<class TYPE>
AtomicCounter<TYPE>::~AtomicCounter() throw() {
}
