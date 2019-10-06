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
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception will include information for locating the error in the
  original XML document. Note that although the application will receive a
  SAXParseException as the argument to the handlers in the ErrorHandler
  interface, the application is not actually required to throw the exception;
  instead, it can simply read the information in it and take a different
  action.
  
  @short Encapsulate an XML parse error or warning.
  @ingroup exceptions xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SAXParseException : public SAXException {
private:

  /**
    The public identifer of the entity that generated the error or warning.
  */
  String publicId;
  /**
    The system identifer of the entity that generated the error or warning.
  */
  String systemId;
  /** The line number. */
  unsigned int line;
  /** The column number. */
  unsigned int column;
public:
  
  /** Exception causes. */
  enum Cause {
    NOT_WELL_FORMED /**< The document is not well-formed. */
  };
  
  /**
    Initializes the exception with no message.
  */
  inline SAXParseException() throw() : line(0), column(0) {
  }

  /**
    Initializes the exception with the specified message.
  */
  inline SAXParseException(const char* message) throw()
    : SAXException(message), line(0), column(0) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline SAXParseException(Type type) throw()
    : SAXException(type), line(0), column(0) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline SAXParseException(const char* message, Type type) throw()
    : SAXException(message, type), line(0), column(0) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
    @param publicId The public identifier of the entity where the exception
    occurred.
    @param systemId The system identifier of the entity where the exception
    occurred.
    @param line The line number.
    @param column The column number.
  */
  inline SAXParseException(
    const char* message,
    Type type,
    String _publicId,
    String _systemId,
    unsigned int _line,
    unsigned int _column) throw()
    : SAXException(message, type),
      publicId(_publicId),
      systemId(_systemId),
      line(_line),
      column(_column) {
  }

  /**
    Returns the line number.
  */
  inline unsigned int getLine() const throw() {
    return line;
  }

  /**
    Returns the column number.
  */
  inline unsigned int getColumn() const throw() {
    return column;
  }
  
  /**
    Returns the public identifier of the entity where the exception occurred.
  */
  inline String getPublicId() const throw() {
    return publicId;
  }
  
  /**
    Returns the system identifier of the entity where the exception occurred.
  */
  inline String getSystemId() const throw() {
    return systemId;
  }

  inline ~SAXParseException() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
