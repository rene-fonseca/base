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

#include <base/MemoryException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This exception is raised when a 0 (NULL) pointer is dereferenced.
  
  @short Invalid pointer exception.
  @ingroup exceptions memory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API NullPointer : public MemoryException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline NullPointer() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline NullPointer(const char* message) throw() : MemoryException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline NullPointer(Type type) throw() : MemoryException(type) {
  }

  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline NullPointer(const char* message, Type type) throw()
    : MemoryException(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
