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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#  include <intrin.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <stdatomic.h>
#else
#  include <atomic>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template<typename TYPE>
class MapToSignedType {
};

template<>
class MapToSignedType<unsigned char> {
public:

  typedef char SignedType;
};

template<>
class MapToSignedType<unsigned short> {
public:

  typedef short SignedType;
};

template<>
class MapToSignedType<unsigned int> {
public:

  typedef int SignedType;
};

template<>
class MapToSignedType<unsigned long> {
public:

  typedef long SignedType;
};

template<>
class MapToSignedType<unsigned long long> {
public:

  typedef long long SignedType;
};

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
namespace _win32 {

  template<typename TYPE>
  TYPE atomicExchange(volatile TYPE* target, const TYPE value) noexcept;

  template<>
  inline char atomicExchange(volatile char* target, const char value) noexcept
  {
    return _InterlockedExchange8(target, value);
  }

  template<>
  inline short atomicExchange(volatile short* target, const short value) noexcept
  {
    return _InterlockedExchange16(target, value);
  }

  template<>
  inline int atomicExchange(volatile int* target, const int value) noexcept
  {
    ASSERT(sizeof(int) == sizeof(long));
    return _InterlockedExchange(reinterpret_cast<volatile long*>(target), value);
  }

  template<>
  inline long atomicExchange(volatile long* target, const long value) noexcept
  {
    return _InterlockedExchange(target, value);
  }

  template<>
  inline long long atomicExchange(volatile long long* target, const long long value) noexcept
  {
    return _InterlockedExchange64(target, value);
  }

  // returns initial!
  template<typename TYPE>
  TYPE atomicAdd(volatile TYPE* addend, const TYPE value) noexcept;

  template<>
  inline char atomicAdd(volatile char* addend, const char value) noexcept
  {
    return _InterlockedExchangeAdd8(addend, value);
  }

  template<>
  inline short atomicAdd(volatile short* addend, const short value) noexcept
  {
    return _InterlockedExchangeAdd16(addend, value);
  }

  template<>
  inline long atomicAdd(volatile long* addend, const long value) noexcept
  {
    return _InterlockedExchangeAdd(addend, value);
  }

  template<>
  inline long long atomicAdd(volatile long long* addend, const long long value) noexcept
  {
    return _InterlockedExchangeAdd64(addend, value);
  }

  template<typename TYPE>
  TYPE atomicCompareExchange(volatile TYPE* target, const TYPE exchange, const TYPE comparand) noexcept;

  template<>
  inline char atomicCompareExchange(volatile char* target, const char exchange, const char comparand) noexcept
  {
    return _InterlockedCompareExchange8(target, exchange, comparand);
  }

  template<>
  inline short atomicCompareExchange(volatile short* target, const short exchange, const short comparand) noexcept
  {
    return _InterlockedCompareExchange16(target, exchange, comparand);
  }

  template<>
  inline long atomicCompareExchange(volatile long* target, const long exchange, const long comparand) noexcept
  {
    return _InterlockedCompareExchange(target, exchange, comparand);
  }

  template<>
  inline long long atomicCompareExchange(volatile long long* target, const long long exchange, const long long comparand) noexcept
  {
    return _InterlockedCompareExchange64(target, exchange, comparand);
  }
}
#endif

/**
  MT-safe counter. Try to avoid atomic load() if result is already available from
  previous operation. It is recommended to use signed types to make it easier to
  detect problems around 0.

  @short Atomic counter
  @see SpinLock
  @ingroup concurrency
  @version 1.0
*/

template<class TYPE>
class _COM_AZURE_DEV__BASE__API AtomicCounter {
private:

  /** The value of the counter. */
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  volatile TYPE value;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  volatile _Atomic(TYPE) value; // initialized in constructors
#else
  volatile std::atomic<TYPE> value;
#endif
  
  /**
    Loads the value.
  */
  inline TYPE load() const noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    return _win32::atomicAdd<TYPE>(const_cast<volatile TYPE*>(&value), 0);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_load(&value);
#else
    return value;
#endif
  }

  /**
    Stores the given value.
  */
  inline void store(const TYPE desired) noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    _win32::atomicExchange<TYPE>(&value, desired);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    atomic_exchange(&value, desired);
#else
    value.store(desired);
#endif
  }
