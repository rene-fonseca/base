/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Underflow.h"

Underflow::Underflow() {
}

Underflow::Underflow(const string& str) : Exception(str) {
}

Underflow::~Underflow() {
}
