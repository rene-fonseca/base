/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/OutOfDomain.h>

OutOfDomain::OutOfDomain() throw() {
}

OutOfDomain::OutOfDomain(const char* message) throw() : Exception(message) {
}
