/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_EVENT_H
#define _BASE_EVENT_H

#include "MutualExclusion.h"
#include "base/RangeException.h"

/**
  Event.

  @author René Møller Fonseca
  @version 1.0
*/

class Event : public MutualExclusion {
private:

  /** The maximum number of concurrent waiting threads. */
  unsigned int maximum;
  /** Indicates that the event is being reset. */
  bool resetting;
  /** Indicates that the event has been signaled. */
  bool signaled;
  /** The number of waiting threads. */
  unsigned int waitingThreads;
  /** Conditional. */
  pthread_cond_t condition;
public:

  /**
    Initializes the event.

    @param maximum The maximum number of waiting thread. The default is one.
  */
	Event(unsigned int maximum = 1) throw(Construct, ResourceException);

  /**
    Resets the event. Returns when all waiting threads.
  */
  void reset() throw(MutualExclusion::MutualExclusionException);

  /**
    Signal the waiting threads. Control is returned immediately.
  */
  void signal() throw(MutualExclusion::MutualExclusionException);

  /**
    Wait for signal. The executing thread is suspended until event is signaled. Will wait forever if never signaled. If the event if currently being reset the thread is suspended. Throws an 'RangeException' exception if the maximum number of waiting threads is exceeded.
  */
  void wait() throw(RangeException, MutualExclusion::MutualExclusionException);

  /**
    Wait for signal. The executing thread is suspended until event is signaled or a time out occures. If the event if currently being reset the thread is suspended. Throws an 'RangeException' exception if the maximum number of waiting threads is exceeded.

    @param microseconds The desired time out interval in microseconds.
  */
  bool wait(unsigned int microseconds) throw(RangeException, MutualExclusion::MutualExclusionException);

  /**
    Destroys the event.
  */
	~Event() throw();
};

#endif
