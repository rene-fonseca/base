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

#include <base/xml/TransformerException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  XPath exception.
  
  @short XPath exception.
  @ingroup exceptions xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API XPathException : public TransformerException {
public:

  /**
    Initializes the exception with no message.
  */
  inline XPathException() throw() {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline XPathException(const char* message) throw()
    : TransformerException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline XPathException(const Type& type) throw() : TransformerException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline XPathException(const char* message, const Type& type) throw()
    : TransformerException(message, type) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
