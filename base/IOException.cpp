/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "IOException.h"

IOException::IOException() {
}

IOException::IOException(const string& str) : Exception(str) {
}

IOException::~IOException() {
}
