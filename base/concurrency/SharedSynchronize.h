/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__SHARED_SYNCHRONIZE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__SHARED_SYNCHRONIZE_H

#include <base/concurrency/Synchronizeable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short A stack based wrapper of a synchronize able class.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class GUARD>
class SharedSynchronize {
private:

  /** The synchronize able object to be synchronized. */
  const GUARD& guard;

  SharedSynchronize(const SharedSynchronize& copy); // prohibit copy construction
  SharedSynchronize& operator=(const SharedSynchronize& eq); // prohibit assignment
public:

  /**
    Initializes the synchronization object.

    @param obj The synchronize able object to be synchronized.
  */
  explicit SharedSynchronize(const GUARD& guard) throw();

  /**
    Releases the lock if not already released and destroys the synchronization object.
  */
  inline ~SharedSynchronize() throw() {
    guard.releaseLock();
  }
};

template<class GUARD>
inline SharedSynchronize<GUARD>::SharedSynchronize(const GUARD& g) throw() : guard(g) {
  guard.sharedLock();
}



/**
  Optimized version of Synchronize intended for single threaded appliances.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<>
class SharedSynchronize<Unsafe> {
private:

  SharedSynchronize(const SharedSynchronize& copy); // prohibit copy construction
  SharedSynchronize& operator=(const SharedSynchronize& eq); // prohibit assignment
public:

  /**
    Initializes the synchronization object.

    @param obj The synchronize able object to be synchronized.
  */
  inline explicit SharedSynchronize(const Unsafe& guard) throw() {}

  /**
    Releases the lock if not already released and destroys the synchronization object.
  */
  inline ~SharedSynchronize() throw() {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
