/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/net/NetworkException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

NetworkException::NetworkException() throw() : IOException() {
}

NetworkException::NetworkException(const char* message) throw() :
  IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
