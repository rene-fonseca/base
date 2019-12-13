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

#include <base/Date.h>
#include <base/xml/Node.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class EventTarget;

/**
  @short DOM event.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DOMEvent : public Object {
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
  EventTarget* target = nullptr;
  /**
    Used to indicate the EventTarget whose EventListeners are currently being
    processed. This is particularly useful during capturing and bubbling.
  */
  EventTarget* currentTarget = nullptr;
  /**
    Used to indicate which phase of event flow is currently being evaluated.
  */
  PhaseType phase = CAPTURING_PHASE;
  /**
    Used to indicate whether or not an event is a bubbling event. If the event
    can bubble the value is true, else the value is false.
  */
  bool bubbles = false;
  /**
    Used to indicate whether or not an event can have its default action
    prevented. If the default action can be prevented the value is true, else
    the value is false.
  */
  bool cancelable = false;
  /**
    Used to specify the time at which the event was created. Due to the fact
    that some systems may not provide this information the value of timeStamp
    may be not available for all events. When not available, a value of 0 will
    be returned.
  */
  Date timeStamp;
public:

  DOMEvent() noexcept {
  }

  inline const String& getType() const noexcept {
    return type;
  }

  inline EventTarget* getTarget() const noexcept {
    return target;
  }
  
  inline EventTarget* getCurrentTarget() const noexcept {
    return currentTarget;
  }
  
  inline PhaseType getPhase() const noexcept {
    return phase;
  }

  inline bool getBubbles() const noexcept {
    return bubbles;
  }

  inline bool isCancelable() const noexcept {
    return cancelable;
  }

  inline Date getTimeStamp() const noexcept {
    return timeStamp;
  }

  void stopPropagation() noexcept;
  
  void preventDefault() noexcept;
  
  void initEvent(
    const String& eventTypeArg, 
    bool canBubbleArg, 
    bool cancelableArg) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
