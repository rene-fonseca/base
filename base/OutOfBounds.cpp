/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutOfBounds.h"

OutOfBounds::OutOfBounds() throw() {
}

OutOfBounds::OutOfBounds(const char* message) throw() : Exception(message) {
}
