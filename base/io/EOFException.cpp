/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "EOFException.h"

EOFException::EOFException() {
}

EOFException::EOFException(const char* str) : Exception(str) {
}
