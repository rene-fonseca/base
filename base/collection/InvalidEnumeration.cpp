/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/InvalidEnumeration.h>

InvalidEnumeration::InvalidEnumeration() {
}

InvalidEnumeration::InvalidEnumeration(const char* message) : Exception(message) {
}
