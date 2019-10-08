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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Raised on memory corruption such as buffer overflows.
  
  @short Memory corruption exception.
  @ingroup exceptions memory
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API MemoryCorruption : public MemoryException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline MemoryCorruption() throw() {
  }
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  inline MemoryCorruption(const char* message) throw()
    : MemoryException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline MemoryCorruption(Type type) throw() : MemoryException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline MemoryCorruption(const char* message, Type type) throw()
    : MemoryException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
