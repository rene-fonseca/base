/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/EndOfEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

EndOfEnumeration::EndOfEnumeration() {
}

EndOfEnumeration::EndOfEnumeration(const char* message) : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
