/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SYNCHRONIZEABLE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SYNCHRONIZEABLE_H

#include <base/concurrency/ReadWriteLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to specify that a synchronize able class should not be
  synchronized. In other words the class will be unsafe to use in a multi-
  threaded environment. If you, however, know that an object is only going to
  be accessed by one thread at a time you can use unsafe objects for better
  performance. This class is the only non-lock class that can be used with the
  classes Synchronizeable and Synchronize. Unsafe triggers a highly optimized
  specialization in either case.

  @short Select no synchronization.
  @ingroup concurrency
  @see Synchronizeable, Synchronize
*/
class Unsafe {
};

/**
  Defines the default synchronization class (a class that implements the Lock
  interface) to use for synchronize able classes when no synchronization class
  is specified.
*/
typedef Unsafe DefaultLock;

/**
  Wrapper for a synchronization object. Ensures that only one synchronization
  object is used by a synchronized object. The synchronization object is not
  considered a part of the state of the synchronized object. Valid
  synchronization objects are those who implement the @ref Lock interface.

  @short Wrapper for a synchronization object.
  @ingroup concurrency
  @see Synchronize Lock ReadWriteLock MutualExclusion Unsafe
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class LOCK = DefaultLock>
class Synchronizeable : public virtual LOCK {
private:

  /**
    Copy constructor. Does not copy the internal locking object but creates a new locking object.
  */
  inline Synchronizeable(const Synchronizeable& copy) : LOCK() {
  }

  /**
    Assignment operator. Does not modify the internal locking object.
  */
  inline Synchronizeable& operator=(const Synchronizeable& eq) {
    return *this;
  }
public:

  /**
    Initializes a synchronize able object.
  */
  inline Synchronizeable() : LOCK() {
  }

// protected:
public:

  /**
    Acquires an exclusive lock on this object.
  */
  inline void exclusiveLock() const throw() {
    LOCK::exclusiveLock();
  }

  /**
    Acquires a shared lock on this object.
  */
  inline void sharedLock() const throw() {
    LOCK::sharedLock();
  }

  /**
    Releases the lock on this object.
  */
  inline void releaseLock() const throw() {
    LOCK::releaseLock();
  }
};



/**
  Specialization of the template class Synchronizeable that is intended for single threaded appliances.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<>
class Synchronizeable<Unsafe> {
protected:

  /**
    Acquires an exclusive lock on this object.
  */
  inline void exclusiveLock() const throw() {
  }

  /**
    Acquires a shared lock on this object.
  */
  inline void sharedLock() const throw() {
  }

  /**
    Releases the lock on this object.
  */
  inline void releaseLock() const throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
