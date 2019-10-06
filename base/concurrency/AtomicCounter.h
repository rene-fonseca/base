/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/SpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  MT-safe counter.

  @short Atomic counter
  @see SpinLock
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class AtomicCounter {
private:

  /** The type of the guard. */
  typedef SpinLock Guard;
  /** The value of the counter. */
  TYPE value = 0;
  /** The guard. */
  Guard guard;
public:

  /**
    Initializes counter with zero.
  */
  inline AtomicCounter() throw() {
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
    ExclusiveSynchronize<Guard> _guard(guard);
    return value;
  }

  /**
    Increments the counter.
  */
  inline TYPE operator++() throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    ++value;
    return value;
  }

  /**
    Decrements the counter.
  */
  inline TYPE operator--() throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    --value;
    return value;
  }

  /**
    Increment counter by value.
  */
  inline TYPE operator+=(TYPE value) throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value += value;
    return value;
  }

  /**
    Decrement counter by value.
  */
  inline TYPE operator-=(TYPE value) throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value -= value;
    return value;
  }

  /**
    Assign value to counter.
  */
  inline TYPE operator=(const TYPE& value) throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value = value;
    return value;
  }

};

template<>
class AtomicCounter<int> {
private:

  /** The type of the guard. */
  typedef SpinLock Guard;
  /** The value. */
  int value;
  /** The guard. */
  Guard guard;
public:

  inline AtomicCounter() throw() : value(0) {
  }

  inline AtomicCounter(int _value) throw() : value(_value) {
  }

  inline operator int() const throw() {
    return value;
  }

  inline int operator++() throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__I386)
    asm volatile (
      "lock; incl %0 \n\t"
      : "=m" (value) // output
      : "0" (value) // input
    );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "fetchadd4.acq r2 = %0, 1 \n\t"
      : "=m" (value) // output
      : "0" (value) // input
      : "r2" // clobbered
    );
#else
    ExclusiveSynchronize<Guard> _guard(guard);
    ++value;
    return value;
#endif // architecture
  }

  inline int operator--() throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__I386)
    asm volatile (
      "lock; decl %0 \n\t"
      : "=m" (value) // output
      : "0" (value) // input
    );
#elif (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "fetchadd4.acq r2 = %0, -1 \n\t"
      : "=m" (value) // output
      : "0" (value) // input
      : "r2" // clobbered
    );
#else
    ExclusiveSynchronize<Guard> _guard(guard);
    --value;
    return value;
#endif // architecture
  }

  inline int operator+=(int value) throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "1: \n\t"
      "ld4 r14 = %0 ;; \n\t" // get current value
      "mov ar.ccv = r14 \n\t"
      "add r15 = %1, r14 ;; \n\t" // add values
      "cmpxchg4.acq r2 = %0, r15, ar.ccv ;; \n\t" // acquire the lock
      "cmp.eq p0, p6 = r14, r2 \n\t" // did the add succeed
      "(p6) br.cond.dpnt 1b ;; \n\t" // try again
      : "=m" (this->value) // output
      : "0" (this->value), "r" (value) // input
      : "r2", "r14", "r15" // clobbered
    );
#else
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value += value;
    return value;
#endif // architecture
  }

  inline int operator-=(int value) throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "1: \n\t"
      "ld4 r14 = %0 ;; \n\t" // get current value
      "mov ar.ccv = r14 \n\t"
      "sub r15 = r14, %1 ;; \n\t" // subtract values
      "cmpxchg4.acq r2 = %0, r15, ar.ccv ;; \n\t" // acquire the lock
      "cmp.eq p0, p6 = r14, r2 \n\t" // did the store succeed
      "(p6) br.cond.dpnt 1b ;; \n\t" // try again
      : "=m" (this->value) // output
      : "0" (this->value), "r" (value) // input
      : "r2", "r14", "r15" // clobbered
    );
#else
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value -= value;
    return value;
#endif // architecture
  }

  inline int operator=(int value) throw() {
#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__IA64)
    asm volatile (
      "1: \n\t"
      "ld4 r14 = %0 ;; \n\t" // get current value
      "mov ar.ccv = r14 \n\t"
      "sub r15 = r14, %1 ;; \n\t" // subtract values
      "cmpxchg4.acq r2 = %0, r15, ar.ccv ;; \n\t" // acquire the lock
      "cmp.eq p0, p6 = r14, r2 \n\t" // did the store succeed
      "(p6) br.cond.dpnt 1b ;; \n\t" // try again
      : "=m" (this->value) // output
      : "0" (this->value), "r" (value) // input
      : "r2", "r14", "r15" // clobbered
    );
#else
    ExclusiveSynchronize<Guard> _guard(guard);
    this->value = value;
    return value;
#endif
  }

};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
