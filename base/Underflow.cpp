/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Underflow.h"

Underflow::Underflow() throw() {
}

Underflow::Underflow(const char* message) throw() : Exception(message) {
}
