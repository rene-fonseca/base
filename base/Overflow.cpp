/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Overflow.h"

Overflow::Overflow() throw() {
}

Overflow::Overflow(const char* message) throw() : Exception(message) {
}
