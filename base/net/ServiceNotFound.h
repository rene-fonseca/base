/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SERVICE_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_NET__SERVICE_NOT_FOUND_H

#include <base/net/NetworkException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception specifies that the service could not be resolved.

  @author René Møller Fonseca
  @version 1.0
*/

class ServiceNotFound : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  ServiceNotFound();

  /**
    Initializes the exception object.

    @param message The message.
  */
  ServiceNotFound(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
