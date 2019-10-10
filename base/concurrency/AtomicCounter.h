/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>
#include <atomic>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  MT-safe counter.

  @short Atomic counter
  @see SpinLock
  @ingroup concurrency
  @version 1.0
*/

template<class TYPE>
class AtomicCounter {
private:

  /** The value of the counter. */
  std::atomic<TYPE> value;
public:

  /**
    Initializes counter with zero.
  */
  inline AtomicCounter() throw() : value(0) {
  }

  /**
    Initializes the counter.

    @param value The initial value.
  */
  inline AtomicCounter(TYPE _value) throw() : value(_value) {
  }

  /**
    Returns the value of the counter.
  */
  inline operator TYPE() const throw() {
    return value;
  }

  /**
    Increments the counter.
  */
  inline TYPE operator++() throw() {
    return ++value;
  }

  /**
    Decrements the counter.
  */
  inline TYPE operator--() throw() {
    return --value;
  }

  /**
    Increment counter by value.
  */
  inline TYPE operator+=(TYPE _value) throw() {
    value += _value;
    return value;
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(TYPE _value) throw() {
    value -= _value;
    return value;
  }

  /**
    Assign value to counter.
  */
  inline AtomicCounter& operator=(TYPE _value) throw() {
    value = _value;
    return *this;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
