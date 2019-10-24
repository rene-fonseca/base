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

#include <base/collection/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Base class for listeners.
  
  @short Base class for listeners.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Listener {
public:
};



/**
  Class responsible for dispatching a signal to a listener.

  @short Signal dispatcher.
  @version 1.0
*/

template<class SENDER, class LISTENER>
class Signal : public UnaryOperation<LISTENER*, void> {
private:

  /** The type of the sender. */
  typedef SENDER Sender;
  /** The type of the listener. */
  typedef LISTENER Listener;
  /** The type of the member function. */
  typedef void (Listener::*Member)(const Sender*);
  /** The sender. */
  const Sender* sender = nullptr;
  /** The end-point of the signal. */
  Member member;

  /** Disable default assignment. */
  Signal& operator=(const Signal& eq) noexcept;
public:

  /**
    Initializes the signal.

    @param sender The sender.
    @param member The end-point of the signal.
  */
  inline Signal(const Sender* _sender, Member _member) noexcept
    : sender(_sender), member(_member) {
  }
  
  /**
    Initialize signal from other signal.
  */
  inline Signal(const Signal& copy) noexcept
    : sender(copy.sender), member(copy.member) {
  }

  /**
    Sends the signal.
  */
  inline void operator()(Listener* listener) const {
    (listener->*member)(sender);
  }
};



/**
  Returns a Signal object for the specified sender and member function.
*/
template<class SENDER, class LISTENER>
inline Signal<SENDER, LISTENER> signal(
  const SENDER* sender, void (LISTENER::*member)(const SENDER*)) {
  return Signal<SENDER, LISTENER>(sender, member);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
