/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SINGLE_EXCLUSIVE_SYNCHRONIZE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SINGLE_EXCLUSIVE_SYNCHRONIZE_H

#include <base/concurrency/Synchronizeable.h>
#include <base/concurrency/ConcurrencyException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to synchronize executing contexts exclusively with a
  guard object of your choice.
  
  @short Exclusive synchronization.
  @ingroup concurrency
  @see SharedSynchronize
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class GUARD>
class SingleExclusiveSynchronize {
private:

  /** The type of the guard. */
  typedef GUARD Guard;
  /** The synchronize able object to be synchronized. */
  const Guard& guard;
  
  SingleExclusiveSynchronize(const SingleExclusiveSynchronize& copy) throw();
  SingleExclusiveSynchronize& operator=(
    const SingleExclusiveSynchronize& eq) throw();
public:
  
  /**
    Initializes the synchronization object. Raises ConcurrencyException if the
    guard has already been locked.
    
    @param guard The synchronize able object to be synchronized.
  */
  inline explicit SingleExclusiveSynchronize(
    const Guard& _guard) throw(ConcurrencyException)
    : guard(_guard) {
    assert(guard.tryExclusiveLock(), ConcurrencyException(this));
  }
  
  /**
    Releases the lock if not already released and destroys the synchronization
    object.
  */
  inline ~SingleExclusiveSynchronize() throw() {
    guard.releaseLock();
  }
};



/*
  Optimized version of SingleExclusiveSynchronize intended for single threaded
  applications.
  
  @short Unsafe exclusive synchronization.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<>
class SingleExclusiveSynchronize<Unsafe> {
private:

  SingleExclusiveSynchronize(const SingleExclusiveSynchronize& copy) throw();
  SingleExclusiveSynchronize& operator=(
    const SingleExclusiveSynchronize& eq) throw();
public:

  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  template<class POLY>
  inline explicit SingleExclusiveSynchronize(const POLY& guard) throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
