/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ServiceNotFound.h"

ServiceNotFound::ServiceNotFound() {
}

ServiceNotFound::ServiceNotFound(const char* message) : NetworkException(message) {
}
