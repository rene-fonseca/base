/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Underflow.h>

Underflow::Underflow() throw() {
}

Underflow::Underflow(const char* message) throw() : Exception(message) {
}
