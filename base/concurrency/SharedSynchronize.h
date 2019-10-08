/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/Synchronizeable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to synchronize executing contexts (shared) with a guard
  object of your choice. Some guard classes may implement the shared locks
  using exclusive locks.

  @code
  class MyResource : public Object {
  private:

    ReadWriteLock guard;
  public:

    void myFirstMethod() const throw(MyResourceException) {
      SynchronizeShared<ReadWriteLock> synchronizeShared(guard);
      // do something as long as it doesn't modify the object
      if (earlyReturn) {
        return; // no need to explicitly release guard
      }
      bassert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }

    void mySecondMethod() throw(MyResourceException) {
      ExclusiveExclusive<ReadWriteLock> exclusiveShared(guard);
      // do modification of object
      if (earlyReturn) {
        return; // no need to explicitly release guard
      }
      bassert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }
  };
  @endcode

  @short Shared synchronization.
  @ingroup concurrency
  @see ExclusiveSynchronize
  @version 1.0
*/

template<class GUARD>
class SharedSynchronize {
private:

  /** The type of the guard. */
  typedef GUARD Guard;
  /** The synchronize able object to be synchronized. */
  const Guard& guard;

  SharedSynchronize(const SharedSynchronize& copy) throw();
  SharedSynchronize& operator=(const SharedSynchronize& eq) throw();
public:

  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  inline explicit SharedSynchronize(const Guard& _guard) throw()
    : guard(_guard) {
    guard.sharedLock();
  }
  
  /**
    Releases the lock if not already released and destroys the synchronization
    object.
  */
  inline ~SharedSynchronize() throw() {
    guard.releaseLock();
  }
};



/*
  Optimized version of SharedSynchronize intended for single threaded
  applications.

  @short Unsafe shared synchronization.
  @version 1.0
*/

template<>
class SharedSynchronize<Unsafe> {
private:
  
  SharedSynchronize(const SharedSynchronize& copy) throw();
  SharedSynchronize& operator=(const SharedSynchronize& eq) throw();
public:
  
  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  template<class POLY>
  inline explicit SharedSynchronize(const POLY& guard) throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
