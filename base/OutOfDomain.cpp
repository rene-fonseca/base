/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/OutOfDomain.h>

OutOfDomain::OutOfDomain() throw() {
}

OutOfDomain::OutOfDomain(const char* message) throw() : Exception(message) {
}
