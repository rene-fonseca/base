/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/OutOfRange.h>

OutOfRange::OutOfRange() throw() {
}

OutOfRange::OutOfRange(const char* message) throw() : Exception(message) {
}
