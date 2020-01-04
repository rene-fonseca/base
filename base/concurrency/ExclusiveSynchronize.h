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

#include <base/concurrency/Synchronizeable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is used to synchronize executing contexts exclusively with a
  guard object of your choice.

  @code
  class MyResource : public Object {
  private:

    ReadWriteLock guard;
  public:

    void myFirstMethod() const {
      SynchronizeShared<ReadWriteLock> synchronizeShared(guard);
      // do something as long as it doesn't modify the object
      if (earlyReturn) {
        return; // no need to explicitly release guard
      }
      bassert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }

    void mySecondMethod() {
      ExclusiveExclusive<ReadWriteLock> exclusiveShared(guard);
      // do modification of object
      if (earlyReturn) {
        return; // no need to explicitly release guard
      }
      bassert(!somethingWentWrong, MyResourceException()); // no need to explicitly release guard
    }
  };
  @endcode

  @short Exclusive synchronization.
  @ingroup concurrency
  @see SharedSynchronize
  @version 1.0
*/

template<class GUARD>
class ExclusiveSynchronize {
private:

  /** The type of the guard. */
  typedef GUARD Guard;
  /** The synchronize able object to be synchronized. */
  const Guard& guard;
  
  ExclusiveSynchronize(const ExclusiveSynchronize& copy);
  ExclusiveSynchronize& operator=(const ExclusiveSynchronize& assign);
public:
  
  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  inline explicit ExclusiveSynchronize(const Guard& _guard) 
    : guard(_guard) {
    guard.exclusiveLock();
  }
  
  /**
    Releases the lock if not already released and destroys the synchronization
    object.
  */
  inline ~ExclusiveSynchronize() {
    guard.releaseLock();
  }
};



/*
  Optimized version of ExclusiveSynchronize intended for single threaded
  applications.
  
  @short Unsafe exclusive synchronization.
  @version 1.0
*/

template<>
class ExclusiveSynchronize<Unsafe> {
private:

  ExclusiveSynchronize(const ExclusiveSynchronize& copy);
  ExclusiveSynchronize& operator=(const ExclusiveSynchronize& assign);
public:

  /**
    Initializes the synchronization object.

    @param guard The synchronize able object to be synchronized.
  */
  template<class POLY>
  inline explicit ExclusiveSynchronize(const POLY& guard) noexcept
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
