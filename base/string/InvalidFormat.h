/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__INVALID_FORMAT_H
#define _DK_SDU_MIP__BASE__INVALID_FORMAT_H

#include <base/Exception.h>

/**
  Invalid format exception is thrown when a string does not comply with a given format.

  @author René Møller Fonseca
  @version 1.0
*/

class InvalidFormat : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidFormat();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidFormat(const char* message);
};

#endif
