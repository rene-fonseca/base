/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutOfRange.h"

OutOfRange::OutOfRange() throw() {
}

OutOfRange::OutOfRange(const char* message) throw() : Exception(message) {
}
