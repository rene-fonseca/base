/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/DeviceException.h>

DeviceException::DeviceException() throw() {
}

DeviceException::DeviceException(const char* message) throw() : ResourceException(message) {
}
