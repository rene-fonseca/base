/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "NetworkException.h"

NetworkException::NetworkException() {
}

NetworkException::NetworkException(const char* message) : Exception(message) {
}
