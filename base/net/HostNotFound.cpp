/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/HostNotFound.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

HostNotFound::HostNotFound() {
}

HostNotFound::HostNotFound(const char* message) : NetworkException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
