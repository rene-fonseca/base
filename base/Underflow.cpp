/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Underflow.h"

Underflow::Underflow() {
}

Underflow::Underflow(const string& str) : Exception(str) {
}

Underflow::~Underflow() {
}
