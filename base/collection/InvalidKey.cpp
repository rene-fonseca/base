/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InvalidKey.h"

InvalidKey::InvalidKey() {
}

InvalidKey::InvalidKey(const char* message) : Exception(message) {
}
