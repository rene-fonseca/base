/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutOfRange.h"

OutOfRange::OutOfRange() {
}

OutOfRange::OutOfRange(const char* str) : Exception(str) {
}
