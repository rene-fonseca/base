/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__ATOMIC_COUNTER_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__ATOMIC_COUNTER_H

#include <base/features.h>
#include <base/concurrency/Synchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  MT-safe counter.

  @short Atomic counter.
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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

  /**<
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
