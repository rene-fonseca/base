/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Underflow.h>

Underflow::Underflow() throw() {
}

Underflow::Underflow(const char* message) throw() : Exception(message) {
}