public:

  enum {
    DEFAULT_VALUE = 0,
    DESTRUCT_VALUE = -1
  };

  /**
    Initializes the counter with 0.

    @param value The initial value.
  */
  inline AtomicCounter(const TYPE _value = DEFAULT_VALUE) noexcept
  {
    ASSERT((getAddressOf(&value) & (sizeof(TYPE) - 1)) == 0);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    ASSERT(atomic_is_lock_free(&value));
    atomic_init(&value, _value);
#else
    store(_value);
#endif
  }

  /**
    Copy constructor. Avoid this.
  */
  inline AtomicCounter(const AtomicCounter& _value) noexcept
  {
    ASSERT((getAddressOf(&value) & (sizeof(TYPE) - 1)) == 0);
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    ASSERT(atomic_is_lock_free(&value));
    atomic_init(&value, _value.load());
#else
    store(_value.load());
#endif
  }

  /**
    Assignment. Avoid this.
  */
  AtomicCounter& operator=(const AtomicCounter& _value) noexcept
  {
    store(_value.load());
    return *this;
  }

  /**
    Returns the value of the counter.
  */
  inline operator TYPE() const noexcept
  {
    return load();
  }

  /**
    Post-increments the counter. Prefer pre-increment.
  */
  inline TYPE operator++(int) noexcept
  {
    const TYPE result = *this += 1;
    return result - 1; // initial
   }

  /**
    Pre-increments the counter.
  */
  inline TYPE operator++() noexcept
  {
    return *this += 1; // new value
  }

  /**
    Post-decrements the counter. Prefer pre-decrement.
  */
  inline TYPE operator--(int) noexcept
  {
    const TYPE result = *this -= 1;
    return result + 1; // initial
  }

  /**
    Pre-decrements the counter.
  */
  inline TYPE operator--() noexcept
  {
    return *this -= 1; // new value
  }

  /**
    Increment counter by value.
  */
  inline TYPE operator+=(const TYPE _value) noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    return _win32::atomicAdd<TYPE>(&value, _value) + _value;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_add(&value, _value) + _value;
#else
    return value += _value;
#endif
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(const TYPE _value) noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    return _win32::atomicAdd<TYPE>(&value, -_value) - _value;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_fetch_sub(&value, _value) - _value;
#else
    return value -= _value;
#endif
  }

  /**
    Assign value to counter.
  */
  inline AtomicCounter& operator=(const TYPE _value) noexcept
  {
    store(_value);
    return *this;
  }

  /**
    Exchange the value with the given value.
  */
  inline TYPE exchange(const TYPE desired) noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    return _win32::atomicExchange<TYPE>(&value, desired);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_exchange(&value, desired);
#else
    return value.atomic_exchange(desired);
#endif
  }

  /**
    Strong compare and exchange.
  */
  inline bool compareAndExchange(TYPE& expected, const TYPE desired) noexcept
  {
    ASSERT(expected != desired);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    TYPE initial = _win32::atomicCompareExchange<TYPE>(&value, desired, expected);
    bool result = initial == expected;
    expected = initial;
    return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
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
    ASSERT(expected != desired);
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    TYPE initial = _win32::atomicCompareExchange<TYPE>(&value, desired, expected);
    bool result = initial == expected;
    expected = initial;
    return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    return atomic_compare_exchange_weak(&value, &expected, desired);
#else
    return value.compare_exchange_weak(expected, desired);
#endif
  }

  inline ~AtomicCounter() noexcept
  {
    store(DESTRUCT_VALUE); // for MT-consistency
  }
};

/** Atomic counter. */
template<class TYPE>
class _COM_AZURE_DEV__BASE__API AtomicCounterU : public AtomicCounter<typename MapToSignedType<TYPE>::SignedType> {
public:

  typedef typename MapToSignedType<TYPE>::SignedType SignedType;
  typedef AtomicCounter<SignedType> BASE;

  inline AtomicCounterU(const TYPE _value = static_cast<TYPE>(BASE::DEFAULT_VALUE)) noexcept
    : BASE(static_cast<SignedType>(_value))
  {
  }

  inline operator TYPE() const noexcept
  {
    return BASE::operator SignedType();
  }

  inline TYPE operator++(int) noexcept
  {
    return BASE::operator++(0);
  }

  inline TYPE operator++() noexcept
  {
    return BASE::operator++();
  }

  inline TYPE operator--(int) noexcept
  {
    return BASE::operator--(0);
  }

  inline TYPE operator--() noexcept
  {
    return BASE::operator--();
  }

  inline TYPE operator+=(const TYPE _value) noexcept
  {
    return BASE::operator+=(static_cast<SignedType>(_value));
  }

  inline TYPE operator-=(const TYPE _value) noexcept
  {
    return BASE::operator-=(static_cast<SignedType>(_value));
  }

  inline AtomicCounterU& operator=(const TYPE _value) noexcept
  {
    BASE::operator=(static_cast<SignedType>(_value));
    return *this;
  }

  inline TYPE exchange(const TYPE desired) noexcept
  {
    return BASE::exchange(static_cast<SignedType>(desired));
  }

  inline bool compareAndExchange(TYPE& expected, const TYPE desired) noexcept
  {
    return BASE::compareAndExchange(reinterpret_cast<SignedType&>(expected), static_cast<SignedType>(desired));
  }

  inline bool compareAndExchangeWeak(TYPE& expected, const TYPE desired) noexcept
  {
    return BASE::compareAndExchangeWeak(reinterpret_cast<SignedType&>(expected), static_cast<SignedType>(desired));
  }
};

typedef AtomicCounter<MemoryDiff> PreferredAtomicCounter;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
