/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "EOFException.h"

EOFException::EOFException() {
}

EOFException::EOFException(const char* str) : Exception(str) {
}
