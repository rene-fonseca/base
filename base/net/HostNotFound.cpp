/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "HostNotFound.h"

HostNotFound::HostNotFound() {
}

HostNotFound::HostNotFound(const char* message) : NetworkException(message) {
}
