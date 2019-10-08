/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Exception.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised on convertion errors from or to multibyte
  characters. A negative index indicates an invalid index.
  
  @short Multibyte character conversion exception.
  @ingroup exceptions
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API MultibyteException : public Exception {
private:
  
  /** Index of first invalid character. */
  int index = -1;
  /** Index of first invalid octet. */
  int octetIndex = -1;
  /** Index of first invalid octet within first invalid character. */
  int suboctetIndex = -1;
public:

  enum Cause {
    INVALID_CHARACTER_CODE, /**< Invalid character code. */
    INCOMPLETE_CHARACTER_CODE /**< Incomplete character code. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  MultibyteException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  MultibyteException(const char* message) throw() : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  MultibyteException(Type type) throw() : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline MultibyteException(
    const char* message,
    Type type) throw()
    : Exception(message, type) {
  }
  
  /**
    Returns the index of the first invalid character.
  */
  inline int getIndex() const throw() {
    return index;
  }
  
  /**
    Sets the index of the first invalid character.
  */
  inline void setIndex(unsigned int index) throw() {
    this->index = index;
  }
  
  /**
    Returns the octet index.
  */
  inline int getOctetIndex() const throw() {
    return octetIndex;
  }
  
  /**
    Sets the index of the first invalid octet.
  */
  inline void setOctetIndex(unsigned int _octetIndex) throw() {
    octetIndex = _octetIndex;
  }
  
  /**
    Returns the index of the first invalid octet within the first invalid
    character.
  */
  inline int getSuboctetIndex() const throw() {
    return suboctetIndex;
  }
  
  /**
    Sets the index of the first invalid octet within the first invalid
    character.
  */
  inline void setSuboctetIndex(uint8 _suboctetIndex) throw() {
    suboctetIndex = _suboctetIndex;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
