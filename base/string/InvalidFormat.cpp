/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InvalidFormat.h"

InvalidFormat::InvalidFormat() {
}

InvalidFormat::InvalidFormat(const char* str) : Exception(str) {
}
