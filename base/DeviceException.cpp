/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/DeviceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

DeviceException::DeviceException() throw() {
}

DeviceException::DeviceException(const char* message) throw() : ResourceException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
