/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutOfBounds.h"

OutOfBounds::OutOfBounds() {
}

OutOfBounds::OutOfBounds(const string& str) : Exception(str) {
}

OutOfBounds::~OutOfBounds() {
}
