/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/InvalidEnumeration.h>

InvalidEnumeration::InvalidEnumeration() {
}

InvalidEnumeration::InvalidEnumeration(const char* message) : Exception(message) {
}
