/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ResourceException.h"

ResourceException::ResourceException() {
}

ResourceException::ResourceException(const char* message) : Exception(message) {
}
