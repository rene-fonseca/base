/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InvalidFormat.h"

InvalidFormat::InvalidFormat() {
}

InvalidFormat::InvalidFormat(const char* str) : Exception(str) {
}
