/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H

#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on device access error.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DeviceException : public ResourceException {
public:

  /**
    Initializes the exception object with no message.
  */
  DeviceException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  DeviceException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
