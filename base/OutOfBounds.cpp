/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutOfBounds.h"

OutOfBounds::OutOfBounds() {
}

OutOfBounds::OutOfBounds(const string& str) : Exception(str) {
}

OutOfBounds::~OutOfBounds() {
}
