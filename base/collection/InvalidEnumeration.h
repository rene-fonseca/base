/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__INVALID_ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__INVALID_ENUMERATION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates that an enumeration is used incorrectly.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InvalidEnumeration : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidEnumeration();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidEnumeration(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
