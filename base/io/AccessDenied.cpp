/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/AccessDenied.h>

AccessDenied::AccessDenied() throw() : IOException() {
}

AccessDenied::AccessDenied(const char* message) throw() : IOException(message) {
}
