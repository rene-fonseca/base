/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "EndOfEnumeration.h"

EndOfEnumeration::EndOfEnumeration() {
}

EndOfEnumeration::EndOfEnumeration(const char* message) : Exception(message) {
}
