/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_DEVICE_EXCEPTION_H
#define _BASE_DEVICE_EXCEPTION_H

#include "ResourceException.h"

/**
  Thrown on device access error.

  @author René Møller Fonseca
  @version 1.0
*/

class DeviceException : public ResourceException {
public:
  DeviceException();
  ~DeviceException();
};

#endif
