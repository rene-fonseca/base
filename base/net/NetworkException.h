/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Indicates a network exception.
  
  @short Network IO exception.
  @ingroup exceptions net
  @version 1.1
*/

class _DK_SDU_MIP__BASE__API NetworkException : public IOException {
public:

  /** Cause. */
  enum Cause {
    ALREADY_CONNECTED, /**< Socket has already been connected. */
    NOT_CONNECTED, /**< Socket has not been connected. */
    CONNECTION_TIMED_OUT, /**< Connection timed out. */
    ADDRESS_NOT_AVAILABLE, /**< Address is not available. */
    ADDRESS_IN_USE, /**< Address already in use. */
    DESTINATION_REQUIRED, /**< Destination address is required. */
    FAMILY_NOT_SUPPORTED, /**< Family not supported. */
    PROTOCOL_NOT_SUPPORTED, /**< Protocol not supported. */
    TYPE_NOT_SUPPORTED, /**< Type not supported. */
    OPERATION_NOT_SUPPORTED, /**< Operation not supported. */
    IN_PROGRESS, /**< Operation already in progress. */
    NON_BLOCKING, /**< Operation would have blocked. */
    CONNECTION_ABORTED, /**< Connection aborted locally. */
    CONNECTION_REFUSED, /**< Connection refused. */
    CONNECTION_RESET, /**< Connection aborted by peer. */
    HOST_DOWN, /**< Host is down or disconnected. */
    HOST_UNREACHABLE, /**< Host is not reachable. */
    PREVIOUS_DATAGRAM_REJECTED, /**< A previous datagram could not be delivered due to invalid message size. */
    NETWORK_DOWN, /**< Local network is not operational. */
    NETWORK_RESET, /**< Connection aborted by network. */
    NETWORK_UNREACHABLE /**< Network is not reachable. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  inline NetworkException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline NetworkException(const char* message) throw() : IOException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline NetworkException(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline NetworkException(const char* message, Type type) throw()
    : IOException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
