/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__EXCLUSIVE_SYNCHRONIZE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__EXCLUSIVE_SYNCHRONIZE_H

#include <base/concurrency/Synchronizeable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to synchronize executing contexts exclusively with a
  guard object of your choice.

  <pre>
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
      assert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }

    void mySecondMethod() throw(MyResourceException) {
      ExclusiveExclusive<ReadWriteLock> exclusiveShared(guard);
      // do modification of object
      if (earlyReturn) {
        return; // no need to explicitly release guard
      }
      assert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }
  };
  </pre>

  @short Exclusive synchronization
  @ingroup concurrency
  @see SharedSynchronize
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class GUARD>
class ExclusiveSynchronize {
private:

  /** The synchronize able object to be synchronized. */
  const GUARD& guard;

  ExclusiveSynchronize(const ExclusiveSynchronize& copy); // prohibit copy construction
  ExclusiveSynchronize& operator=(const ExclusiveSynchronize& eq); // prohibit assignment
public:

  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  explicit ExclusiveSynchronize(const GUARD& guard) throw();

  /**
    Releases the lock if not already released and destroys the synchronization object.
  */
  inline ~ExclusiveSynchronize() throw() {
    guard.releaseLock();
  }
};

template<class GUARD>
inline ExclusiveSynchronize<GUARD>::ExclusiveSynchronize(const GUARD& _guard) throw() : guard(_guard) {
  guard.exclusiveLock();
}



/**
  Optimized version of ExclusiveSynchronize intended for single threaded
  applications.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<>
class ExclusiveSynchronize<Unsafe> {
private:

  ExclusiveSynchronize(const ExclusiveSynchronize& copy); // prohibit copy construction
  ExclusiveSynchronize& operator=(const ExclusiveSynchronize& eq); // prohibit assignment
public:

  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  template<class POLY>
  inline explicit ExclusiveSynchronize(const POLY& guard) throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
