/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H
#define _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Indicates a network exception.

  @author René Møller Fonseca
  @version 1.01
*/

class NetworkException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  NetworkException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  NetworkException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
