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

#include <base/rmi/RMIException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) exception.
  
  @short Object Request Broker (ORB) exception.
  @ingroup exceptions rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbException : public RMIException {
public:

  /** Exception causes. */
  enum Causes {
    OUT_OF_MEMORY, /**< Out of memory. */
    NOT_SUPPORTED, /**< Operation is not supported. */
    INVALID_ENCODING_NAMESPACE, /**< Invalid encoding scheme namespace. */
    INVALID_PROTOCOL_NAMESPACE, /**< Invalid protocol namespace. */
    SCHEME_NOT_REGISTERED, /**< Scheme has not been registered. */
    SCHEME_MISSING, /**< Scheme is missing within URI. */
    NOT_AN_END_POINT, /**< URI does not specify an end point. */
    INVALID_SCHEME, /**< Scheme may be known but is invalid in context. */
    SCHEME_MISMATCH, /**< Scheme is not as expected. */
    UNABLE_TO_ESTABLISH_CONNECTION, /**< Unable to establish connection. */
    INVALID_ENCODING, /**< Violation of encoding scheme. */
    UNABLE_TO_INITIALIZE_FACTORY, /**< Unable to initialize factory. */
    INVALID_METHOD /**< Method not found. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  inline OrbException() noexcept
  {
  }
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  inline OrbException(const char* message) noexcept
    : RMIException(message)
  {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline OrbException(const Type& type) noexcept
    : RMIException(type)
  {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline OrbException(const char* message, const Type& type) noexcept
    : RMIException(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
