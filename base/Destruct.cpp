/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Destruct.h"

Destruct::Destruct() {
}

Destruct::Destruct(const string& str) : Exception(str) {
}

Destruct::~Destruct() {
}
