/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__SYNCHRONIZEABLE_H
#define _DK_SDU_MIP__BASE_THREAD__SYNCHRONIZEABLE_H

#include "ReadWriteLock.h"

/**
  This class is used to specify that a template class should not be
  synchronized. In other words the class will be unsafe to use in a multi-
  threaded environment.
*/
class Unsafe {
};

/**
  Defines the default lock class to use for synchronize able classes when no
  locking class is specified.

  template<class LOCK = DefaultLock> class MyClass...
*/
typedef ReadWriteLock DefaultLock;

/**
  Wrapper for a locking object. Ensures that only one synchronization object
  is used by a synchronized object. The synchronization object is not
  considered a part of the state of the synchronized object.

  @author René Møller Fonseca
  @version 1.0
*/

template<class LOCK = DefaultLock>
class Synchronizeable : private virtual LOCK {
public:

  /**
    Initializes a synchronize able object.
  */
  inline Synchronizeable() : LOCK() {};

  /**
    Copy constructor. Does not copy the internal locking object but creates a new locking object.
  */
  inline Synchronizeable(const Synchronizeable& copy) : LOCK() {};

  /**
    Assignment operator. Does not modify the internal locking object.
  */
  inline Synchronizeable& operator=(const Synchronizeable& eq) {return *this;};
//protected:
public:

  /**
    Acquires an exclusive lock on this object.
  */
  void exclusiveLock() const throw();

  /**
    Acquires a shared lock on this object.
  */
  void sharedLock() const throw();

  /**
    Releases the lock on this object.
  */
  void releaseLock() const throw();
};



/**
  Specialization of the template class Synchronizeable that is intended for single threaded appliances.

  @author René Møller Fonseca
  @version 1.0
*/

template<>
class Synchronizeable<Unsafe> {
protected:

  /**
    Acquires an exclusive lock on this object.
  */
  inline void exclusiveLock() const throw() {};

  /**
    Acquires a shared lock on this object.
  */
  inline void sharedLock() const throw() {};

  /**
    Releases the lock on this object.
  */
  inline void releaseLock() const throw() {};
};

#endif
