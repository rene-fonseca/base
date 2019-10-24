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

#include <base/communication/CommunicationsException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Exception raised by the IEEE 1394 classes.

  @short Exception raised by the IEEE 1394 classes.
  @ingroup exceptions communications
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IEEE1394Exception : public CommunicationsException {
public:

  /**
    Initializes the exception object with no message.
  */
  IEEE1394Exception() noexcept;

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  IEEE1394Exception(const char* message) noexcept;

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  IEEE1394Exception(Type type) noexcept : CommunicationsException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  IEEE1394Exception(const char* message, Type type) noexcept : CommunicationsException(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
