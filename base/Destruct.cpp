/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Destruct.h"

Destruct::Destruct() {
}

Destruct::Destruct(const char* message) : Exception(message) {
}
