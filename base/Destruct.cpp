/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Destruct.h"

Destruct::Destruct() throw() {
}

Destruct::Destruct(const char* message) throw() : Exception(message) {
}
