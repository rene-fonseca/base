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

#include <base/Primitives.h>

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <stdatomic.h>
#else
#  include <atomic>
#endif

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
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  volatile _Atomic(TYPE) value; // initialized in constructors
#else
  volatile std::atomic<TYPE> value;
#endif
  
  AtomicCounter(const AtomicCounter& _value);
  AtomicCounter& operator=(const AtomicCounter& _value);
public:

  /**
    Initializes counter with zero.
  */
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  inline AtomicCounter() noexcept {
    ASSERT(atomic_is_lock_free(&value));
    atomic_init(&value, 0);
  }
#else
  inline AtomicCounter() noexcept : value(0) {
  }
#endif

  /**
    Initializes the counter.

    @param value The initial value.
  */
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  inline AtomicCounter(TYPE _value) noexcept {
    atomic_init(&value, _value);
  }
#else
  inline AtomicCounter(TYPE _value) noexcept : value(_value) {
  }
#endif

  /**
    Returns the value of the counter.
  */
  inline operator TYPE() const noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_load(&value);
#else
    return value;
#endif
  }

  /**
    Increments the counter.
  */
  inline TYPE operator++(int) noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_add(&value, 1);
#else
    return value++;
#endif
   }

  /**
    Increments the counter.
  */
  inline TYPE operator++() noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_add(&value, 1) + 1;
#else
    return ++value;
#endif
  }

  /**
    Decrements the counter.
  */
  inline TYPE operator--(int) noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_sub(&value, 1);
#else
    return value--;
#endif
  }

  /**
    Decrements the counter.
  */
  inline TYPE operator--() noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_sub(&value, 1) - 1;
#else
    return --value;
#endif
  }

  /**
    Increment counter by value.
  */
  inline TYPE operator+=(TYPE _value) noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_add(&value, _value) + _value;
#else
    value += _value;
    return value;
#endif
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(TYPE _value) noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_sub(&value, _value) - _value;
#else
    value -= _value;
    return value;
#endif
  }

  /**
    Assign value to counter.
  */
  inline AtomicCounter& operator=(TYPE _value) noexcept {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    atomic_store(&value, _value);
#else
    value = _value;
#endif
    return *this;
  }

  /**
    Strong compare and exchange.
  */
  inline bool compareAndExchange(TYPE& expected, const TYPE desired) noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_compare_exchange_strong(&value, &expected, desired);
#else
    return value.compare_exchange_strong(expected, desired);
#endif
  }

  /**
    Weak compare and exchange.
  */
  inline bool compareAndExchangeWeak(TYPE& expected, const TYPE desired) noexcept
  {
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_compare_exchange_weak(&value, &expected, desired);
#else
    return value.compare_exchange_weak(expected, desired);
#endif
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
