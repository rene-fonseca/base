/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__RUNNABLE_H
#define _DK_SDU_MIP__BASE_THREAD__RUNNABLE_H

#include "Thread.h"

class Thread;

/**
  This interface is implemented by active objects. An active object can be run by a thread object.

  @short Interface implemented by active objects.
  @see Thread
  @author René Møller Fonseca
  @version 1.0
*/

class Runnable {
public:

  /**
    Entry point for the thread.
  */
  virtual void run() = 0;

  /**
    Returns true if the object should terminate (includes a cancellation point).
  */
//  inline virtual bool isTerminated() {Thread::allowCancellation(); return Thread::getTread()->isTerminated();};

  /**
    Called if the thread is cancelled. It is possible that the thread is killed while holding a resource, such as a lock or allocated memory. Watch out for MT-safety.
  */
  inline virtual void onCancellation() {};

  /**
    Called when a child thread is terminated (stops running). Watch out for MT-safety.

    @param thread The child thread.
  */
  inline virtual void onChild(Thread* thread) {};

  /**
    Called when the thread is asked to terminate. Watch out for MT-safety.
  */
  inline virtual void onTermination() {};
};

#endif
