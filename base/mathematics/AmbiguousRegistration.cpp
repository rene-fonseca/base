/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/AmbiguousRegistration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AmbiguousRegistration::AmbiguousRegistration() throw() {
}

AmbiguousRegistration::AmbiguousRegistration(const char* message) throw() : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
