/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__ATOMIC_COUNTER_H
#define _DK_SDU_MIP__BASE_THREAD__ATOMIC_COUNTER_H

#include "Synchronize.h"

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
    Initializes the counter.

    @param value The initial value. The default is 0.
  */
	AtomicCounter(TYPE value = 0) throw();

  /**
    Returns the value of the counter.
  */
  TYPE operator()() const throw();

  /**
    Increments the counter.
  */
  TYPE operator++() throw();

  /**
    Decrements the counter.
  */
  TYPE operator--() throw();

  /**
    Increment counter by value.
  */
  TYPE operator+=(TYPE value) throw();

  /**
    Decrement counter by value.
  */
  TYPE operator-=(TYPE value) throw();

  /**
    Assign value to counter.
  */
  TYPE operator=(TYPE value) throw();

  /**
    Destroys the counter.
  */
	~AtomicCounter() throw();
};

#endif
