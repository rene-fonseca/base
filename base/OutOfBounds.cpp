/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/OutOfBounds.h>

OutOfBounds::OutOfBounds() throw() {
}

OutOfBounds::OutOfBounds(const char* message) throw() : Exception(message) {
}
