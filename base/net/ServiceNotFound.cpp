/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ServiceNotFound.h"

ServiceNotFound::ServiceNotFound() {
}

ServiceNotFound::ServiceNotFound(const char* message) : NetworkException(message) {
}
