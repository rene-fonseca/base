/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/concurrency/AtomicCounter.h>

template AtomicCounter<unsigned int>;
template AtomicCounter<unsigned long long>;

template<class TYPE>
AtomicCounter<TYPE>::AtomicCounter(TYPE value) throw() {
  this->value = value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator()() const throw() {
  SynchronizeShared();
  return value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator++() throw() {
  SynchronizeExclusively();
  ++value;
  return value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator--() throw() {
  SynchronizeExclusively();
  --value;
  return value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator+=(TYPE value) throw() {
  SynchronizeExclusively();
  this->value += value;
  return value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator-=(TYPE value) throw() {
  SynchronizeExclusively();
  this->value -= value;
  return value;
}

template<class TYPE>
TYPE AtomicCounter<TYPE>::operator=(TYPE value) throw() {
  SynchronizeExclusively();
  this->value = value;
  return value;
}

template<class TYPE>
AtomicCounter<TYPE>::~AtomicCounter() throw() {
}
