/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "NetworkException.h"

NetworkException::NetworkException() throw() : IOException() {
}

NetworkException::NetworkException(const char* message) throw() :
  IOException(message) {
}
