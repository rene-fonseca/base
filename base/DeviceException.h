/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H

#include <base/ResourceException.h>

/**
  Thrown on device access error.

  @author René Møller Fonseca
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

#endif
