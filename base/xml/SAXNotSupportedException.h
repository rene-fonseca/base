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

#include <base/xml/SAXException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  An XMLReader will raise this exception when it recognizes a feature or
  property identifier, but cannot perform the requested operation (setting a
  state or value). Other SAX2 applications and extensions may use this class
  for similar purposes.
  
  @short Exception for unsupported SAX operations.
  @ingroup exceptions xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API SAXNotSupportedException : public SAXException {
public:

  /**
    Initializes the exception with no message.
  */
  inline SAXNotSupportedException() noexcept {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline SAXNotSupportedException(const char* message) noexcept
    : SAXException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SAXNotSupportedException(Type type) noexcept
    : SAXException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SAXNotSupportedException(const char* message, Type type) noexcept
    : SAXException(message, type) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
