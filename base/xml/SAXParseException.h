/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__SAX_PARSE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_XML__SAX_PARSE_EXCEPTION_H

#include <base/SAXException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  SAX parse exception.
  
  @short SAX parse exception.
  @ingroup exceptions xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SAXParseException : public Exception {
private:

  /** The line number. */
  unsigned int line;
  /** The column number. */
  unsigned int column;
public:

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
    @param line The line number.
    @param column The column number.
  */
  inline SAXParseException(
    const char* message,
    Type type,
    unsigned int _line,
    unsigned int _column) throw()
    : SAXException(message, type), line(_line), column(_column) {
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
  unsigned int getColumn() const throw() {
    return column;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
