/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__ATOMIC_COUNTER_H
#define _DK_SDU_MIP__BASE_THREAD__ATOMIC_COUNTER_H

#include <config.h>
#include <base/concurrency/Synchronize.h>

/**
  MT-safe counter.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class AtomicCounter : public Synchronizeable<DefaultLock> {
public:

  typedef DefaultLock LOCK;
private:

  /** The value of the counter. */
  TYPE value;
public:

  /**
    Initializes counter with zero.
  */
	inline AtomicCounter() throw() : value(0) {
    this->value = value;
	}

  /**
    Initializes the counter.

    @param value The initial value.
  */
	inline AtomicCounter(TYPE value) throw() : value(value) {}

  /**
    Returns the value of the counter.
  */
  inline TYPE operator()() const throw() {
    SynchronizeShared();
    return value;
  }

  /**
    Increments the counter.
  */
  inline TYPE operator++() throw() {
    SynchronizeExclusively();
    ++value;
    return value;
  }

  /**
    Decrements the counter.
  */
  inline TYPE operator--() throw() {
    SynchronizeExclusively();
    --value;
    return value;
  }

  /**
    Increment counter by value.
  */
  inline TYPE operator+=(TYPE value) throw() {
    SynchronizeExclusively();
    this->value += value;
    return value;
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(TYPE value) throw() {
    SynchronizeExclusively();
    this->value -= value;
    return value;
  }

  /**
    Assign value to counter.
  */
  inline TYPE operator=(TYPE value) throw() {
    SynchronizeExclusively();
    this->value = value;
    return value;
  }

};

#endif
