/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__RUNNABLE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__RUNNABLE_H

#include <base/Object.h>
#include <base/concurrency/Thread.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Thread;

/**
  Base class of active objects. An active object can be run by a thread object.

  @short Base class of active objects.
  @ingroup concurrency
  @see Thread
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Runnable : public virtual Object {
protected:

  /** Specifies that the active object should be terminated. */
  bool terminated;
public:
 
  /**
    Initializes runnable object.
  */
  inline Runnable() throw() : terminated(false) {}

  /**
    Entry point for the thread.
  */
  virtual void run() = 0;

  /**
    Returns true if the active object should be terminated.
  */
  inline virtual bool isTerminated() throw() {return terminated;}

  /**
    Invocated when a child thread is completed. Watch out for MT-safety.

    @param child The child thread.
  */
  inline virtual void onChild(Thread* child) throw() {}

  /**
    Invocated when the thread is asked to terminate. Watch out for MT-safety.
  */
  inline virtual void onTermination() throw() {terminated = true;}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
