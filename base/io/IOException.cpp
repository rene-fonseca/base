/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "IOException.h"

IOException::IOException() throw() : Exception() {
}

IOException::IOException(const char* message) throw() : Exception(message) {
}
