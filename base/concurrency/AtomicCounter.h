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

#if ((_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM) || \
     (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC))
// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
// https://en.cppreference.com/w/cpp/atomic/memory_order
// https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
#if defined(__has_builtin)
#if __has_builtin(__atomic_fetch_add) && defined(__ATOMIC_SEQ_CST)
#  define _COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC
#endif
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC) && defined(__ATOMIC_SEQ_CST)
#  define _COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC
#endif
#endif

#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
#elif (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) && \
      (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#  include <intrin.h> // header approved
#if ((_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86) || (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64))
#  define _COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86
#endif
#else
#  include <atomic> // header approved
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
namespace internal {

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
    BASSERT(sizeof(int) == sizeof(long));
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
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64)
    return _InterlockedExchange64(target, value);
#else
    BASSERT(!"Not supported.");
    return 0;
#endif
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
  inline int atomicAdd(volatile int* addend, const int value) noexcept
  {
    BASSERT(sizeof(int) == sizeof(long));
    return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(addend), value);
  }

  template<>
  inline long atomicAdd(volatile long* addend, const long value) noexcept
  {
    return _InterlockedExchangeAdd(addend, value);
  }

  template<>
  inline long long atomicAdd(volatile long long* addend, const long long value) noexcept
  {
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64)
    return _InterlockedExchangeAdd64(addend, value);
#else
    BASSERT(!"Not supported.");
    return 0;
#endif
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
  inline int atomicCompareExchange(volatile int* target, const int exchange, const int comparand) noexcept
  {
    BASSERT(sizeof(int) == sizeof(long));
    return _InterlockedCompareExchange(reinterpret_cast<volatile long*>(target), exchange, comparand);
  }

  template<>
  inline long atomicCompareExchange(volatile long* target, const long exchange, const long comparand) noexcept
  {
    return _InterlockedCompareExchange(target, exchange, comparand);
  }

  template<>
  inline long long atomicCompareExchange(volatile long long* target, const long long exchange, const long long comparand) noexcept
  {
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64)
    return _InterlockedCompareExchange64(target, exchange, comparand);
#else
    BASSERT(!"Not supported.");
    return 0;
#endif
  }
}
#endif

/** Atomic helpers. */
class _COM_AZURE_DEV__BASE__API Atomic {
public:

  /** Yields CPU. */
  static inline void yield() noexcept
  {
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM)
    __yield();
#endif
#endif
  }

  /** Thread fence. */
  static inline void threadFence() noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    _ReadWriteBarrier();
    static long dummy = 0;
    _InterlockedCompareExchange(&dummy, 0, 0);
    _ReadWriteBarrier();
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC) && \
      (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM)
    __dmb(0xb) // inner shared data memory barrier
#else
    atomic_thread_fence(std::memory_order_acquire);
#endif
  }

  /** Signal fence. */
  static inline void signalFence() noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    _ReadWriteBarrier();
#else
    atomic_signal_fence(std::memory_order_acquire);
#endif
  }
};

/**
  MT-safe counter. Try to avoid atomic load() if result is already available from
  previous operation. It is recommended to use signed types to make it easier to
  detect problems around 0.

  Make sure to use common alignment for the given type.

  @short Atomic counter
  @see SpinLock
  @ingroup concurrency
  @version 1.0
*/

template<class TYPE>
class _COM_AZURE_DEV__BASE__API AtomicCounter {
private:

  /** The value of the counter. */
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
  mutable volatile TYPE value;
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
  volatile TYPE value;
#else
  volatile std::atomic<TYPE> value;
#endif
  
  /**
    Loads the value.
  */
  inline TYPE load() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    return __atomic_load_n(&value, __ATOMIC_ACQUIRE); // __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE, __ATOMIC_CONSUME
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    return internal::atomicAdd<TYPE>(const_cast<volatile TYPE*>(&value), 0);
#else
    return value;
#endif
  }

  /**
    Stores the given value.
  */
  inline void store(const TYPE desired) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    BASSERT(__atomic_is_lock_free(sizeof(value), &value));
    __atomic_store_n(&value, desired, __ATOMIC_RELEASE); // __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_RELEASE
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    internal::atomicExchange<TYPE>(&value, desired);
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
    BASSERT(sizeof(TYPE) <= 16);
    BASSERT((getAddressOf(&value) & (sizeof(TYPE) - 1)) == 0);
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    BASSERT(__atomic_is_lock_free(sizeof(value), &value));
    __atomic_store_n(&value, _value, __ATOMIC_RELEASE); // __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_RELEASE
#else
    store(_value);
#endif
  }

  /**
    Copy constructor. Avoid this.
  */
  inline AtomicCounter(const AtomicCounter& _value) noexcept
  {
    BASSERT(sizeof(TYPE) <= 16);
    BASSERT((getAddressOf(&value) & (sizeof(TYPE) - 1)) == 0);
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    BASSERT(__atomic_is_lock_free(sizeof(value), &value));
    __atomic_store_n(&value, _value.value, __ATOMIC_RELEASE); // __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_RELEASE
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
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    return __atomic_fetch_add(&value, _value, __ATOMIC_ACQ_REL); // all memory orders
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    return internal::atomicAdd<TYPE>(&value, _value) + _value;
#else
    return value += _value;
#endif
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(const TYPE _value) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    return __atomic_fetch_sub(&value, _value, __ATOMIC_ACQ_REL); // all memory orders
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    return internal::atomicAdd<TYPE>(&value, -_value) - _value;
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

  inline bool operator==(const TYPE _value) const noexcept
  {
    return load() == _value;
  }

  inline bool operator!=(const TYPE _value) const noexcept
  {
    return load() != _value;
  }

  /**
    Exchange the value with the given value.
  */
  inline TYPE exchange(const TYPE desired) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    // __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE, and __ATOMIC_ACQ_REL
    return __atomic_exchange_n(&value, desired, __ATOMIC_SEQ_CST);
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    return internal::atomicExchange<TYPE>(&value, desired);
#else
    return value.exchange(desired);
#endif
  }

  /**
    Strong compare and exchange.
  */
  inline bool compareAndExchange(TYPE& expected, const TYPE desired) noexcept
  {
    BASSERT(expected != desired);
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    return __atomic_compare_exchange_n(&value, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); // strong
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    TYPE initial = internal::atomicCompareExchange<TYPE>(&value, desired, expected);
    bool result = initial == expected;
    expected = initial;
    return result;
#else
    return value.compare_exchange_strong(expected, desired);
#endif
  }

  /**
    Weak compare and exchange. Prefer strong if unsure.
  */
  inline bool compareAndExchangeWeak(TYPE& expected, const TYPE desired) noexcept
  {
    BASSERT(expected != desired);
#if defined(_COM_AZURE_DEV__BASE__USE_BUILT_IN_ATOMIC)
    return __atomic_compare_exchange_n(&value, &expected, desired, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); // weak
#elif defined(_COM_AZURE_DEV__BASE__USE_WIN32_INTRINSIC_X86)
    TYPE initial = internal::atomicCompareExchange<TYPE>(&value, desired, expected);
    bool result = initial == expected;
    expected = initial;
    return result;
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
