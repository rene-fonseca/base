/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "AccessDenied.h"

AccessDenied::AccessDenied() throw() : IOException() {
}

AccessDenied::AccessDenied(const char* message) throw() : IOException(message) {
}
