/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__INVALID_ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__INVALID_ENUMERATION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates that an enumeration is used incorrectly.

  @author René Møller Fonseca
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
