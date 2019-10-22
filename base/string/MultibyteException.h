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

#include <base/Exception.h>
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is raised on convertion errors from or to multibyte
  characters. A negative index indicates an invalid index.
  
  @short Multibyte character conversion exception.
  @ingroup exceptions
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API MultibyteException : public Exception {
private:
  
  /** Index of first invalid character. */
  MemoryDiff index = -1;
  /** Index of first invalid octet. */
  MemoryDiff octetIndex = -1;
  /** Index of first invalid octet within first invalid character. */
  MemoryDiff suboctetIndex = -1;
public:

  enum Cause {
    INVALID_CHARACTER_CODE, /**< Invalid character code. */
    INCOMPLETE_CHARACTER_CODE /**< Incomplete character code. */
  };
  
  /**
    Initializes the exception object with no message.
  */
  MultibyteException() noexcept {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  MultibyteException(const char* message) noexcept : Exception(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  MultibyteException(Type type) noexcept : Exception(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline MultibyteException(const char* message, Type type) noexcept
    : Exception(message, type) {
  }
  
  /**
    Returns the index of the first invalid character.
  */
  inline MemoryDiff getIndex() const noexcept {
    return index;
  }
  
  /**
    Sets the index of the first invalid character.
  */
  inline void setIndex(MemoryDiff index) noexcept {
    this->index = index;
  }
  
  /**
    Returns the octet index.
  */
  inline MemoryDiff getOctetIndex() const noexcept {
    return octetIndex;
  }
  
  /**
    Sets the index of the first invalid octet.
  */
  inline void setOctetIndex(MemoryDiff _octetIndex) noexcept {
    octetIndex = _octetIndex;
  }
  
  /**
    Returns the index of the first invalid octet within the first invalid
    character.
  */
  inline MemoryDiff getSuboctetIndex() const noexcept {
    return suboctetIndex;
  }
  
  /**
    Sets the index of the first invalid octet within the first invalid
    character.
  */
  inline void setSuboctetIndex(uint8 _suboctetIndex) noexcept {
    suboctetIndex = _suboctetIndex;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
