/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/AmbiguousRegistration.h>

AmbiguousRegistration::AmbiguousRegistration() throw() {
}

AmbiguousRegistration::AmbiguousRegistration(const char* message) throw() : Exception(message) {
}
