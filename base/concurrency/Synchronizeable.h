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

#include <base/Object.h>
#include <base/concurrency/ReadWriteLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

class _COM_AZURE_DEV__BASE__API Unsafe {
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
  @version 1.0
*/

template<class LOCK = DefaultLock>
class Synchronizeable : public Object {
private:

  /** Lock. */
  LOCK lock;
  
  /**
    Copy constructor. Does not copy the internal locking object but creates a
    new locking object.
  */
  inline Synchronizeable(const Synchronizeable& copy) throw(LockException) {
  }

  /**
    Assignment operator. Does not modify the internal locking object.
  */
  inline Synchronizeable& operator=(const Synchronizeable& eq) throw() {
    return *this;
  }
protected:
  
  /**
    Initializes a synchronize able object.
  */
  inline Synchronizeable() throw(LockException) {
  }
  
  /**
    Acquires an exclusive lock on this object.
  */
  inline void exclusiveLock() const throw(LockException) {
    lock.exclusiveLock();
  }
  
  /**
    Acquires a shared lock on this object.
  */
  inline void sharedLock() const throw(LockException) {
    lock.sharedLock();
  }
  
  /**
    Releases the lock on this object.
  */
  inline void releaseLock() const throw(LockException) {
    lock.releaseLock();
  }
};



/*
  Specialization of the template class Synchronizeable that is intended for
  single threaded appliances.
  
  @short Unsafe synchronizeable.
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
