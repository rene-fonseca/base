/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SYNCHRONIZE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SYNCHRONIZE_H

#include <base/concurrency/Synchronizeable.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Selects exclusive synchronization of this scope of this synchronizeable
  object (@ref Synchronize).
*/
#define SynchronizeExclusively() ExclusiveSynchronize<LOCK> _sync(*this)

/**
  Selects shared synchronization of this scope of this synchronizeable object.
  (@ref Synchronize).
*/
#define SynchronizeShared() SharedSynchronize<LOCK> _sync(*this)

/**
  Releases the lock acquired by SynchronizeExclusively() or
  SynchronizeShared().
*/
#define SynchronizeRelease() _sync.releaseLock()

/**
  This is a stack based wrapper of a synchronize able class. A Synchronize
  object will acquire and release an exclusive (or a shared lock) on the
  provided synchronize able object when it is, respectively, created and
  destroyed. If required, the lock can be explicitly released by calling the
  method release(). This is required before throwing an exception.

  Three macros have been provided for easy use of this class:
  SynchronizeExclusively(), SynchronizeShared(), and SynchronizeRelease().
  These macros should be used like any normal function.

  SynchronizeExclusively(): Selects exclusively synchronization of the current
  scope within a synchronize able class (@ref Synchronizeable). The macro
  manages this by creating a temporary Synchronize object called '_sync' on
  the stack. Please note that the variable '_sync' is reserved for this
  purpose and should not be used elsewhere to avoid misunderstandings. You
  should only use the provided macros to access the variable '_sync'.

  SynchronizeShared(): Works the same way as SynchronizeExclusively() but, as
  the name suggests, acquires a shared lock.

  SynchronizeRelease(): If required, you can release the lock explicitly by
  using SynchronizeRelease().

  The macros are intended to be used within a template class that takes a
  template argument called LOCK and is a base class of Synchronizeable<LOCK>.
  Like illustrated by this example:

  @code
  template<class LOCK = DefaultLock>
  class MyClass : public Synchronizeable<LOCK> {
    // stuffing goes here :-)
  };
  @endcode

  However, if you for some reason want to use these macros within a class that
  does not take a template argument LOCK, you must define LOCK explicitly and
  identical to the template argument of the base class Synchronizeable. Like
  this:

  @code
  class MyClass : public Synchronizeable<DefaultLock> {
  public:
    typedef DefaultLock LOCK;

    void myFirstMethod() const throw() {
      SynchronizeShared();
      // do something as long as it doesn't modify the object
    }

    void mySecondMethod() throw() {
      SynchronizeExclusive();
      // do modification of object
    }
  };
  @endcode

  @short A stack based wrapper of a synchronize able class.
  @ingroup concurrency
  @see Synchronizeable
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
  @deprecated
*/

template<class LOCK>
class Synchronize {
private:

  /** The synchronize able object to be synchronized. */
  const Synchronizeable<LOCK>& object;

  Synchronize(const Synchronize& copy) throw();
  Synchronize& operator=(const Synchronize& eq) throw();
public:
  
  /**
    Initializes the synchronization object.

    @param object The synchronize able object to be synchronized.
    @param exclusive Specifies if the lock should be exclusive (write-lock) or
    shared (read-lock). Default is exclusive.
  */
  inline explicit Synchronize(
    const Synchronizeable<LOCK>& _object, bool exclusive = true) throw()
    : object(_object) {
    if (exclusive) {
      object.exclusiveLock();
    } else {
      object.sharedLock();
    }
  }

  /**
    Forces the lock to be released.
  */
//    inline void release() throw() {
//      object.releaseLock();
//    }

  /**
    Releases the lock if not already released and destroys the synchronization
    object.
  */
  inline ~Synchronize() throw() {
    object.releaseLock();
  }
};



/*
  Optimized version of Synchronize intended for single threaded appliances.
  
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
  @deprecated
*/

template<>
class Synchronize<Unsafe> {
private:

  Synchronize(const Synchronize& copy) throw();
  Synchronize& operator=(const Synchronize& eq) throw();
public:
  
  /**
    Initializes the synchronization object.
    
    @param object The synchronize able object to be synchronized.
    @param exclusive Specifies if the lock should be exclusive (write-lock) or
    shared (read-lock). Default is exclusive.
  */
  inline explicit Synchronize(
    const Synchronizeable<Unsafe>& object, bool exclusive = true) throw() {
  }

  /**
    Forces the lock to be released.
  */
  inline void release() throw() {
  }

  /**
    Releases the lock if not already released and destroys the synchronization
    object.
  */
  inline ~Synchronize() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
