/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "DeviceException.h"

DeviceException::DeviceException() {
}

DeviceException::DeviceException(const char* message) : ResourceException(message) {
}
