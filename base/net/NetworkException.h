/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H
#define _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H

#include "base/Exception.h"

/**
  Indicates a network exception.

  @author René Møller Fonseca
  @version 1.0
*/

class NetworkException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  NetworkException();

  /**
    Initializes the exception object.

    @param message The message.
  */
  NetworkException(const char* message);
};

#endif
