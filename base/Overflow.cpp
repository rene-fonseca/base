/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Overflow.h"

Overflow::Overflow() {
}

Overflow::Overflow(const string& str) : Exception(str) {
}

Overflow::~Overflow() {
}
