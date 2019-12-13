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

#include <base/Object.h>
#include <base/collection/Array.h>
#include <base/collection/AlreadyKeyException.h>
#include <base/collection/InvalidKey.h>
#include <base/concurrency/SpinLock.h>
#include <base/concurrency/ConcurrencyException.h>
#include <base/mem/Allocator.h>
#include <base/net/StreamSocket.h>
#include <base/Listener.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API SocketListener : public Listener {
public:
  
  virtual void onSocketEvent(
    StreamSocket socket, unsigned int events) noexcept = 0;
};

/**
  Socket I/O multiplexer.
  
  @short Socket I/O multiplexer.
  @ingroup net
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MultipleSockets : public Object {
private:
  
  typedef SpinLock Guard;
  /** Guard. */
  Guard guard;
  /** Sockets. */
  Array<StreamSocket> streamSockets;
  /** Context. */
  Allocator<uint8> context;
  /** The current number of selected sockets. */
  unsigned int numberOfSelected = 0;
public:
  
  /** The granularity. */
  static const unsigned int GRANULARITY = 64;
  
  /** Events. */
  enum {
    /** Normal data may be read without blocking. */
    INPUT = 1,
    /** Priority data may be read without blocking. */
    PRIORITY_INPUT = 2,
    /** High priority data may be read without blocking. */
    HIGH_PRIORITY_INPUT = 4,
    /** Data may be written without blocking. */
    OUTPUT = 8,
    /** Priority data may be written without blocking. */
    PRIORITY_OUTPUT = 16,
    /** Error condition. */
    ERROR = 32,
    /** The connection has been disconnected. */
    DISCONNECTED = 64
  };
  
  /**
    Initializes multiple socket object.
  */
  MultipleSockets();
  
  /**
    Returns the current number of selected sockets.
  */
  inline unsigned int getSelected() const noexcept {
    return numberOfSelected;
  }
  
  /**
    Adds a socket with the specified events filter (only INPUT, PRIORITY_INPUT,
    HIGH_PRIORITY_INPUT, OUTPUT, and PRIORITY_OUTPUT are significant).

    @param socket The socket to be added.
    @param events The filter events. The default is all events.
  */
  void add(
    StreamSocket socket,
    unsigned int events =
      INPUT|PRIORITY_INPUT|HIGH_PRIORITY_INPUT|OUTPUT|PRIORITY_OUTPUT)
   ;
  
  /**
    Removes a socket.
    
    @param socket The socket to be removed.
  */
  void remove(StreamSocket socket);
  
  /**
    Returns the events for the specified socket. Raises InvalidKey if socket is
    invalid. The socket is deselected if present within the selected socket
    set.
  */
  unsigned int getEvents(
    StreamSocket socket);
  
  /**
    Returns the currently filtered events (mask of INPUT, PRIORITY_INPUT,
    HIGH_PRIORITY_INPUT, OUTPUT, and PRIORITY_OUTPUT).    
  */
  unsigned int getFilter(
    StreamSocket socket) const;
  
  /**
    Sets the events filter (only INPUT, PRIORITY_INPUT, HIGH_PRIORITY_INPUT,
    OUTPUT, and PRIORITY_OUTPUT are significant).
  */
  void setFilter(
    StreamSocket socket,
    unsigned int events);
  
  /**
    Waits for an event on one of the sockets.
    
    @return The number of sockets which have changed.
  */
  unsigned int poll();
  
  /**
    Waits for an event on one of the sockets. The time out period is silently
    clamped to 999999999.
    
    @param milliseconds The time out period.
    @return The number of sockets which have changed. 0 if interrrupted or
    timed out.
  */
  unsigned int poll(
    unsigned int milliseconds);
  
  /**
    Signals the listener for each selected socket. Resets the number of
    selected sockets.
  */
  void signal(SocketListener* listener);
  
  /**
    Destroys the object.
  */
  ~MultipleSockets() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
