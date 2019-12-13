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

#include <base/Listener.h>
#include <base/collection/Set.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Class responsible for sending a given signal to multiple listeners.
  
  @short Event dispatcher.
  @version 1.0
*/

template<class SENDER, class LISTENER>
class EventDispatcher : public Object {
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
  EventDispatcher(const Signal<Sender, Listener>& _signal) noexcept
    : signal(_signal) {
  }

  /**
    Registers the specified listener.

    @param listener The listener to be deregistered.
  */
  void add(Listener* listener) noexcept {
    listeners.add(listener);
  }

  /**
    Deregisters the specified listener.

    @param listener The listener to be deregistered.
  */
  void remove(Listener* listener) noexcept {
    listeners.remove(listener);
  }

  /**
    Sends the signal to the registered listeners.
  */
  void notify() const noexcept {
    forEach(listeners, signal);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
