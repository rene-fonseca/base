/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__SAX_NOT_SUPPORTED_EXCEPTION_H
#define _DK_SDU_MIP__BASE_XML__SAX_NOT_SUPPORTED_EXCEPTION_H

#include <base/xml/SAXException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An XMLReader will raise this exception when it recognizes a feature or
  property identifier, but cannot perform the requested operation (setting a
  state or value). Other SAX2 applications and extensions may use this class
  for similar purposes.
  
  @short Exception for unsupported SAX operations.
  @ingroup exceptions xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SAXNotSupportedException : public SAXException {
public:

  /**
    Initializes the exception with no message.
  */
  inline SAXNotSupportedException() throw() {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline SAXNotSupportedException(const char* message) throw()
    : SAXException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SAXNotSupportedException(Type type) throw()
    : SAXException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SAXNotSupportedException(const char* message, Type type) throw()
    : SAXException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
