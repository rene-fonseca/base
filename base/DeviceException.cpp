/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "DeviceException.h"

DeviceException::DeviceException() {
}

DeviceException::DeviceException(const string& str) : ResourceException(str) {
}

DeviceException::~DeviceException() {
}
