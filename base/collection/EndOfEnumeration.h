/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H

#include <base/features.h>
#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown when the end of an enumeration is exceeded.

  @short End of enumeration exception  
  @ingroup collections exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class EndOfEnumeration : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  EndOfEnumeration();

  /**
    Initializes the exception object.

    @param message The message.
  */
  EndOfEnumeration(const char* message);
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  EndOfEnumeration(Type type) throw() {}
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  EndOfEnumeration(const char* message, Type type) throw() {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
