/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/NetworkException.h>

NetworkException::NetworkException() throw() : IOException() {
}

NetworkException::NetworkException(const char* message) throw() :
  IOException(message) {
}
