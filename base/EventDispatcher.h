/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__EVENT_DISPATCHER_H
#define _DK_SDU_MIP__BASE__EVENT_DISPATCHER_H

#include <base/Listener.h>
#include <base/collection/Set.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class responsible for sending a given signal to multiple listeners.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

template<class SENDER, class LISTENER>
class EventDispatcher : public virtual Object {
public:

  /** The type of the sender. */
  typedef SENDER Sender;
  /** The type of the listener. */
  typedef LISTENER Listener;
protected:

  /** The registered listeners. */
  Set<Listener*> listeners;
  /** The signal to be send to the listeners. */
  Signal<Sender, Listener> signal;
public:

  /**
    Initializes an event dispatcher.

    @param signal The signal to be send on request.
  */
  EventDispatcher(const Signal<Sender, Listener>& signal) throw() : signal(signal) {}

  /*
    Registers the specified listener.

    @param listener The listener to be deregistered.
  */
  void add(Listener* listener) throw() {
    listeners.add(listener);
  }

  /**
    Deregisters the specified listener.

    @param listener The listener to be deregistered.
  */
  void remove(Listener* listener) throw() {
    listeners.remove(listener);
  }

  /**
    Sends the signal to the registered listeners.
  */
  void notify() const throw() {
    forEach(listeners, signal);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
