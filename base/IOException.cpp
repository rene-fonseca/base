/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "IOException.h"

IOException::IOException() {
}

IOException::IOException(const string& str) : Exception(str) {
}

IOException::~IOException() {
}
