/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__INVALID_KEY_H
#define _DK_SDU_MIP__BASE_COLLECTION__INVALID_KEY_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception specifies that a key of a collection is invalid.

  @author René Møller Fonseca
  @version 1.0
*/

class InvalidKey : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidKey();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidKey(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
