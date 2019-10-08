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

#include <base/xml/SAXException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An XMLReader will raise this exception when it finds an unrecognized feature
  or property identifier; SAX applications and extensions may use this class
  for other, similar purposes.
  
  @short Exception for an unrecognized identifier.
  @ingroup exceptions xml
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API SAXNotRecognizedException : public SAXException {
public:

  /**
    Initializes the exception with no message.
  */
  inline SAXNotRecognizedException() throw() {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline SAXNotRecognizedException(const char* message) throw()
    : SAXException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SAXNotRecognizedException(Type type) throw()
    : SAXException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SAXNotRecognizedException(const char* message, Type type) throw()
    : SAXException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
