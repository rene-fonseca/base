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

#include <base/io/IOException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Indicates a network exception.
  
  @short Network IO exception.
  @ingroup exceptions net
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API NetworkException : public IOException {
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
  NetworkException() noexcept;

  /**
    Initializes the exception object.

    @param message The message.
  */
  NetworkException(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  NetworkException(const Type& type) noexcept;
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  NetworkException(const char* message, const Type& type) noexcept;

  NetworkException(const char* message, const Type& type, unsigned int cause) noexcept;

  NetworkException(const char* message, const Type& type, unsigned int error, int) noexcept;

  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
