/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/InvalidKey.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

InvalidKey::InvalidKey() {
}

InvalidKey::InvalidKey(const char* message) : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
