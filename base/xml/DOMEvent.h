/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Date.h>
#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class EventTarget;

/**
  @short DOM event.
  @ingroup xml
  @version 1.0
*/

class DOMEvent : public Object {
public:

  /** The phase of the event flow which is being processed. */
  enum PhaseType {
    CAPTURING_PHASE, /**< The current event phase is the capturing phase. */
    AT_TARGET,/**< The event is currently being evaluated at the target. */
    BUBBLING_PHASE/**< The current event phase is the bubbling phase. */
  };
private:

  /**
    The name of the event (case-insensitive). The name must be an XML name.
  */
  String type;
  /**
    Used to indicate the EventTarget to which the event was originally
    dispatched.
  */
  EventTarget* target;
  /**
    Used to indicate the EventTarget whose EventListeners are currently being
    processed. This is particularly useful during capturing and bubbling.
  */
  EventTarget* currentTarget;
  /**
    Used to indicate which phase of event flow is currently being evaluated.
  */
  PhaseType phase;
  /**
    Used to indicate whether or not an event is a bubbling event. If the event
    can bubble the value is true, else the value is false.
  */
  bool bubbles;
  /**
    Used to indicate whether or not an event can have its default action
    prevented. If the default action can be prevented the value is true, else
    the value is false.
  */
  bool cancelable;
  /**
    Used to specify the time at which the event was created. Due to the fact
    that some systems may not provide this information the value of timeStamp
    may be not available for all events. When not available, a value of 0 will
    be returned.
  */
  Date timeStamp;
public:

  DOMEvent() throw() {
  }

  inline String getType() const throw() {
    return type;
  }

  inline EventTarget* getTarget() const throw() {
    return target;
  }
  
  inline EventTarget* getCurrentTarget() const throw() {
    return currentTarget;
  }
  
  inline PhaseType getPhase() const throw() {
    return phase;
  }

  inline bool getBubbles() const throw() {
    return bubbles;
  }

  inline bool isCancelable() const throw() {
    return cancelable;
  }

  inline Date getTimeStamp() const throw() {
    return timeStamp;
  }

  void stopPropagation() throw();
  
  void preventDefault() throw();
  
  void initEvent(
    const String& eventTypeArg, 
    bool canBubbleArg, 
    bool cancelableArg) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
