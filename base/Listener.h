/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__LISTENER_H
#define _DK_SDU_MIP__BASE__LISTENER_H

#include <base/collection/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base class for all listeners.

  @author René Møller Fonseca
  @version 1.0
*/

class Listener {
public:
};



/**
  Class responsible for dispatching a signal to a listener.

  @author René Møller Fonseca
  @version 1.0
*/

template<class SENDER, class LISTENER>
class Signal : public UnaryOperation<LISTENER*, void> {
private:

  /** The type of the sender. */
  typedef SENDER Sender;
  /** The type of the listener. */
  typedef LISTENER Listener;
  /** The type of the memeber function. */
  typedef void (Listener::*Member)(const Sender*);
  /** The sender. */
  const Sender* sender;
  /** The end-point of the signal. */
  Member member;

  /** Disable default assignment. */
  Signal& operator=(const Signal& eq) throw();
public:

  /**
    Initializes the signal.

    @param sender The sender.
    @param memeber The end-point of the signal.
  */
  inline Signal(const Sender* sender, Member member) throw() : sender(sender), member(member) {}

  /**
    Initialize signal from other signal.
  */
  inline Signal(const Signal& copy) throw() : sender(copy.sender), member(copy.member) {}

  /**
    Sends the signal.
  */
  inline void operator()(Listener* listener) const {(listener->*member)(sender);}
};



/**
  Returns a Signal object for the specified sender and member function.
*/
template<class SENDER, class LISTENER>
inline Signal<SENDER, LISTENER> signal(const SENDER* sender, void (LISTENER::*member)(const SENDER*)) {
  return Signal<SENDER, LISTENER>(sender, member);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
