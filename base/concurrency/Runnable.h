/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__RUNNABLE_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__RUNNABLE_H

#include <base/Object.h>
#include <base/concurrency/Thread.h>

class Thread;

/**
  Base class of active objects. An active object can be run by a thread object.

  @short Base class of active objects.
  @see Thread
  @author René Møller Fonseca
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

#endif
